/*
 * interrup.cpp
 *
 * Created: 05/10/2020 18:57:39
 * Author : Admin
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
uint8_t a = 3, b=0;
int main(void)
{
	DDRD =0x00;
	DDRA = 0xff;
    /* Replace with your application code */
	MCUCR |= (1 << ISC01 )|(1 << ISC00 );
	GICR = (1 << INT0)|(1 << INT1);
	sei();
	
    while (1) 
    {
		PORTA = 1 << a;
    }
}
ISR(INT0_vect)
{
	a=0;
}

ISR(INT1_vect)
{
	a=1;
}
