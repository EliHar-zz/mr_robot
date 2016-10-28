// Part2A: SPI Communication
// Implement the Master/Slave configuration using SPI.

// **************** SLAVE **************

#define FOSC 16000000                       // Clock Speed
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

void SPI_SlaveInit(void) {
	// Set SS, MOSI and SCK as input
	DDRB &= ~((1 << PB2) | (1 << PB3) | (1 << PB5));
	// MISO as output
	DDRB |= (1 << DDB4);
	// Set as Slave
	SPCR &= ~(1 << MSTR);
	// Enable SPI, set clock rate to fck/16
	SPCR |= (1 << SPE);
}

char SPI_SlaveTransmit (char cData) {
	// Start transmission
	SPDR = cData;
	// Wait for transmission compete
	while (!(SPSR & (1 << SPIF))){}
	return SPDR;
}

void SPI_SlaveTransmit_Message(char str[]){
	for (int i = 0; str[i]; i++){
		SPI_SlaveTransmit(str[i]);
	}
}

void USART_Init() {
    /*Set baud rate */
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	// Enable transmitter
    UCSR0B |= (1 << RXEN0);
    // Set frame: 8data, 1 stp
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void ADC_Init() {
	// initialize adc
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// get value of adc
uint16_t read_adc(uint8_t ch) {
	ch = ch&0b00000111;
	ADMUX |= ch;

	// Start single conversion
	ADCSRA |= (1 << ADSC);

	// Wait for conversion to complete
	while(!(ADCSRA & (1 << ADIF)));

	// Clear ADIF by writing one to it
	// Note you may be wondering why we have write one to clear it
	// This is standard way of clearing bits in io as said in datasheets.
	// The code writes '1' but it result in setting bit to '0' !!!
	ADCSRA |= (1 << ADIF);

	return (ADC);
}

//Converts integer to String (char*)
char* itoa(int i, char b[]){
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

void setup() {
	// Setup connection to Serial monitor
	USART_Init();
    // Setup Master
    SPI_SlaveInit();
    // Init ADC
    ADC_Init();
}

void loop() {
	uint16_t adc_result;
	while(1) {
		adc_result = read_adc(0);

		// convert value of ADC
		char adc_result_str[] = "";
		itoa(adc_result, adc_result_str);
		strcat(adc_result_str, "\r\n");

		// Send IR reading to Master
		SPI_SlaveTransmit_Message(adc_result_str);
	}
}

int main( void ) {
	setup();
	loop();
}
