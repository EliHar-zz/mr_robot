// Our INT0 interrupt will start/stop the clock from running
// If the clock is on, our LED connected to PB1 will blink every 8 block cycles

#define F_CPU 555555UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

bool clock_on = true;

// this will set up an asynchronous interrupt such that
// if there is a change in input to the INT0 pin, it will call the ISR(INT0_vect) function
void initialize_int0() {
    cli();

    EICRA |= ((0 << ISC01) | (1 << ISC00));
    EIMSK |= (1 << INT0);

    sei();
}

// starts our clock
void start_clock() {
    cli();

    TIMSK1 = (1 << TOIE1);
    TCCR1B |= ((0 << CS12) | (1 << CS11) | (0 << CS10));

    clock_on = true;

    sei();
}

// stops our clock from running and resets it to 0
void stop_clock() {
    cli();

    TCCR1B &= ((0 << CS12) | (0 << CS11) | (0 << CS10));

    clock_on = false;

    sei();
}

// this interrupt will be called if INT0's input value changes
ISR(INT0_vect) {
    cli();

    if (clock_on) {
        stop_clock();
    } else {
        start_clock();
    }

    sei();
}

// if clock is running, this will be called when the clock overflows
ISR(TIMER1_OVF_vect) {
    cli();
    PORTB ^= (1 << PB1); // toggle LED
    sei();
}

void setup() {
    DDRB |= (1 << PB1); // set PB1 as output pin
}

int main(void) {
    DDRB |= (1 << PB1); // set PB1 as our output pin

    initialize_int0();

    for(;;) {

    }
}
