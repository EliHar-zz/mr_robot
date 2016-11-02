#include <avr/io.h>
#include <util/delay.h>
#include <compat/twi.h>
#include <string.h>

#define FOSC 1000000UL                       // Clock Speed
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#define ERROR 0
#define SUCCESS 1
#define EEDEVADR 0b10100000

uint16_t Master_addr = 0x07F1;
uint8_t u8ebyte;

/*
 * USART METHODS
 */
char receiveChar() {
	// Wait until data is received
	while (!(UCSR0A & (1 << RXC0))) {}
	// Return the data from the RX buffer
	return UDR0;
}

void sendChar(char c) {
	// Wait until buffer is empty
	while (!(UCSR0A & (1 << UDRE0))) {}
	// Return the data from the RX buffer
	UDR0 = c;
}

void sendMessage(char msg[]) {
	for (int i = 0; msg[i]; i++) {
		sendChar(msg[i]);
	}
}

void USART_Init() {
	/*Set baud rate */
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
	// Enable Receiver
	UCSR0B |= (1 << TXEN0);
	// Set frame: 8data, 1 stp
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

/*
 * I2C METHODS
 */

void TWIInit(void)
{
	//set SCL to 400kHz
	TWSR = 0x00;
	TWBR = 0x0C;
	//enable TWI
	TWCR = (1<<TWEN);
}

uint8_t TWIReadACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); //Send START condition
	//Wait for TWINT Flag set. This indicates that the START condition has been transmitted
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

//read byte with NACK
uint8_t TWIReadNACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

void TWIWrite(uint8_t u8data)
{
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t TWIGetStatus(void)
{
	uint8_t status;
	//mask status
	status = TWSR & 0xF8;
	return status;
}

uint8_t EEWriteByte(uint16_t u16addr, uint8_t u8data)
{
	while ((TWCR & (1<<TWINT)) == 0);
	if (TWIGetStatus() != 0x08) // A START condition hasn't been transmitted
		return ERROR;
	//select device and send A2 A1 A0 address bits
	TWIWrite((EEDEVADR)|(uint8_t)((u16addr & 0x0700)>>7));
	if (TWIGetStatus() != 0x18)// if SLA+W not transmitted or status not ACK
		return ERROR;
	//send the rest of address
	TWIWrite((uint8_t)(u16addr));
	if (TWIGetStatus() != 0x28) // Addr nottransmitted or status not ACK
		return ERROR;
	//write byte to eeprom
	TWIWrite(u8data);
	if (TWIGetStatus() != 0x28)// Data byte not transmitted or status not ACK
		return ERROR;
	return SUCCESS;
}

/*
 * ADC METHODS
 */
void ADC_Init() {
	// initialize adc
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

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

//Converts integer to String (char*)
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
	// Setup connection to Serial monitor
	USART_Init();
	// Setup TWI
	TWIInit();
	// Init ADC
	ADC_Init();
}

void loop() {
	uint16_t adc_result;
	while(1) {
		adc_result = read_adc(0);
		// convert value of ADC
		char adc_result_str[] = "";
		itoa(adc_result, adc_result_str);
		strcat(adc_result_str, "\r\n");
		// Send IR reading to Master
		for (int i = 0; adc_result_str[i]; i++){
			EEWriteByte(Master_addr, adc_result_str[i]);
		}
	}
}

int main (void)
{
	setup();
	loop();
}
