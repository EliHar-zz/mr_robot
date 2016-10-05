#ifndef F_CPU
#define F_CPU 16000000
#endif
#include <avr/interrupt.h>
#include <avr/io.h>
#include "usb_serial.h"

ISR(TIMER0_COMPA_vect) // Interrupt Service Routine
{
	PORTB ^= (1 << PB7); // Use xor to toggle the LED
	usb_serial_write("Hello Teensy\r\n",14);
}
void setupt()
{
	cli(); // Disable global interrupts
	TCCR0B |= ((1 << CS02) | (1 << CS00)); // Timer 0 prescaling 1024
	TCCR0A |= (1 << WGM01); // Put timer 0 in CTC mode
	OCR0A = 255; // Count 255 cycles for interrupt
	TIMSK0 |= (1 << OCIE0A); // enable timer compare interrupt

	DDRB |= (1 << PB7); // Set PortB Pin7 as an output
	PORTB |= (1 << PB7); // Set PortB Pin7 high to turn on LED
	usb_init();
	sei(); // Enable global interrupts
}
int main()
{
	setup();
	while(1) { } // Don't do anything
}


