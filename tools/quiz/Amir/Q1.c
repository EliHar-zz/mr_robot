#include <avr/io.h>
#include <avr/interrupt.h>

int externalTimer1 = 0;
int externalTimer2 = 0;
int INT_EVERY = 5; // 5 seconds

// Timer handler
ISR(TIMER0_COMPA_vect) {

	// Disable interrupts
	cli();

	if(externalTimer1++ == INT_EVERY * 10) {
		PORTB ^= (1 << PB1);
		externalTimer1 = 0;
	}

    // Enable interrupts
    sei();
}

ISR(TIMER1_COMPA_vect) {

	// Disable interrupts
	cli();

	if(externalTimer2++ == INT_EVERY * 10) {
		PORTB ^= (1 << PB2);
		externalTimer2 = 0;
	}

    // Enable interrupts
    sei();
}

void setup() {

	// First LED
    DDRB |= (1 << PB1); // LED on pin PB1
    TCCR0A = (1 << WGM01); // CTC mode
    TIMSK0 = (1 << OCIE0A);

    // Value calculated from http://eleccelerator.com/avr-timer-calculator/
    OCR0A = 97.65625;
    TCCR0B = (1 << CS00) | (1 << CS02); // 1024 prescalar

    // Second LED
    DDRB |= (1 << PB2); // LED on pin PB1
    TCCR1A = (1 << WGM01); // CTC mode
    TIMSK1 = (1 << OCIE1A);

    // Value calculated from http://eleccelerator.com/avr-timer-calculator/
    OCR1A = 97.65625;
    TCCR1B = (1 << WGM01) | (1 << WGM00); // 1024 prescalar

	// Enable global interrupts
	sei();
}

int main(void) {
    setup();
    while(1);
}
