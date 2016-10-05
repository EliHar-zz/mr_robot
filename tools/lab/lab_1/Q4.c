/*
 * Questions 4: Output the value of the LED brightness to the serial monitor
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

/*
 * Setup step:
 *	- Configure board pins
 *	- Configure Serial monitor
 */
void setup() {
	DDRB = (1 << PB7); // Set pin PB7 as OUTPUT

	// Fast PWM
	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
	
	// Clock/64 (From prescaler)
	TCCR0B = (1 << CS00) | (1 << CS01);

	// Reset OCR0A
	OCR0A = 0;

	// Initialize USB Serial
	usb_init();
	sei();
}

/*
 * Loop step:
 * 	- Keep program running
 *	- Update OCR0A every X ms
 */
void loop() {
	while(1) {

		// Delay the program
		_delay_ms(100);

		// Print the brightness to the Serial Monitor
		char brightness[50] = "";
        char integer[20];
		strcat(brightness, "LED brightness is: ");
        strcat(brightness, itoa(OCR0A, integer));
		strcat(brightness, "\n\r");
		usb_serial_write(brightness, 30);

		// Update OCR0A
		OCR0A += 20;
	}
}

/*
 * Start program
 */
int main() {
	setup();
	loop();
	return 0;
}
