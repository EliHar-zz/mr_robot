#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

// Interrupt Service Routine (interrupts when timer counter overflows)
ISR(TIMER0_OVF_vect) {
    cli(); // disable interrupts
    PORTB ^= (1 << PB1); // toggle led
    sei(); // re-enable interrupts
}

ISR(TIMER2_OVF_vect) {
    cli(); // disable interrupts
    PORTB ^= (1 << PB2); // toggle led
    sei(); // re-enable interrupts
}

void setup() {
	cli(); // disable global interrupts

	DDRB |= (1 << PB1); // set PB1 as output pin for LED
	TIMSK0 = (1 << TOIE0); // enable Timer1 overflow interrupt
	TCCR0B = (1 << CS00) | (1 << CS02); // 1024 prescalar
	OCR0A = 3;

	DDRB |= (1 << PB2); // set PB2 as output pin for LED
	TIMSK2 = (1 << TOIE2); // enable Timer1 overflow interrupt
	TCCR2B = (1 << CS00) | (1 << CS02); // 1024 prescalar
	OCR2A = 3;

	sei(); // enable global interrupts:
}

int main(void) {
    setup();

    // loop()
    while(1);
}
