#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

void setup() {
    DDRB = (1 << PB7);

    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
    TCCR0B = (1 << CS00) | (1 << CS01);

    OCR0A = 0;
}

void loop() {
    int count = 0;

    while(1) {

        _delay_ms(100);
        
        PORTB |= (1 << PB7);

        OCR0A = 128;

        PORTB &= ~(1 << PB7);
    }
}

int main() {

    setup();
    loop();

    return 0;
}
