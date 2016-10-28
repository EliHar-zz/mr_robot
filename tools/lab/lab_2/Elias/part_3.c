#ifndef F_CPU
#define F_CPU 1000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

int timerOn = 0;

void setup(){
	// Disable interrupts in case of another interrupt
	cli();
	// Set pin PB1, PB2 as OUTPUT, will connect to LED
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);
	// Pin initial state is low
	PORTB &= ~(1 << PB1);
	PORTB &= ~(1 << PB2);
	// Set pin PD2 (int0) as input
	DDRD &= ~(1 << PD2);
	// any logical change will trigger interrupt
	EICRA |= ((0 << ISC01) | (1 << ISC00));
	//ENABLE INT0
	EIMSK |= (1 << INT0);
	// Timer stopped as initial state
	TIMSK1 = 0x00;
	TCCR1B = 0x00;
	// turn on interrupts
    sei();
}

void startTimer() {
	// Disable interrupts in case of another interrupt
	cli();
	// turn timer flag on
	timerOn = 1;
	// enable Timer1 overflow interrupt
	TIMSK1 |= (1 << TOIE1);
	// counter increments every 64 clock cycles
	TCCR1B |= ((0 << CS12) | (1 << CS11) | (1 << CS10));
	// Re-enable interrupts
	sei();
}

void stopTimer() {
	// disable interrupts
	cli();
	// turn timer flag off
	timerOn = 0;
	// mode (disconnect clock source)
	TIMSK1 = 0x00;
	TCCR1B = 0x00;
	// Re-enable interrupts
	sei();
}

void toggleLed(int pin) {
	// Disable interrupts in case of another interrupt
	cli();
	// Turn LED on, set PB1 to high
	PORTB ^= (1 << pin);
	// Re-enable interrupts
	sei();
}

// Interrupt vector for when button is pressed
ISR(INT0_vect)
{
	// Use another LED as control, since button causes bounce
	// If this LED is on then the timer is counting
	toggleLed(PB2);

	if (timerOn == 0)
	{
		startTimer();
	} else
	{
		stopTimer();
	}
}

// Interrupt vector for when timer 1 overflows
ISR(TIMER1_OVF_vect)
{
	toggleLed(PB1);
}

void loop(){
	while(1){}
}

int main(void){
	setup();
	loop();
	return 0;
}
