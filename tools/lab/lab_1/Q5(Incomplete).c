/*
 * Questions 5: Blink LED without the use of interrupt
 */

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include "usb_serial.h"

/*
 * Converts integer to String (char*)
 */
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

	// Fast PWM
	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
	
	// Clock/64 (From prescaler)
	TCCR0B = (1 << CS00) | (1 << CS01);

	// Reset OCR0A
	OCR0A = 255;

	// Initialize USB Serial
	usb_init();
	sei();
}

void loop() {
    int counter = 0;
	while(1) {
        if(counter++ == 30000) {
            DDRB ^= (1 << PB7); // Toggle pin mode every 30000 iterations
            counter = 0;
        }
    }
}

int main() {
	setup();
	loop();
	return 0;
}
