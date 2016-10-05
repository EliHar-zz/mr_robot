#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "usb_serial.h"

// get value of adc
uint16_t read_adc(uint8_t ch) {
	ch = ch&0b00000111;
	ADMUX |= ch;

	// Start single conversion
	ADCSRA |= (1 << ADSC);

	// Wait for conversion to complete
	while(!(ADCSRA & (1 << ADIF)));

	// Clear ADIF by writing one to it
	// Note you may be wondering why we have write one to clear it
	// This is standard way of clearing bits in io as said in datasheets.
	// The code writes '1' but it result in setting bit to '0' !!!
	ADCSRA |= (1 << ADIF);

	return (ADC);
}

void setup() {
	// initialize adc
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	// initialize usb and call sei() for serial writes
	usb_init();
	sei();

	// set pin F1 as output pin for led
	DDRF |= (1 << PF1);
}

void loop() {
	uint16_t adc_result;

	while(1) {
		adc_result = read_adc(0);

		// convert value of adc and write it to the serial monitor
		char adc_result_str[] = "";
		itoa(adc_result, adc_result_str, 10);
		strcat(adc_result_str, "\r\n");
		usb_serial_write(adc_result_str, 6);

		// We noticed that the result of adc acted like a parabolic function. If it received 
		// no input the value was around 60. If we started moving our hands towards the IR 
		// sensor from far away, the value being printed was increasing. It seemed to reach
 		// its max value at 15 cm away from the sensor. If we moved our hand closer than 
		// 15 cm the value began to decrease. The maximum value we received was slightly over
		// 530. We noticed that only turning the light on at values higher than 515 seemed to
		// give us reasonably accurate results.

		// if sensor detects something approximately 15 cm away, turn on led
		if (adc_result > 515) {
			PORTF |= (1 << PF1);
		} else {
			PORTF &= (0 << PF1);
		}
	}
}

int main(void){
	setup();
	loop();
}

