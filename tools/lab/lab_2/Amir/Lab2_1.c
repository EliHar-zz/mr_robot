#include <avr/io.h>
#include <avr/interrupt.h>

// Set LED status
int isOn = 1;

// INT0 handler
ISR(INT0_vect) {

    // Disable interrupts
    cli();

    // Toggle LED status
    isOn = 1 - isOn;

    // Eliminate bouncing as much as possible
    if(bit_is_clear(PIND, 2)) {

    	// If LED is ON
    	if(isOn == 1) {
    		PORTB |= 1 << PB1;
    	} else {
    		PORTB &= ~(1 << PB1);
    	}
    }

    // Enable interrupts
    sei();
}

void setup() {

    // LED on pin PB1
    DDRB |= (1 << PB1);

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
