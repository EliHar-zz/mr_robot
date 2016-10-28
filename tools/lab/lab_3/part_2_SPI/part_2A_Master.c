// Part2A: SPI Communication
// Implement the Master/Slave configuration using SPI.

// **************** MASTER **************

#define FOSC 16000000 // Clock Speed
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile char data;

void SPI_MasterInit(void) {
	// Set SS, MOSI, and SCK as output
	DDRB |= (1 << DDB2)|(1 << DDB3)|(1 << DDB5);
	// Enable SPI, Master, set clock rate fck/16
	SPCR |= (1 << SPE)|(1 << MSTR)|(1 << SPR0);
}

char SPI_MasterTransmit (char cData) {
	// Start transmission
	SPDR = cData;
	// Wait for transmission compete
	while (!(SPSR & (1 << SPIF))){}
	return SPDR;
}

char SPI_MasterReceive (void) {
	return SPI_MasterTransmit(0x00);
}

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

void USART_Init() {
    /*Set baud rate */
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	// Enable Receiver
    UCSR0B |= (1 << TXEN0);
    // Set frame: 8data, 1 stp
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void setup() {
	// Setup connection to Serial monitor
	USART_Init();
    // Setup Master
    SPI_MasterInit();
}

void loop() {
	char received_byte;
	while(1) {
		received_byte = SPI_MasterReceive();
		sendChar(received_byte);
	}
}

int main( void ) {
	setup();
	loop();
}
