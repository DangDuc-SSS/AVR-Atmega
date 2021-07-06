#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
void ADC_init(){
	DDRA = 0x00;	        /* Make ADC port as input */
	ADCSRA = 0x87;          /* Enable ADC, with freq/128  */
	ADMUX = 0x40;           /* Vref: Avcc, ADC channel: 0 */
}

int read_adc(char channel)
{
	ADMUX = 0x40 | (channel & 0x07);   /* set input channel to read */
	ADCSRA |= (1<<ADSC);               /* Start ADC conversion */
	while (!(ADCSRA & (1<<ADIF)));     /* Wait until end of conversion by polling ADC interrupt flag */
	ADCSRA |= (1<<ADIF);               /* Clear interrupt flag */
	_delay_ms(1);                      /* Wait a little bit */
	return ADCW;                       /* Return ADC word */
}
void LCD_cmd(unsigned char cmd)
{
	PORTB = (PORTB & 0x0F) | (cmd & 0xF0);
	PORTB &= ~(1<<PINB0);
	PORTB &= ~(1<<PINB1);
	PORTB |= (1<<PINB2);
	_delay_us(1);
	PORTB &= ~(1<<PINB2);
	
	_delay_us(200);
	
	PORTB = (PORTB & 0x0F) | (cmd << 4);
	PORTB |= (1<<PINB2);
	_delay_us(1);
	PORTB &= ~(1<<PINB2);
	_delay_ms(2);
}
void LCD_data(unsigned char data)
{
	PORTB = (PORTB & 0x0F) | (data & 0xF0);
	PORTB |= (1<<PINB0);
	PORTB |= (1<<PINB2);
	_delay_us(1);
	PORTB &= ~(1<<PINB2);
	
	_delay_us(200);
	
	PORTB = (PORTB & 0x0F) | (data << 4);
	PORTB |= (1<<PINB2);
	_delay_us(1);
	PORTB &= ~(1<<PINB2);
	_delay_ms(2);
}
void LCD_init()
{
	DDRB=0xff;
	_delay_ms(20);
	LCD_cmd(0x02);
	LCD_cmd(0x28);	/* 2 line, 5*7 matrix in 4-bit mode */
	LCD_cmd(0x0c);	/* Display on cursor off */
	LCD_cmd(0x06);	/* Increment cursor (shift cursor to right) */
	LCD_cmd(0x01);	/* Clear display screen */
	_delay_ms(2);
}
void display_string(char *str)
{
	for (int i=0; str[i]!=0;i++)
	{
		LCD_data(str[i]);
	}
}
void display_xy(char row,char pos,char *str)
{
	if (row==0 & pos<16)
	{
		LCD_cmd((pos & 0x0f)|0x80);
	}
	else if (row==1 && pos < 16)
	{
		LCD_cmd((pos & 0x0f)|0xc0);
	}
	display_string(str);
}

int main(void)
{
	LCD_init();
	ADC_init();
	char A[25];
	while(1)
	{
		int dem = 0, tong = 0;
		for (uint8_t i=0; i<10; i++)
		{
			read_adc(1);
			_delay_ms(10);
			tong += read_adc(1);
			dem++;
		}
		_delay_ms(10);
		int a = tong/dem;
		int b=(int)(a*0.488756);
		sprintf(A,"<< %d        ",a);
		display_xy(0,0,A);
	}
}