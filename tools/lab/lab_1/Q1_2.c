#ifndef F_CPU
#define F_CPU 16000000
#endif
#include <avr/io.h>
#include <util/delay.h>

void setup() {
	//Set data direction on Pin 7 to OUTPUT
	DDRB |= (1 << PB7);
	//Set Timer0 in Fast PWM counting from 0x0 to 0xFF with a prescaler 64
	//The OC0A pin is cleared on match.
	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
	TCCR0B = (1 << CS00) | (1 << CS01);
	OCR0A = 0;
}
void loop() {
	while(1) {
		/* The timer goes from 0 to 255, so we toggle between the following Duty Cycles
		 * by setting OCR0A which will clear the OC0A in on match.
		 * This will make the LED go through 4 different levels of 
		 * brightness as the OC0A matches 0, 64, 128, 255
		 */
		OCR0A = 0; 		// 0%
		_delay_ms(200);
		OCR0A = 64; 	// 25%
		_delay_ms(200);
		OCR0A = 128; 	// 50%
		_delay_ms(200);
		OCR0A = 255; 	// 100%
		_delay_ms(200);
	}
}
int main() {
	setup();
	loop();
	return 0;
}
