/*
 * SPI_Slave.c
 *
 * Created: 06/07/2021 08:25:42
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
void SPI_Slave_Init()
{
	DDRB = (1 << PB6)|(0x00);
	SPCR = (1 << SPE);
}
char SPI_Read()
{
	while(!(SPSR & (1<<SPIF)));	
	return SPDR;
}
void SPI_Write(char data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));	
}
int main(void)
{
    /* Replace with your application code */
	SPI_Slave_Init();
	DDRD = 0xff;
    while (1) 
    {
		PORTD= SPI_Read();
		_delay_ms(1000);
		SPI_Write(0x23);
    }
}

