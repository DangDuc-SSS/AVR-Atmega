/*
 * CTCmode.c
 *
 * Created: 29/06/2021 07:21:21
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>

		//void Normal()
		//{
//
			//OCR0 = 100;
			//TCCR0 = (1 << COM00)|(1 << COM01)|(1 << CS01)|(1<<WGM01);
			//while(!(TIFR&(1 << OCF0)));
			//TIFR = 1 << OCF0;
			//
					//OCR0 = 50;
					//TCCR0 = (1 << COM01)|(1 << CS01)|(1<<WGM01);
					//while(!(TIFR&(1 << OCF0)));
					//TIFR = 1 << OCF0;
//
		//}
		//int main(void)
		//{
			///* Replace with your application code */
			//DDRB = 0xff;
			//while (1) 
			//{
				//Normal();
			//}
		//}
int main ( )
{
	DDRB |= (1<<3);		/*PB3 (OC0) as output */
	while (1)
	{
		OCR0 = 69;
		TCCR0 = 0x39;	/* CTC, set on match, no prescaler */
		while ((TIFR&(1<<OCF0)) == 0);  /* monitor OCF0 flag */
		TIFR = (1<<OCF0);/* Clear OCF0 by writing 1 */
		OCR0 = 99;
		TCCR0 = 0x29;	/* CTC, clear on match, no prescaler */
		while ((TIFR&(1<<OCF0)) == 0);
		TIFR = (1<<OCF0);/* Clear OCF0 by writing 1 */
	}
}