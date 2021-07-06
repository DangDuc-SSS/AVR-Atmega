f/*
 * led7x4.cpp
 *
 * Created: 03/02/2020 19:39:50
 * Author : Admin
 */ 
#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
	unsigned char a, b, c, d;
	unsigned int x;
    unsigned char number[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
	unsigned char m[4], i ;

void hienthi(int x)
{
	PORTC= 0xFF;
	DDRC = 0xFF;
	PORTD= 0xFF;
	DDRD = 0xFF;
	int temp;
	temp = x ;
	// tach 4 so luu vao 4 bien
	for (i=0 ; i < 4; i++)
	{
		m[i]= temp % 10 ;
		temp= temp / 10 ;
	}
	
		cbi(PORTC,0);
		PORTD = number[m[3]] ;
		_delay_ms(1);
		sbi(PORTC,0);
		
		cbi(PORTC,1);
		PORTD = number[m[2]] ;
		_delay_ms(1);
		sbi(PORTC,1);
		
		cbi(PORTC,2);
		PORTD = number[m[1]] ;
		_delay_ms(1);
		sbi(PORTC,2);
		
		cbi(PORTC,3);
		PORTD = number[m[0]] ;
		_delay_ms(1);
		sbi(PORTC,3);
}
int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		{
			for (int j=0 ; j<= 15 ; j++)
			hienthi(x);
		}
		x++;
		
    }
}

