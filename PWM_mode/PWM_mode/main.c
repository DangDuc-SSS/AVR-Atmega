/*
 * PWM_mode.c
 *
 * Created: 29/06/2021 08:43:24
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
void PWM_T0(int a)
{

	OCR0 = a;
	TCCR0 = (1 <<WGM01)| (1 << WGM00)|(1 << CS01)|(1 << COM01);
	while(!(TIFR&(1 << OCF0)));
	TIFR = 1 << OCF0;	
}
int Input_T1()
{
	int temp=0;
	TIFR = 1 << ICF1;
	TCCR1B = (1 << ICES1)|(1 << CS01);
	while (!(TIFR&(1 << ICF1)));
	temp = ICR1;

		TIFR = 1 << ICF1;
		TCCR1B = (1 << ICES1)|(1 << CS01);
		while (!(TIFR&(1 << ICF1)));
		temp = ICR1-temp;
	return temp;
}
void PWM_T1()
{
	OCR1A = 100;
	TCCR1A = (1 << COM1A1)|(1 << WGM10);
	TCCR1B = (1<<WGM12)|(1 << CS11);
	while(!(TIFR&(1 << OCF1A)));
	TIFR = 1 << OCF1A;
}
int main(void)
{
    /* Replace with your application code */
	DDRB = 0xff;
	DDRD = 0xff;
    while (1) 
    {
		PWM_T1();
    }
}

