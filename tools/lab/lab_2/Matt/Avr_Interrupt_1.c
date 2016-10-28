// Interrupt is called when switch is pressed

#define F_CPU 555555UL

#include <avr/io.h>
#include <avr/interrupt.h>

void initialize_INT0() {
    /*  External Interrupt Control Register A
        Can change 4 bits ISC11, ISC10, ISC01, ISC00

        ISC11 and ISC10 control Interrupt 1 sense control

        Interrupt 0 sense control
        ISC01   ISC00
        0       0       The low level of INT0 generates an interrupt
        0       1       Any logical change to INT0 generates an interrupt
        1       0       The falling edge of INT0 generates an interrupt
        1       1       The rising edge of INT0 generates an interrupt
    */
    EICRA |= ((0 << ISC01) | (1 << ISC00)); // any logical change will trigger interrupt

    // turn on INT0 from external interrupt mask register
    EIMSK |= (1 << INT0);

    sei(); // enable global interrupts:
}

// Interrupt Service Routine (interrupts when INT0 has a change in input)
ISR(INT0_vect) {
    cli(); // disable interrupts

    PORTB ^= (1 << PB1); // toggle LED

    sei(); // re-enable interrupts
}

void setup() {
    DDRB |= (1 << PB1); // set PB1 as output pin for LED
    initialize_INT0();
}

int main(void) {
    setup();

    // loop()
    for (;;) {
        // can run code here that won't need to worry about delays
    }
}
