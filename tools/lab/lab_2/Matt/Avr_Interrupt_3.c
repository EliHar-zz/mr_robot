// Our INT0 interrupt will start or stop the timer
// If the timer is on, our LED connected to PB1 will blink when timer counter overflows

#define F_CPU 555555UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

bool clock_on = true;

// This will set up an asynchronous interrupt such that
// if there is a change of input to the INT0 pin, it will call the ISR(INT0_vect) function
void initialize_INT0() {
    cli();

    EICRA |= ((0 << ISC01) | (1 << ISC00));
    EIMSK |= (1 << INT0);

    sei();
}

// Starts our timer
void start_timer() {
    cli();

    TIMSK1 = (1 << TOIE1);
    TCCR1B |= ((0 << CS12) | (1 << CS11) | (0 << CS10)); // counter increments every 8 clock cycles

    clock_on = true;

    sei();
}

// Stops our timer from running and resets counter to 0
void stop_timer() {
    cli();

    TCCR1B &= ((0 << CS12) | (0 << CS11) | (0 << CS10));

    clock_on = false;

    sei();
}

// This interrupt will be called if INT0's input value changes
ISR(INT0_vect) {
    cli();

    if (clock_on) {
        stop_timer();
    } else {
        start_timer();
    }

    sei();
}

// If timer is running, this will be called when the timer counter overflows
ISR(TIMER1_OVF_vect) {
    cli();
    PORTB ^= (1 << PB1); // toggle LED
    sei();
}

void setup() {
    DDRB |= (1 << PB1); // set PB1 as output pin for LED
    initialize_INT0(); // turn on INT0 interrupt
}

int main(void) {
    setup();

    for(;;) {

    }
}
