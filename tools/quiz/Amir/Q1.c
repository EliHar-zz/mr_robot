#define FOSC 1000000
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <avr/interrupt.h>

// In the following program, variables ending with the number 1
// correspond to the first LED, and variables ending with the
// number 2 correspond to the second LED

// Timer variables are used to count to a specific unit of time.
// For example, if g_timer_1 is equal to 1, then the 1 msec has
// passed. This is useful because we can have accurate timing
// for blinking the LEDs.
int g_timer_1 = 0;
int g_timer_2 = 0;

// The amount of time specified by the user for blinking an LED.
// For example, if g_delay_1 is equal to 1, then the first LED is
// going to blink every 1 second
int g_delay_1 = 1; 
int g_delay_2 = 1;

// The brightness of an LED is also specified by the user.
// The value for the brightness variables goes from 0 to 255,
// where 0 is LOW brightness and 255 is HIGH brightness.
int g_brightness_1 = 0;
int g_brightness_2 = 0;

// Timer 2 handler executes every 1 msec.
// Each time the handler executes, the timer checks
// if the first and/or second LED should toggle.
ISR(TIMER2_COMPA_vect) {

    // Disable interrupts
	cli();

    // Check if it's time to toggle LED 1
	if(g_timer_1++ >= g_delay_1) {

        // Toggle LED 1
        if(OCR0A == 0) {
            OCR0A = g_brightness_1;
        } else {
            OCR0A = 0;
        }
		g_timer_1 = 0;
	}

    // Check if it's time to toggle LED 2
	if(g_timer_2++ >= g_delay_2) {

        // Toggle LED 2
        if(OCR0B == 0) {
            OCR0B = g_brightness_2;
        } else {
            OCR0B = 0;
        }
		g_timer_2 = 0;
	}

    // Re-enable interrupts
    sei();
}

void init_timers() {
    // Disable interrupts
	cli();

    // Prepare the 2 LEDs on pins PD6 and PD5
	DDRD |= (1 << PD6);
	DDRD |= (1 << PD5);

    // Configure the mode of Timer0 which is going to take care
    // of updating the brightness of the LEDs
	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
	TCCR0B = (1 << CS00);

    // Configure the mode of Timer2 which is goging to take care
    // of blinking the LEDs according to the values provided
    // by the user
	TCCR2A = (1 << WGM21);
	TIMSK2 = (1 << OCIE2A);

	// Value calculated from http://eleccelerator.com/avr-timer-calculator/
    // The OCR2A value allows us to "guarantee" an Timer interruption every
    // 1 msec.
	OCR2A = 0.9765625;
	TCCR2B = (1 << CS20) | (1 << CS21) | (1 << CS22); // 1024 prescalar

	// Enable interrupts
	sei();
}

void updateLEDs(int d1, int i1, int d2, int i2) {
}

//***************************  USART  *****************************
// The following code, is a helper code that allow us to log
// messages and to provide input throght the tty USB

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
}

int main() {

    // Initialize serial monitor communication
	init_USART();

    // Initialize timer0 and timer2
	init_timers();

    // Keep listening for input from the user
    // Input has the following format:
    // <First LED delay>;<First LED brightness 0-255>;<Second LED delay>;<Second LED brightness 0-255>;
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

                // Specify the delimiter between values to be ';'
				while ((token = strsep(&string, ";")) != NULL)
				{
					input = parseInt(token);
					switch(count) {
					case 1: g_delay_1 = input;
					break;
					case 2: g_brightness_1 = input;
					break;
					case 3: g_delay_2 = input;
					break;
					case 4: g_brightness_2 = input;
					break;
					}
					count++;
				}
				free(tofree);
			}
		}
	}
	return 0;
}
