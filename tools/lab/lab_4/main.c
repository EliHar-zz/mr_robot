#define FOSC 1000000                       // Clock Speed
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <avr/delay.h>
#include <stdlib.h>

enum direction {FORWARD, BACKWARD};

void init_wheels() {
	// In back right wheel, Set as output
	DDRC |= (1 << PC2);
	DDRC |= (1 << PC1);

	// In back left wheel, Set as output
	DDRD |= (1 << PD2);
	DDRB |= (1 << PB1);

	// *******************************************************
	// In front right wheel, Set as output
	DDRC |= (1 << PC0);
	DDRB |= (1 << PB2);

	// In front left wheel, Set as output
	DDRB |= (1 << PB4);
	DDRB |= (1 << PB5);

	// *******************************************************
	// Enabler for front right wheels, Set as output
	DDRD |= (1 << PD6);

	// Enabler for front left wheels, Set as output
	DDRD |= (1 << PD3);

	// Enabler for back right wheels, Set as output
	DDRB |= (1 << PB3);

	// Enabler for back left wheels, Set as output
	DDRD |= (1 << PD5);

	// Set Timer0, Timer2 in Fast PWM counting, with a prescaler 0
	// The OC0A, OC0B, OC2A, OC2B pin is cleared on match.
	// Non Inverted PWM output mode
	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
	TCCR0B = (1 << CS00);

	TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2A1) | (1 << COM2B1);
	TCCR2B = (1 << CS20);
}

/*
 * Moves front right wheel. Takes direction and duty cycle as argumet
 */
void fr_right(enum direction dir, uint8_t duty) {
	if (duty < 0 || duty > 255) {
		return;
	}
	switch(dir) {
	case FORWARD:
		PORTB |= (1 << PB4); // Set to High
		PORTB &= ~(1 << PB5);// Set to Low
		break;
	case BACKWARD:
		PORTB |= (1 << PB5); // Set to High
		PORTB &= ~(1 << PB4); // Set to Low
		break;
	default:
		PORTB &= ~(1 << PB5); // Set to Low
		PORTB &= ~(1 << PB4); // Set to Low
	}

	OCR0A = duty;
}

/*
 * Moves front left wheel. Takes direction and duty cycle as argumet
 */
void fr_left(enum direction dir, uint8_t duty) {
	if (duty < 0 || duty > 255) {
		return;
	}
	switch(dir) {
	case FORWARD:
		PORTB |= (1 << PB2);
		PORTC &= ~(1 << PC0);
		break;
	case BACKWARD:
		PORTC |= (1 << PC0);
		PORTB &= ~(1 << PB2);
		break;
	default:
		PORTB &= ~(1 << PB2);
		PORTC &= ~(1 << PC0);
	}

	OCR2B = duty;
}

/*
 * Moves back left wheel. Takes direction and duty cycle as argumet
 */
void bk_left(enum direction dir, uint8_t duty) {
	if (duty < 0 || duty > 255) {
		return;
	}
	switch(dir) {
	case FORWARD:
		PORTB &= ~(1 << PB1);
		PORTD |= (1 << PD2);
		break;
	case BACKWARD:
		PORTD &= ~(1 << PD2);
		PORTB |= (1 << PB1);
		break;
	default:
		PORTB &= ~(1 << PB1);
		PORTD &= ~(1 << PD2);
	}

	OCR0B = duty;
}

/*
 * Moves back right wheel. Takes direction and duty cycle as argumet
 */
void bk_right(enum direction dir, uint8_t duty) {
	if (duty < 0 || duty > 255) {
		return;
	}

	switch(dir) {
	case FORWARD:
		PORTC |= (1 << PC1);
		PORTC &= ~(1 << PC2);
		break;
	case BACKWARD:
		PORTC |= (1 << PC2);
		PORTC &= ~(1 << PC1);
		break;
	default:
		PORTC &= ~(1 << PC1);
		PORTC &= ~(1 << PC2);
	}

	OCR2A = duty;
}

/*
 * Set the speed and direction of each wheel. +ive is forward, -ve is backward
 */
void set_wheels(int fl, int fr, int bl, int br){
	if (fl < 0) {
		fr_left(BACKWARD, -fl);
	} else {
		fr_left(FORWARD, fl);
	}

	if (fr < 0) {
		fr_right(BACKWARD, -fr);
	} else {
		fr_right(FORWARD, fr);
	}

	if (bl < 0) {
		bk_left(BACKWARD, -bl);
	} else {
		bk_left(FORWARD, bl);
	}

	if (br < 0) {
		bk_right(BACKWARD, -br);
	} else {
		bk_right(FORWARD, br);
	}
}

int speed(int speedOption) {
	switch(speedOption) {
	case 1: return 255;
	break;
	case 2: return 200;
	break;
	case 3: return 150;
	break;
	}
	return 0;
}

/*
 * Set the speed Of all wheels based on a preset 1=HIGH, 2 = MEDIUM, 3=LOW, +ve is Forward, -ve is backward
 */
void set_speed(int speedOption){
	int duty = 0;
	if (speedOption < 0) {
		duty = -speed(-speedOption);
	} else {
		duty = speed(speedOption);
	}
	set_wheels(duty,duty,duty,duty);
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

		int duty = 0;
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
				int fl = 0, fr = 0, bl = 0, br = 0; // variables to hold the speed
				while ((token = strsep(&string, ",")) != NULL)
				{
					duty = parseInt(token);
					switch(count) {
					case 1: fl = duty;
					break;
					case 2: fr = duty;
					break;
					case 3: bl = duty;
					break;
					case 4: br = duty;
					break;
					}
					count++;

				}
				set_wheels(fl,fr,bl,br);
				free(tofree);
			}
		} else { // Choose petween speed presets (1, 2 or 3). Use "-" sign for backward rotation
			int speedOption = parseInt(message);
			set_speed(speedOption);
		}
	}
}

int main() {
	init_USART();
	init_wheels();
	loop();
	return 0;
}
