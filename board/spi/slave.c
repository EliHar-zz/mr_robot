// ATMEGA328 as Slave

#ifndef F_CPU
#define F_CPU 1000000UL
#endif
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ACK 0x7E

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

void UART_setup() {
	sendMessage("In setup\n");

	/*Set baud rate */
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	// Enable receiver and transmitter
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    // Set frame: 8data, 1 stp
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void spi_init_slave (void)
{
    DDRB=(1<<6);                                  //MISO as OUTPUT
    SPCR=(1<<SPE);                                //Enable SPI
}

//Function to send and receive data
unsigned char spi_tranceiver (unsigned char data)
{
    SPDR = data;                                  //Load data into buffer
    while(!(SPSR & (1<<SPIF) ));                  //Wait until transmission complete
    return(SPDR);                                 //Return received data
}

int main(void)
{
	UART_setup();
    spi_init_slave();                             //Initialize slave SPI
    unsigned char data, buffer[10];
	sendMessage("Awaiting message\n");
    while(1)
    {
        data = spi_tranceiver(ACK);               //Receive data, send ACK
        itoa(data, buffer, 10);                   //Convert integer into string

        sendMessage(buffer);
		sendChar('\n');
    }
}
