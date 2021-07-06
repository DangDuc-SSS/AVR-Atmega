/*
 * GccApplication1.c
 *
 * Created: 26/06/2021 20:22:13
 * Author : Admin
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
static uint8_t i=0;
void Interrupt_init()
{
	GICR |= 1 << 6;
	MCUCR |= (1 << ISC00);
}
int main(void)
{
    /* Replace with your application code */
	DDRA = 0xFF;
	
	DDRD = 0x00;
	PORTD = 0xff;
	Interrupt_init();
	sei();
    while (1) 
    {

    }
}
ISR(INT0_vect)
{
	i++;
	PORTA = ~PORTA;
}

