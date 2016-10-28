#include <avr/io.h>
#include <avr/interrupt.h>

int externalTimer = 0;
int INT_1_EVERY = 1; // 1 second
int INT_2_EVERY = 2; // 2 seconds
int INT_TURN = 1;

ISR(INT0_vect) {
	// Disable interrupts
	cli();

	// Toggle timer
	if(INT_TURN == INT_1_EVERY) {
		INT_TURN = INT_2_EVERY;
 	} else {
 		INT_TURN = INT_1_EVERY;
 	}

    // Enable interrupts
    sei();
}

// Timer handler
ISR(TIMER0_COMPA_vect) {

	// Disable interrupts
	cli();

	if(externalTimer++ == INT_TURN * 10) {
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

    // Any logical change on INT0 generates an interrupt request
    EICRA |= (1 << ISC01);

    // Turn on INT0
    EIMSK |= (1 << INT0);

	// Enable global interrupts
	sei();
}

int main(void) {
    setup();
    while(1);
}
