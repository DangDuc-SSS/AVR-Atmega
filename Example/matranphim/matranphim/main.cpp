/*
 * matranphim.cpp
 *
 * Created: 22-04-2020 16:56:46
 * Author : Admin
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
unsigned char a, b, maphim;
unsigned char number[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
unsigned char maquet[4]={0b11111110,0b11111101,0b11111011,0b11110111};
				  
void kiemtra()
{
	for(int i = 0; i < 4; i ++)
	{
		PORTA = maquet[i];
		{
			for (int j=4;j<8;j++)
			{
				if (bit_is_clear(PINA,j))
				{
					maphim=i*4+j-4;
				}
			}
			//if(bit_is_clear(PINA,4))
			//{
				//maphim= i*4;
			//}
			//if(bit_is_clear(PINA,5))
			//{
				//maphim= i*4+1;
			//}
			//if(bit_is_clear(PINA,6))
			//{
				//maphim= i*4+2;
			//}
			//if(bit_is_clear(PINA,7))
			//{
				//maphim= i*4+3;
			//}
		}
	}
}

void hienthi(int maphim)
{
	a= maphim % 10;
	b= maphim / 10;
	PORTC= (1<<PINC6);
	PORTB= number[b];
	_delay_ms(2);
	PORTC= 0x00;
	
	PORTC= (1<<PINC7);
	PORTB= number[a];
	_delay_ms(2);
	PORTC= 0x00;
}
int main(void)
{
	/* Replace with your application code */
	DDRA= 0x0f;
	DDRB = 0xff;
	DDRC = 0xff;
	while (1)
	{
		kiemtra();
		hienthi(maphim);
	}
}