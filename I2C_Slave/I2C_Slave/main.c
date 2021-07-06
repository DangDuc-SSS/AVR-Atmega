/*
 * I2C_Slave.c
 *
 * Created: 03/07/2021 09:47:31
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
void LCD_cmd(char cmd)
{
	PORTD = (PORTD&0xf0)|(cmd>>4);
	PORTA &= ~(1 << PA0);
	PORTA &= ~(1 << PA1);
	PORTA |= 1 << PA2;
	_delay_ms(1);
	PORTA &= ~(1 << PA2);
	_delay_ms(1);
	PORTD = (PORTD&0xf0)|(cmd&0x0f);
	PORTA &= ~(1 << PA0);
	PORTA &= ~(1 << PA1);
	PORTA |= 1 << PA2;
	_delay_ms(1);
	PORTA &= ~(1 << PA2);
	_delay_ms(1);
}
void LCD_data(char data)
{
	PORTD = (PORTD&0xf0)|(data>>4);
	PORTA |= 1 << PA0;
	PORTA &= ~(1 << PA1);
	PORTA |= 1 << PA2;
	_delay_ms(1);
	PORTA &= ~(1 << PA2);
	_delay_ms(1);
	PORTD = (PORTD&0xf0)|(data&0x0f);
	PORTA |= 1 << PA0;
	PORTA &= ~(1 << PA1);
	PORTA |= 1 << PA2;
	_delay_ms(1);
	PORTA &= ~(1 << PA2);
	_delay_ms(1);
}
void LCD_Init ()			/* LCD Initialize function */
{
	DDRD |= 0x0f;
	//PORTD =0xff;
	DDRA = 0xff;
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
void I2C_Slave_Init(uint8_t slave_address)
{
	TWAR=slave_address;		

}
char I2C_Read_Ack()	
{
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA); 
	while(!(TWCR&(1<<TWINT)));	
	return TWDR;		
}
char I2C_Read_Nack()	
{
	TWCR=(1<<TWEN)|(1<<TWINT);	
	while(!(TWCR&(1<<TWINT)));	
	return TWDR;		
}

int main(void)
{
 
	DDRB=0xff;
	LCD_Init();
	I2C_Slave_Init(0x02);

	char a[10];
    while (1)
    {
		uint8_t i = 0;
		char addr;
		addr = I2C_Read_Ack();
		while(i < 3)
		{
			a[i++]= I2C_Read_Ack() ;			
		}
		LCD_string(a);
	}
}

