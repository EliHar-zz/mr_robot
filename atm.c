#ifndef F_CPU
#define F_CPU 1000000UL
#endif
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

enum direction {FORWARD, BACKWARD};

#define ACK 0x7E

//*********************** Timer ***********************************
void start_timer() {
	cli();
	TIMSK1 = (1 << TOIE1);
	TCCR1B |= ((0 << CS12) | (1 << CS11) | (0 << CS10));
	sei();
}

void stop_timer() {
	cli();
	TCCR1B &= ((0 << CS12) | (0 << CS11) | (0 << CS10));
	PORTC &= (0 << PC3); // make sure led is off when stopped
	sei();
}

// timer interrupt, if timer is running this will toggle the led when timer counter overflows
ISR(TIMER1_OVF_vect) {
	cli();
	PORTC ^= (1 << PC3);
	sei();
}

/*
 * sends char to serial monitor
 */
void sendChar(char c) {
	// Wait until buffer is empty
	while (!(UCSR0A & (1 << UDRE0))) {}
	// Return the data from the RX buffer
	UDR0 = c;
}

/*
 * Send message to serial monitor
 */
void sendMessage(char msg[]) {
	for (int i = 0; msg[i]; i++) {
		sendChar(msg[i]);
		if (msg[i] == '#') {
			break;
		}
	}
}

//***************************  UART  *****************************
void UART_setup() {

	/*Set baud rate */
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	// Enable receiver and transmitter
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    // Set frame: 8data, 1 stp
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

    sendMessage(">> Initializing UART ...\n");
}

/*
 * Receives a character from serial monitor
 */
char receiveChar() {
	// Wait until data is received
	while (!(UCSR0A & (1 << RXC0))) {}
	// Return the data from the RX buffer
	return UDR0;
}

/*
 * Receives a message from serial monitor
 */
void receiveMessage(char* message) {
	int index = 0;
	char c = receiveChar();
	while (isdigit(c) || c == ',' || c == '-') { // Characters allowed to be received
		message[index++] = c;
		c = receiveChar();
	}
	message[index] = '\0';
}


char* itoa2(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

// WARNING: int should be < 1000
void sendInt(int num) {
	char num_str[4] = {' ', ' ', ' ', '\0'};
	itoa2(num, num_str);
	sendMessage(num_str);
	sendMessage("\n");
}

//************************ Control car ***********************************

// initialize pwm for enablers
void initialize_pwm() {
	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
	TCCR0B = (1 << CS00) | (1 << CS02);
}

void init_wheels() {

    sendMessage(">> Initializing wheels ...\n");

	// Set left wheels as output
	DDRD |= (1 << PD2);
	DDRD |= (1 << PD4);

	// Set right wheels as output
	DDRC |= (1 << PC2);
	DDRC |= (1 << PC0);

	// Set enabler left as output
	DDRD |= (1 << PD5);

	// Set enabler right as output
	DDRD |= (1 << PD6);

	initialize_pwm();
}

void power_left_wheel(enum direction dir, uint8_t duty) {
	if (duty < 0 || duty > 255) {
		return;
	}

	switch (dir) {
		case FORWARD:
			sendMessage("Powering left wheel forward by: ");
			sendInt(duty);
			PORTD |= (1 << PD2);
			PORTD &= ~(1 << PD4);
			break;
		case BACKWARD:
			sendMessage("Powering left wheel backward by: ");
			sendInt(duty);
			PORTD |= (1 << PD4);
			PORTD &= ~(1 << PD2);
			break;
		default:
			PORTD &= ~(1 << PD2);
			PORTD &= ~(1 << PD4);
	}

	OCR0B = duty;
}

void power_right_wheel(enum direction dir, uint8_t duty) {
	if (duty < 0 || duty > 255) {
		return;
	}

	switch (dir) {
		case FORWARD:
			sendMessage("Powering right wheel forward by: ");
			sendInt(duty);
			PORTC |= (1 << PC2);
			PORTC &= ~(1 << PC0);
			break;
		case BACKWARD:
			sendMessage("Powering right wheel backward by: ");
			sendInt(duty);
			PORTC |= (1 << PC0);
			PORTC &= ~(1 << PC2);
			break;
		default:
			PORTC &= ~(1 << PC2);
			PORTC &= ~(1 << PC0);
	}

	OCR0A = duty;
}

/*
 * Set the speed and direction of each wheel. +ive is forward, -ve is backward
 */
void set_wheels(int left_wheel, int right_wheel){

	if (left_wheel < 0) {
		power_left_wheel(BACKWARD, -left_wheel);
	} else {
		power_left_wheel(FORWARD, left_wheel);
	}

	if (right_wheel < 0) {
		power_right_wheel(BACKWARD, -right_wheel);
	} else {
		power_right_wheel(FORWARD, right_wheel);
	}
}

//**************************** SPI **************************************
// spi functions
void spi_init_slave (void) {

   sendMessage(">> Initializing slave\n");
    DDRB=(1 << 6);                                //MISO as OUTPUT
    SPCR=(1 << SPE);                              //Enable SPI
}

//Function to send and receive data
unsigned char spi_tranceiver (unsigned char data) {
    SPDR = data;                                  //Load data into buffer
    while(!(SPSR & (1<<SPIF) ));                  //Wait until transmission complete
    return(SPDR);                                 //Return received data
}

void *get_message(char *message) {
	int index = 0;
	char c = spi_tranceiver(ACK);
	while (c != '#') {
		message[index++] = c;
		c = spi_tranceiver(ACK);
	}
	message[index] = '\0';
}

//***************************** Text parsing *****************************
/*
 * Char to int
 */
int ctoi(char d) {
	char str[2];

	str[0] = d;
	str[1] = '\0';
	return (int) strtol(str, NULL, 10);
}

/*
 * Parses a string of integers
 */
 int parseInt(char* integer_str) {
 	int sign = 1;
 	int length = strlen(integer_str);
 	int i = 0;
 	if (integer_str[0] == '-') {
 		sign = -1;
 		i = 1;
 	}
 	int input_int = 0;
 	if (length >= 3) {
 		input_int = 1;
 	}
 	for (i; i < length; i++) {
 		input_int += ctoi(integer_str[i]) * pow(10, length - 1 - i);
 	}
 	return (input_int*sign);
 }

void parse_message(char *msg, int *nums) {
	int msg_index = 0;
	int num_index = 0;
	int k = 0;
	char c;
	char num[5];

	do {
		c = (char) msg[msg_index++];
		if (c == ',' || c == '#') {
			num[num_index] = '\0';
			nums[k++] = parseInt(num);
			num[0] = '\0'; num[1] = '\0', num[2] = '\0';
			num_index = 0;
		} else {
			num[num_index++] = c;
		}
	} while (c != '#');
}

int main(void) {

	UART_setup();
	init_wheels();
    spi_init_slave();

    unsigned char data, msg[16];
    while(1) {

		sendMessage("\nAwaiting message\n");
        int index = 0;

		do {
			data = spi_tranceiver(ACK);
			msg[index++] = data;
		} while (data != '#');

		sendMessage("Received message: ");
        sendMessage(msg);
		sendChar('\n');

		int nums[2] = {0, 0};
		parse_message(msg, nums);
		set_wheels(nums[0], nums[1]);
    }
}
