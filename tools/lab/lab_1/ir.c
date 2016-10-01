#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include "usb_serial.h"

void InitADC() {
    ADMUX = (1 << REFS0);
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

uint16_t ReadADC(uint8_t ch) {
    ch = ch&0b00000111;
    ADMUX |= ch;

    ADCSRA |= (1<<ADSC);
    while(!(ADCSRA & (1<<ADIF)));
    ADCSRA |= (1 << ADIF);
    return (ADC); 
}

void Wait() {
    uint8_t i;
    for(i=0; i < 20; i++)
        _delay_loop_2(0);
}

int main() {
    usb_serial_write("hello", 20);
    usb_init();
    uint16_t adc_result;
    while(1) {
        char buffer[10];
        adc_result = ReadADC(0);
       // usb_serial_write(itoa(adc_result, buffer, 10), 4);
        usb_serial_write("hello", 20);
        Wait();
    }

    return 0;
}
