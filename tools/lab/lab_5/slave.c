// ATMEGA328 as Slave

#ifndef F_CPU
#define F_CPU 1000000UL
#endif
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ACK 0x7E

void start_timer() {
	cli();
	TIMSK1 = (1 << TOIE1);
	TCCR1B |= ((0 << CS12) | (1 << CS11) | (0 << CS10));
	sei();
}

void stop_timer() {
	cli();
	TCCR1B &= ((0 << CS12) | (0 << CS11) | (0 << CS10));
	PORTC &= (0 << PC0); // make sure led is off when stopped
	sei();
}

// timer interrupt, if timer is running this will toggle the led when timer counter overflows
ISR(TIMER1_OVF_vect) {
	cli();
	PORTC ^= (1 << PC0);
	sei();
}

// uart functions 
void sendChar(char c) {
	// Wait until buffer is empty
	while (!(UCSR0A & (1 << UDRE0))) {}
    // Return the data from the RX buffer
	UDR0 = c;
}

void sendMessage(char msg[]) {
	for (int i = 0; msg[i]; i++) {
		if (msg[i] == '#') {
			break;
		}
		sendChar(msg[i]);
	}
}

void UART_setup() {
	/*Set baud rate */
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	// Enable receiver and transmitter
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    // Set frame: 8data, 1 stp
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

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

int ctoi(char d) {
	char str[2];

	str[0] = d;
	str[1] = '\0';
	return (int) strtol(str, NULL, 10);
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
	char num[4];

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
	DDRC |= (1 << PC0);

	UART_setup();
    spi_init_slave();                             //Initialize slave SPI
    unsigned char data, msg[17];

    while(1) {
		sendMessage("Awaiting message\n");
        int index = 0;

		do {
			data = spi_tranceiver(ACK);
			msg[index++] = data;

		} while (data != '#');

        sendMessage(msg);
		sendChar('\n');

		int nums[4] = {0, 0, 0, 0};
		parse_message(msg, nums);

		if (nums[0] == 255 && nums[1] == 255 && nums[2] == 255 && nums[3] == 255) {
			start_timer();
		} else if (nums[0] == 0 && nums[1] == 0 && nums[2] == 0 && nums[3] == 0) {
			stop_timer();
		} else {
			sendMessage("Unrecognized message received\n");
		}

    }
}
