#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

enum Port {
    DA,DB,DC,DD,DE,DF,
    PA,PB,PC,PD,PE,PF
};

void output(enum Port port, int pbValue) {
    switch(port) {
         case DA: DDRA |= (1 << pbValue); break;
         case DB: DDRB |= (1 << pbValue); break;
         case DC: DDRC |= (1 << pbValue); break;
         case DD: DDRD |= (1 << pbValue); break;
         case DE: DDRE |= (1 << pbValue); break;
         case DF: DDRF |= (1 << pbValue); break;
    }   
}

void input(enum Port port, int pbValue) {
    switch(port) {
        case DA: DDRA &= (0 << pbValue); break;
        case DB: DDRB &= (0 << pbValue); break;
        case DC: DDRC &= (0 << pbValue); break;
        case DD: DDRD &= (0 << pbValue); break;
        case DE: DDRE &= (0 << pbValue); break;
        case DF: DDRF &= (0 << pbValue); break;
   }   
}

void on(enum Port port, int pbValue) {
    switch(port) {
        case PA: PORTA |= (1 << pbValue); break;
        case PB: PORTB |= (1 << pbValue); break;
        case PC: PORTC |= (1 << pbValue); break;
        case PD: PORTD |= (1 << pbValue); break;
        case PE: PORTE |= (1 << pbValue); break;
        case PF: PORTF |= (1 << pbValue); break;
   }   
}

void off(enum Port port, int pbValue) {
    switch(port) {
        case PA: PORTA &= (0 << pbValue); break;
        case PB: PORTB &= (0 << pbValue); break;
        case PC: PORTC &= (0 << pbValue); break;
        case PD: PORTD &= (0 << pbValue); break;
        case PE: PORTE &= (0 << pbValue); break;
        case PF: PORTF &= (0 << pbValue); break;
   }
}

void offAll() {
    int i;
    for(i=0; i <= 6; i++) {
        off(i, i);
    }
}

int main(void) {
    output(DB, 6);
    for(;;) {
        on(PB, 6);
        _delay_ms(500);
        off(PB, 6);
        _delay_ms(500);
    }
}
