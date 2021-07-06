/*
 * SPI_Master.c
 *
 * Created: 06/07/2021 07:57:52
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
void SPI_Master_Init()
{
	DDRB = (1 << PB4)|(1 << PB5)|(1 << PB7)&~(1 << PB6);
	PORTB = 1 << PB4;
	SPCR = (1 << SPE)|(1 << MSTR)|(1 << SPI2X)|(1 << SPR0);
	
}
void SPI_Write(char data)
{
	SPDR = data;
	while(!(SPSR&(1<<SPIF)));
}
char SPI_Read()
{
	while(!(SPSR&(1<<SPIF)));
	return SPDR;
}
int main(void)
{
    /* Replace with your application code */
	SPI_Master_Init();
	PORTB = ~(1 << PB4);
	DDRD = 0xff;
    while (1) 
    {
		SPI_Write(0x32);
		PORTD = SPI_Read();
		_delay_ms(1000);
    }
}

