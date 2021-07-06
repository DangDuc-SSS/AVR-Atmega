/*
 * Timer0.c
 *
 * Created: 27/06/2021 14:17:13
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
void Timer0_Interrup()
{
	TCNT0 = 56;  // bat dau dem
	TCCR0 |= (1 << CS01);  // cai dat bo chia
	TIMSK |= 1 << TOIE0;	// cho phep ngat khi overflow
	while (!(TIFR&(1 << TOV0)));	// doi overflow
}
void Timer2_Delay()
{
	TCNT2 = 156;
	TCCR2 |= (1 << CS21);
	while (!(TIFR&(1 << TOV2)));
	TCCR2 = 0;
	TIFR = 1<<TOV2;
}
ISR(TIMER0_OVF_vect)
{
		TCNT0 = 156;
		PORTA = ~PORTA;
}
int main(void)
{
    /* Replace with your application code */
	
	DDRA = 0xff;
	DDRD = 0xff;
	PORTA = 0xff;
	PORTD = 0;
	Timer0_Interrup();
	sei();

    while (1) 
    {
		PORTD = ~ PORTD;
		Timer2_Delay();
    }
}

