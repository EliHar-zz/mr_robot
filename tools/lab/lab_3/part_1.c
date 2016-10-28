// Part1: USB/UART Communication
// This code waits for a character and transmits the character back (No interrupts)

#define FOSC 16000000                       // Clock Speed
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <stdint.h>                     // needed for uint8_t

char receiveChar() {
	// Wait until data is received
	while (!(UCSR0A & (1 << RXC0))) {}
    // Return the data from the RX buffer
    return UDR0;
}

void sendChar(char c) {
	// Wait until buffer is empty
	while (!(UCSR0A & (1 << UDRE0))) {}
    // Return the data from the RX buffer
	UDR0 = c;
}

void sendMessage(char msg[]) {
	for (int i = 0; msg[i]; i++) {
		sendChar(msg[i]);
	}
}

void setup() {
    /*Set baud rate */
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	// Enable receiver and transmitter
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    // Set frame: 8data, 1 stp
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void loop() {
	char num;
	while(1) {
		num = receiveChar();
		switch(num) {
		case '1':
			sendMessage("Good morning, Yo!\r\n");
			break;
		case '2':
			sendMessage("Hey! What\'s up!\r\n");
			break;
		case '3':
			sendMessage("Bonjour!\r\n");
			break;
		default:
			sendMessage("*** Invalid input! ***\r\n\nType a number between 1 and 3:\r\n");
		}
	}
}

int main( void ) {
	setup();
	loop();
}
