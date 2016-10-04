/*
 * Questions 4: Blink LED without the use of interrupt
 */

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <util/delay.h>

void setup() {
	DDRB = (1 << PB7); // Set pin PB7 as OUTPUT

	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
	TCCR0B = (1 << CS00) | (1 << CS01);

	OCR0A = 0;
}

void loop() {
	int counter = 0;
	while(1) {
		if (OCR0A == 1000) {
			PORTB |= (1 << PB7);
		} else {
			PORTB &= ~(1 << PB7);
		}
		_delay_ms(50);
		OCR0A += 1;
	}
}

int main() {
	setup();
	loop();
	return 0;
}
