#ifndef F_CPU
#define F_CPU 1000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void setup(){
	// Disable interrupts in case of another interrupt
	cli();
	// Set pin PB1 and PB2 as OUTPUT, will connect to LED
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);
	// Set pin PD2 (int0) as input
	DDRD &= ~(1 << PD2);
	// any logical change will trigger interrupt
    EICRA |= ((0 << ISC01) | (1 << ISC00));
    //ENABLE INT0
    EIMSK |= (1 << INT0);
	// turn on interrupts
    sei();
}

void loop(){
	// Toggle LED, Independednt of button presses
	while(1){
		PORTB ^= (1 << PB2);
		_delay_ms(500);
	}
}

void toggleLed() {
	// Disable interrupts in case of another interrupt
	cli();
	// Turn LED on, set PB1 to high
	PORTB ^= (1 << PB1);
	// Re-enable interrupts
	sei();
}

// Interrupt vector for when Button is pressed
ISR(INT0_vect)
{
	toggleLed();
}

int main(void){
	setup();
	loop();
	return 0;
}
