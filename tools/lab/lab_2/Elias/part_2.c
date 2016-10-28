#ifndef F_CPU
#define F_CPU 1000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

void setup(){
	// Disable interrupts in case of another interrupt
	cli();
	// Set pin PB1 as OUTPUT, will connect to LED
	DDRB |= (1 << PB1);
	// enable Timer1 overflow interrupt
	TIMSK1 = (1 << TOIE1);
	// Clock prescaler 1/64
	TCCR1B |= ((0 << CS12) | (1 << CS11) | (1 << CS10));
	// turn on interrupts
    sei();
}

void loop(){
	while(1){
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

// Interrupt vector for when timer overflows
ISR(TIMER1_OVF_vect)
{
	toggleLed();
}

int main(void){
	setup();
	loop();
	return 0;
}
