/*
 * Questions 3: Output the value of the LED brightness to the serial monitor
 */

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "usb_serial.h"

char* itoa(int i, char b[]){
	char const digit[] = "0123456789";
	char* p = b;
	if(i<0){
		*p++ = '-';
		i *= -1;
	}
	int shifter = i;
	do{ //Move to where representation ends
		++p;
		shifter = shifter/10;
	}while(shifter);
	*p = '\0';
	do{ //Move back, inserting digits as u go
		*--p = digit[i%10];
		i = i/10;
	}while(i);
	return b;
}

void setup() {
	DDRB = (1 << PB7); // Set pin PB7 as OUTPUT

	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
	TCCR0B = (1 << CS00) | (1 << CS01);

	OCR0A = 0;
	usb_init();
	sei();
}

void loop() {
	while(1) {
		_delay_ms(100);
		PORTB |= (1 << PB7);
		char message[32] = "LED brightness is: ";
		char brightness[] = "";
		itoa(OCR0A, brightness);
		strcat(message, brightness);
		strcat(message, "\r\n");
		usb_serial_write(message, 32);
		OCR0A += 64;
		PORTB &= ~(1 << PB7);
	}
}

int main() {
	setup();
	loop();
	return 0;
}
