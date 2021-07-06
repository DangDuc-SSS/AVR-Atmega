/*
 * Timer1_input.c
 *
 * Created: 28/06/2021 07:56:53
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
void LCD_cmd(char cmd)
{
	PORTD = (PORTD&0xf0)|(cmd>>4);
	PORTC &= ~(1 << PC0);
	PORTC &= ~(1 << PC1);
	PORTC |= 1 << PC2;
	_delay_ms(1);
	PORTC &= ~(1 << PC2);
	_delay_ms(1);
			PORTD = (PORTD&0xf0)|(cmd&0x0f);
			PORTC &= ~(1 << PC0);
			PORTC &= ~(1 << PC1);
			PORTC |= 1 << PC2;
			_delay_ms(1);
			PORTC &= ~(1 << PC2);
			_delay_ms(1);
}
void LCD_data(char data)
{
		PORTD = (PORTD&0xf0)|(data>>4);
		PORTC |= 1 << PC0;
		PORTC &= ~(1 << PC1);
		PORTC |= 1 << PC2;
		_delay_ms(1);
		PORTC &= ~(1 << PC2);
		_delay_ms(1);
				PORTD = (PORTD&0xf0)|(data&0x0f);
				PORTC |= 1 << PC0;
				PORTC &= ~(1 << PC1);
				PORTC |= 1 << PC2;
				_delay_ms(1);
				PORTC &= ~(1 << PC2);
				_delay_ms(1);
}
void LCD_Init ()			/* LCD Initialize function */
{
	DDRD = 0x0f;
	//PORTD =0xff;
	DDRC = 0xff;
	//PORTC = 0xff;
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	LCD_cmd (0x02);
	LCD_cmd (0x28);		/* Initialization of 16X2 LCD in 8bit mode */
	LCD_cmd (0x0C);		/* Display ON Cursor OFF */
	LCD_cmd (0x06);		/* Auto Increment cursor */
	LCD_cmd (0x01);		/* Clear display */
}
void LCD_string(char *data)
{
	uint8_t i = 0;
	while(data[i])
	{
		LCD_data(data[i]);
		i++;
	}
}
void Int_String(int number, char* data)
{
	int temp = number;
	uint8_t count = 0,i=0;
	while (temp)
	{
		temp/=10;
		count++;
	}
	for (i = 0; i < count; i++)
	{
		data[count-1-i] = (char)(number%10)+'0';
		number /=10;
	}
	data[count]=' ';
}
void LCD_xy(char row, char col, char *data)
{
	if (row==0 && col < 16)
	{
		LCD_cmd((col&0x0f)|(0x80));
	}
	else if (row==1 && col < 16)
	{
		LCD_cmd((col&0x0f)|(0xC0));
	}
	LCD_string(data);
}
int Timer1_Input_Mode()
{
	int a= 0, b=0;
	DDRD &=~(1 << PD6);
	TIFR |= 1<< ICF1;
	
	TCCR1B |= (1 << ICES1)|(1 << CS11);
	while(!(TIFR&(1<< ICF1)));
	a = ICR1;
		//TCCR1B=0;
	TIFR |= 1<< ICF1;
	
		TCCR1B |= (1 << CS11);
		while(!(TIFR&(1<< ICF1)));
		b = ICR1;
	return b-a;
}
int main(void)
{
    /* Replace with your application code */
	char mang[10];
	LCD_Init();
    while (1) 
    {
		
		int temp = 0;
		temp = Timer1_Input_Mode();
		Int_String(temp,mang);
		LCD_xy(1,0,mang);
    }
}

