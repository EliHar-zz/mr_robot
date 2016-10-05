#define F_CPU 16000000

#include <avr/io.h>

int main(void) {

	// This is housekeeping required to use the IR sensor
	// Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
	// Set ADC reference to AVCC	
	ADMUX |= (1 << REFS0); 
	// Left adjust ADC result to allow easy 8 bit reading
	ADMUX |= (1 << ADLAR);

	// Set pin F0 to read input from sensor
	// This is implicit (not required since default value is 0)
	// DDRF &= (0 << PF0);

	// Set pin F1 as an output pin
	DDRF |= (1 << PF1);

	// loop forever
	for(;;) {
			
		if(PINF & _BV(PF0)) {		// evaluates to true if pin F0 is receiving input
			PORTF |= (1 << PF1);	// turn on LED
		} else {					// else if no input is received
			PORTF &= (0 << PF1);	// turn off LED
		}
	}
}
