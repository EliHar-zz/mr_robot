#include <avr/io.h>
#include <util/delay.h>
#include <compat/twi.h>

#define FOSC 1000000UL                       // Clock Speed
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#define ERROR 0
#define SUCCESS 1
#define EEDEVADR 0b10100000

uint16_t Slave_addr = 0x07F0;

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

void TWIStart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}
//send stop signal
void TWIStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	while (TWCR & (1<<TWSTO));

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
	TWIStart();
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
	TWIStop();
	return SUCCESS;
}

uint8_t EEReadByte(uint16_t u16addr, uint8_t *u8data)
{
	//uint8_t databyte;
	TWIStart();
	if (TWIGetStatus() != 0x08)
		return ERROR;
	//select devise and send A2 A1 A0 address bits
	TWIWrite((EEDEVADR)|((uint8_t)((u16addr & 0x0700)>>7)));
	if (TWIGetStatus() != 0x18) // if status not ACK
		return ERROR;
	//send the rest of address
	TWIWrite((uint8_t)(u16addr));
	if (TWIGetStatus() != 0x28) // Data byte hasn't been transmitted; ACK hasn't been received
		return ERROR;
	//send start
	TWIStart();
	if (TWIGetStatus() != 0x10)// A repeated START condition hasn't been transmitted
		return ERROR;
	//select device and send read bit
	TWIWrite((EEDEVADR)|((uint8_t)((u16addr & 0x0700)>>7))|1);
	if (TWIGetStatus() != 0x40) //SLA+R hasn't been transmitted; ACK hasn't been received
		return ERROR;
	*u8data = TWIReadNACK();
	if (TWIGetStatus() != 0x58) //Data byte hasn't been received; NOT ACK hasn't been returned
		return ERROR;
	TWIStop();
	return SUCCESS;
}

void setup() {
	// Setup connection to Serial monitor
	USART_Init();
	// Setup TWI
	TWIInit();
}

void loop() {
	uint8_t received_byte;
	while(1) {
		EEReadByte(Slave_addr, &received_byte);
		sendChar(received_byte);
	}
}

int main( void ) {
	setup();
	loop();
}
