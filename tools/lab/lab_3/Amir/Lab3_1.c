#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <stdint.h>

char receiveChar() {
	// Wait until the RXC0 flag in UCSR0A is set to 1
	while ((UCSR0A & (1 << RXC0)) == 0) {}
    // Get character that caused the flag to be set to 1
    return UDR0;
}

void sendMessage(char msg[]) {
	for (int i = 0; msg[i]; i++) {
		// Wait until buffer is empty
		while ((UCSR0A & (1 << UDRE0)) == 0) {}
		// Return the data from the RX buffer
		UDR0 = msg[i];
	}
}

void setup() {
    // Set baud rate
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	// Enable receiver and transmitter
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

void loop() {
	char num;
	while(1) {
		num = receiveChar();
		switch(num) {
		case '1':
			sendMessage("Hello\n");
			break;
		case '2':
			sendMessage("Hi\n");
			break;
		case '3':
			sendMessage("Hola\n");
			break;
		default:
			sendMessage("Press 1, 2 or 3 :)\n");
		}
	}
}

int main( void ) {
	setup();
	loop();
}
