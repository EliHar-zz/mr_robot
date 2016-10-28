#include <avr/io.h>
#include <avr/interrupt.h>

int externalTimer = 0;
int INT_EVERY = 5; // 5 seconds

// Timer handler
ISR(TIMER0_COMPA_vect) {

	// Disable interrupts
	cli();

	if(externalTimer++ == INT_EVERY * 10) {
		PORTB ^= (1 << PB1);
		externalTimer = 0;
	}

    // Enable interrupts
    sei();
}

void setup() {
    DDRB |= (1 << PB1); // LED on pin PB1
    TCCR0A = (1 << WGM01); // CTC mode
    TIMSK0 = (1 << OCIE0A);

    // Value calculated from http://eleccelerator.com/avr-timer-calculator/
    OCR0A = 97.65625;
    TCCR0B = (1 << CS00) | (1 << CS02); // 1024 prescalar

	// Enable global interrupts
	sei();
}

int main(void) {
    setup();
    while(1);
}
