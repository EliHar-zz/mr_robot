#define FOSC 1000000
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <avr/interrupt.h>

// Global
int externalTimer_1 = 0;
int externalTimer_2 = 0;
int externalTimer_3 = 0;
uint16_t adc_result;

// Specific
int delay_1 = 1; // First LED Speed
int delay_2 = 1; // Second LED Speed
int delay_ir = 1000;
int intensity_1 = 0; // First LED brightness
int intensity_2 = 0; // Second LED brightness

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

	return ADC;
}

void toggleLED_1() {
	if(OCR0A == 0) {
		OCR0A = intensity_1;
	} else {
		OCR0A = 0;
	}
}

void toggleLED_2() {
	if(OCR0B == 0) {
		OCR0B = intensity_2;
	} else {
		OCR0B = 0;
	}
}

char* itoa_2(int i, char b[]){
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

void send_IR() {
	char adc_result_str[32]= {};
	adc_result = read_adc(0);
	itoa_2(adc_result, adc_result_str);
	sendMessage(adc_result_str);
	sendChar('\n');
}

// Timer handler
ISR(TIMER2_COMPA_vect) {
	cli();
	if(externalTimer_3++ >= delay_ir) {
		send_IR();
		externalTimer_3 = 0;
	}

	if(externalTimer_1++ >= delay_1) {
		toggleLED_1();
		externalTimer_1 = 0;
	}

	if(externalTimer_2++ >= delay_2) {
		toggleLED_2();
		externalTimer_2 = 0;
	}
	sei();
}

void init_timer0() {
	// Enabler for front right wheels, Set as output
	DDRD |= (1 << PD6);
	DDRD |= (1 << PD5);

	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
	TCCR0B = (1 << CS00);
}

void init_timer2() {
	TCCR2A = (1 << WGM21); // CTC mode
	TIMSK2 = (1 << OCIE2A);

	// Value calculated from http://eleccelerator.com/avr-timer-calculator/
	OCR2A = 0.9765625;
	TCCR2B = (1 << CS20) | (1 << CS21) | (1 << CS22); // 1024 prescalar

	// Enable global interrupts
	sei();
}

void updateLEDs(int d1, int i1, int d2, int i2) {
	delay_1 = d1;
	delay_2 = d2;
	intensity_1 = i1;
	intensity_2 = i2;
}

void init_Timers() {
	cli();
	init_timer0();
	init_timer2();
	sei();
}


void ADC_Init() {
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}



//***************************  USART  *****************************

void init_USART() {
	/*Set baud rate */
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	// Enable receiver and transmitter
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	// Set frame: 8data, 1 stp
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

/*
 * Receives a character from serial monitor
 */
char receiveChar() {
	// Wait until data is received
	while (!(UCSR0A & (1 << RXC0))) {}
	// Return the data from the RX buffer
	return UDR0;
}

/*
 * Receives a message from serial monitor
 */
void receiveMessage(char* message) {
	int index = 0;
	char c = receiveChar();
	while (isdigit(c) || c == ',' || c == '-') { // Characters allowed to be received
		message[index++] = c;
		c = receiveChar();
	}
	message[index] = '\0';
}

/*
 * sends char to serial monitor
 */
void sendChar(char c) {
	// Wait until buffer is empty
	while (!(UCSR0A & (1 << UDRE0))) {}
	// Return the data from the RX buffer
	UDR0 = c;
}

/*
 * Send message to serial monitor
 */
void sendMessage(char msg[]) {
	for (int i = 0; msg[i]; i++) {
		sendChar(msg[i]);
	}
}

/*
 * Char to int
 */
int ctoi(char d)
{
	char str[2];

	str[0] = d;
	str[1] = '\0';
	return (int) strtol(str, NULL, 10);
}

/*
 * Parses a string of integers
 */
int parseInt(char* integer_str) {
	int sign = 1;
	int length = strlen(integer_str);
	int i = 0;
	if (integer_str[0] == '-') {
		sign = -1;
		i = 1;
	}
	int input_int = 0;
	if (length >= 3) {
		input_int = 1;
	}
	for (i; i < length; i++) {
		input_int += ctoi(integer_str[i]) * pow(10, length - 1 - i);
	}
	return input_int*sign;
}

void loop() {
	while(1) {
		int input = 0;
		int length = 128;
		char message[length + 1];
		receiveMessage(message);
		if (strlen(message) > 2) {
			char* token;
			char* string;
			char* tofree;
			string = strdup(message);
			if (string != NULL) {
				int count = 1;
				tofree = string;
				int d1 = 0, i1 = 0, d2 = 0, i2 = 0; // variables to hold the speed
				while ((token = strsep(&string, ",")) != NULL)
				{
					input = parseInt(token);
					switch(count) {
					case 1: d1 = input;
					break;
					case 2: i1 = input;
					break;
					case 3: d2 = input;
					break;
					case 4: i2 = input;
					break;
					}
					count++;
				}

				if(count == 2) {
					delay_ir = input;
				} else {
					updateLEDs(d1,i1,d2,i2);
				}
				free(tofree);
			}
		}
	}
}

int main() {
	init_USART();
	ADC_Init();
	init_Timers();
	loop();
	return 0;
}
