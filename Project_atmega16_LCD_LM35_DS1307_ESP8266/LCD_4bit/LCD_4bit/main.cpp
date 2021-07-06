/*
 * LCD_4bit.cpp
 *
 * Created: 30/10/2020 17:13:22
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
void LCD_cmd(unsigned char cmd)
{
	PORTB = (PORTB & 0x0F) | (cmd & 0xF0);
	PORTB &= ~(1<<PB0);
	PORTB &= ~(1<<PB1);
	PORTB |= (1<<PB2);
	_delay_us(1);
	PORTB &= ~(1<<PB2);
	_delay_us(200);
	PORTB = (PORTB & 0x0F) | (cmd << 4);
	PORTB |= (1<<PB2);
	_delay_us(1);
	PORTB &= ~(1<<PB2);
	_delay_ms(2);
}
void LCD_data(unsigned char data)
{
	PORTB = (PORTB & 0x0F) | (data & 0xF0);
	PORTB |= (1<<PB0);
	PORTB |= (1<<PB2);
	_delay_us(1);
	PORTB &= ~(1<<PB2);
	_delay_us(200);
	PORTB = (PORTB & 0x0F) | (data << 4);
	PORTB |= (1<<PB2);
	_delay_us(1);
	PORTB &= ~(1<<PB2);
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
	while(1)
	{
		display_string("12345");
	}
}
