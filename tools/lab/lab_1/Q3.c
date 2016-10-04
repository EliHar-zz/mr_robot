#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usb_serial.h"

ISR(TIMER0_OVF_vect) {
	unsigned char greeting[] = "Hello Teensy\r\n";
	usb_serial_write(greeting, 14);
}

void setup() {
	DDRB = (1 << PB7);

	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
	TCCR0B = (1 << CS00) | (1 << CS01);

	TIMSK0 |= (1 << TOIE0);

	usb_init();
	sei();
	OCR0A = 0;
}

void loop() {
	while(1) {
		_delay_ms(50);
		PORTB |= (1 << PB7);
		OCR0A += 15;
		PORTB &= ~(1 << PB7);
	}
}

int main() {

	setup();
	loop();
	return 0;
}
