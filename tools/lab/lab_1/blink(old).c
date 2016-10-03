/*
 * Program that blinks an LED
 */
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

void blink() {
	PORTB |= (1 << PB4);
	_delay_ms(500);
	PORTB &= (0 << PB4);
	_delay_ms(500);
}

int main(void) {
	DDRB |= (1 << PB4);
	while(1) {
		blink();
	}
	return 0;
}
