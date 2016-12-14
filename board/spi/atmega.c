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

//***************************  UART  *****************************
void UART_setup() {
	/*Set baud rate */
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	// Enable receiver and transmitter
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    // Set frame: 8data, 1 stp
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
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
	}
}

//************************ Control car ***********************************
void init_wheels() {
	// In back wheels, Set as output
	DDRD |= (1 << PD7);
	DDRC |= (1 << PC0);
	DDRC |= (1 << PC2);
	DDRC |= (1 << PC1);

	// *******************************************************
	// In front right wheel, Set as output
	DDRC |= (1 << PC5);
	DDRC |= (1 << PC4);

	// In front left wheel, Set as output
	DDRD |= (1 << PD2);
	DDRD |= (1 << PD4);

	// *******************************************************
	// Enabler for front right wheels, Set as output
	DDRD |= (1 << PD3);

	// Enabler for front left wheels, Set as output
	DDRD |= (1 << PD5);

	// Enabler for back wheels, Set as output
	DDRD |= (1 << PD6);

	// Set Timer0, Timer2 in Fast PWM counting, with a prescaler 0
	// The OC0A, OC0B, OC2A, OC2B pin is cleared on match.
	// Non Inverted PWM output mode
	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
	TCCR0B = (1 << CS00);

	TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2A1) | (1 << COM2B1);
	TCCR2B = (1 << CS20);
}

/*
 * Moves front right wheel. Takes direction and duty cycle as argumet
 */
void fr_right(enum direction dir, uint8_t duty) {
	if (duty < 0 || duty > 255) {
		return;
	}
	switch(dir) {
		case FORWARD:
			PORTC |= (1 << PC4); // Set to High
			PORTC &= ~(1 << PC5);// Set to Low
			break;
		case BACKWARD:
			PORTC |= (1 << PC5); // Set to High
			PORTC &= ~(1 << PC4); // Set to Low
			break;
		default:
			PORTC &= ~(1 << PC4); // Set to Low
			PORTC &= ~(1 << PC5); // Set to Low
	}

	OCR2B = duty;
}

/*
 * Moves front left wheel. Takes direction and duty cycle as argumet
 */
void fr_left(enum direction dir, uint8_t duty) {

	if (duty < 0 || duty > 255) {
		return;
	}
	switch(dir) {
		case FORWARD:
			PORTD |= (1 << PD4);
			PORTD &= ~(1 << PD2);
			break;
		case BACKWARD:
			PORTD |= (1 << PD2);
			PORTD &= ~(1 << PD4);
			break;
		default:
			PORTD &= ~(1 << PD4);
			PORTD &= ~(1 << PD2);
	}

	OCR0B = duty;
}

/*
 * Moves back left wheel. Takes direction and duty cycle as argumet
 */
void bk_wheels(enum direction dir, uint8_t duty) {
	if (duty < 0 || duty > 255) {
		return;
	}
	switch(dir) {
		case FORWARD:
			PORTD |= (1 << PD7);
			PORTC &= ~(1 << PC0);
			PORTC |= (1 << PC2);
			PORTC &= ~(1 << PC1);
			break;
		case BACKWARD:
			PORTD &= ~(1 << PD7);
			PORTC |= (1 << PC0);
			PORTC &= ~(1 << PC2);
			PORTC |= (1 << PC1);
			break;
		default:
			PORTD &= ~(1 << PD7);
			PORTC &= ~(1 << PC0);
			PORTC &= ~(1 << PC2);
			PORTC &= ~(1 << PC1);
	}

	OCR0A = duty;
}

/*
 * Set the speed and direction of each wheel. +ive is forward, -ve is backward
 */
void set_wheels(int fl, int fr, int bk){
	if (fl < 0) {
		fr_left(BACKWARD, -fl);
	} else {
		fr_left(FORWARD, fl);
	}

	if (fr < 0) {
		fr_right(BACKWARD, -fr);
	} else {
		fr_right(FORWARD, fr);
	}

	if (bk < 0) {
		bk_wheels(BACKWARD, -bk);
	} else {
		bk_wheels(FORWARD, bk);
	}
}

//**************************** SPI **************************************
// spi functions
void spi_init_slave (void) {
    DDRB=(1<<6);                                  //MISO as OUTPUT
    SPCR=(1<<SPE);                                //Enable SPI
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
int ctoi(char d)
{
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
	return input_int*sign;
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
	DDRC |= (1 << PC3);

	init_wheels();
	UART_setup();
    spi_init_slave();
    unsigned char data, msg[16];

    while(1) {

		sendMessage("Awaiting message\n");
        int index = 0;

		do {
			data = spi_tranceiver(ACK);
			msg[index++] = data;
		} while (data != '#');

        sendMessage(msg);
		sendChar('\n');

		int nums[3] = {0, 0, 0};
		parse_message(msg, nums);

		if (nums[0] == -255 && nums[1] == -255 && nums[2] == -255) {
			start_timer();
		} else if (nums[0] == 0 && nums[1] == 0 && nums[2] == 0) {
			stop_timer();
		} else {
			sendMessage("Unrecognized message received\n");
		}

		set_wheels(nums[0], nums[1], nums[2]);
    }
}
