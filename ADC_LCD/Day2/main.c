/*
 * Day2.c
 *
 * Created: 27/06/2021 06:23:42
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#define LCD_RS PC0
#define LCD_RW PC1
#define LCD_En PC2

void LCD_cmd(char cmd)
{

	PORTD = (PORTD&0xf0)|(cmd>>4);
	PORTC &= ~(1<<LCD_RS);
	PORTC &= ~(1<<LCD_RW);
	PORTC |= (1<<LCD_En);
	_delay_ms(1);
	PORTC &=~(1<<LCD_En);
	_delay_ms(1);
		PORTD = (PORTD&0xf0)|(cmd&0x0f);
		PORTC &= ~(1<<LCD_RS);
		PORTC &= ~(1<<LCD_RW);
		PORTC |= (1<<LCD_En);
		_delay_ms(1);
		PORTC &=~(1<<LCD_En);
		_delay_ms(1);
}
void LCD_data(char data)
{
	PORTD =0x00;
	PORTD |= (data>>4);
	PORTC |= (1<<LCD_RS);
	PORTC &= ~(1<<LCD_RW);
	PORTC |= (1<<LCD_En);
	_delay_ms(1);
	PORTC &=~(1<<LCD_En);
	_delay_ms(1);
	PORTD =0x00;
		PORTD |= (data&0x0f);
		PORTC |= (1<<LCD_RS);
		PORTC &= ~(1<<LCD_RW);
		PORTC |= (1<<LCD_En);
		_delay_ms(1);
		PORTC &=~(1<<LCD_En);
		_delay_ms(1);
}
void LCD_Init (void)			/* LCD Initialize function */
{
	DDRD = 0xff;
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
	int i;
	for(i=0;data[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_data (data[i]);
	}
}
void LCD_xy(char x, char y,char *data)
{
	
	if(x==0 && y<16)
	{
		LCD_cmd((y & 0x0f)| 0x80);
	}
	else if(x==1 && y<16)
	{
		LCD_cmd((y & 0x0f)| 0xC0);
	}
	LCD_string(data);
}
void INT_str(int number,char *mang)
{
	int a =number;
	uint8_t count=0;
	while(a)
	{
		a/=10;
		count++;
	}
	uint8_t i = 0;
	for(i = 0; i < count; i++)
	{
		mang[count-i-1] = (char)(number%10) +'0';
		number/= 10;
	}
	mang[count]=' ';
}
int ADC_Init(char mux)
{
	
	ADMUX |= mux;
	
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA&(1<<ADIF)));
	_delay_ms(10);
	return ADCW;
}
int main(void)
{
    /* Replace with your application code */
	LCD_Init();
	DDRA = 0x00;
	ADCSRA |= (1 << ADEN)|(1 << ADIF)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	char mang[9]={'\0'};
	
    while (1) 
    {
		int i = ADC_Init(0);
		INT_str(i,mang);
		LCD_xy(1,1,mang);

    }
}

