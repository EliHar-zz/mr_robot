#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
//#include "usb_serial/usb_serial.h"

void blink(int delay_time) {
    PORTB |= (1 << PB4);
    _delay_ms(delay_time);
    PORTB &= (0 << PB4);
    _delay_ms(delay_time);
}

int main(void) {
    DDRB |= (1 << PB4);
    for(;;) {
        for (int i = 0; i < 3; i++) {  
            blink(300);
        }
        blink(1000);
    }
}
