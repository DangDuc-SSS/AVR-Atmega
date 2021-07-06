/*
 * I2C.cpp
 *
 * Created: 14/10/2020 20:04:52
 * Author : Admin
 */ 
#define  F_CPU 8000000UL
#include <avr/io.h>
//#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <stdio.h>
int second,minute,hour,day,date,month,year;

void I2C_Init()
{
	TWBR = 32;
}
uint8_t I2C_Start(char w_adress)
{
	uint8_t status;
	TWCR = (1 << TWSTA)|(1<<TWEN)|(1<<TWINT);
	while (!(TWCR&(1<<TWINT)));
	status = TWSR&0xF8;
	if (status!=0x08)
		return 0;
	TWDR = w_adress;
	TWCR=(1<<TWEN)|(1<<TWINT);
	while(!(TWCR&(1<<TWINT)));
	status=TWSR&0xF8;		
	if(status==0x18)	
		return 1;			
	if(status==0x20)
		return 2;		
	else
		return 3;
}
uint8_t I2C_Write(char data)
{
	uint8_t status;	
	TWDR=data;			
	TWCR=(1<<TWEN)|(1<<TWINT);	
	while(!(TWCR&(1<<TWINT)));	
	status=TWSR&0xF8;	
	if(status==0x28)	
		return 0;		
	if(status==0x30)	
		return 1;			
	else
		return 2;			
}	
uint8_t I2C_Repeated_Start(char r_address) /* I2C repeated start function */
{
	uint8_t status;		
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
	while(!(TWCR&(1<<TWINT)));	
	status=TWSR&0xF8;		
	if(status!=0x10)	
	return 0;			
	TWDR=r_address;		
	TWCR=(1<<TWEN)|(1<<TWINT);	
	while(!(TWCR&(1<<TWINT)));
	status=TWSR&0xF8;		
	if(status==0x40)	
		return 1;	
	if(status==0x48)		
		return 2;
	else
		return 3;	
}
char I2C_Read_Ack()		/* I2C read ack function */
{
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA); /* Enable TWI, generation of ack */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	return TWDR;			/* Return received data */
}
char I2C_Read_Nack()		/* I2C read nack function */
{
	TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	return TWDR;		/* Return received data */
}
void I2C_Stop()			/* I2C stop function */
{
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);/* Enable TWI, generate stop */
	while(TWCR&(1<<TWSTO));	/* Wait until stop condition execution */
}

void RTC_Clock_Write(char _hour, char _minute, char _second, char AMPM)
{
	_hour |= AMPM;
	I2C_Start(0xD0);/* Start I2C communication with RTC */
	I2C_Write(0);			/* Write 0 address for second */
	I2C_Write(_second);		/* Write second on 00 location */
	I2C_Write(_minute);		/* Write minute on 01(auto increment) location */
	I2C_Write(_hour);		/* Write hour on 02 location */
	
	I2C_Stop();			/* Stop I2C communication */
}

/* function for calendar */
void RTC_Calendar_Write(char _day, char _date, char _month, char _year, char _SQ)
{
	I2C_Start(0xD0);/* Start I2C communication with RTC */
	I2C_Write(3);			/* Write 3 address for day */
	I2C_Write(_day);		/* Write day on 03 location */
	I2C_Write(_date);		/* Write date on 04 location */
	I2C_Write(_month);		/* Write month on 05 location */
	I2C_Write(_year);		/* Write year on 06 location */
	I2C_Write(_SQ);
	I2C_Stop();			/* Stop I2C communication */
}
void RTC_Read_Clock(char read_address)
{
	I2C_Start(0xD0);/* Start I2C communication with RTC */
	I2C_Write(read_address);	/* Write address to read */
	I2C_Repeated_Start(0xD1);/* Repeated start with device read address */

	second = I2C_Read_Ack();	/* Read second */
	minute = I2C_Read_Ack();	/* Read minute */
	hour = I2C_Read_Ack();		/* Read hour with Nack */
	
	day = I2C_Read_Ack();		/* Read day */
	date = I2C_Read_Ack();		/* Read date */
	month = I2C_Read_Ack();		/* Read month */
	year = I2C_Read_Nack();		/* Read the year with Nack */
	I2C_Stop();			/* Stop i2C communication */
}

//void RTC_Read_Calendar(char read_calendar_address)
//{
	//I2C_Start(0xD0);
	//I2C_Write(read_calendar_address);
	//I2C_Repeated_Start(0xD1);
//
	//day = I2C_Read_Ack();		/* Read day */
	//date = I2C_Read_Ack();		/* Read date */
	//month = I2C_Read_Ack();		/* Read month */
	//year = I2C_Read_Nack();		/* Read the year with Nack */
	//I2C_Stop();			/* Stop i2C communication */
//}

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
	char buffer[20];
	char* days[7]= {"CN","Hai","Ba","Tu","Nam","Sau","Bay"};
	I2C_Init();			
	LCD_init();
	RTC_Clock_Write(24, 33, 0, 0);
	RTC_Calendar_Write(1, 19, 18, 32, 16);
	while (1)
    {
		RTC_Read_Clock(0);
		sprintf(buffer, "%02x:%02x:%02x  ",(hour & 0b00111111), minute, second);
		display_xy(0,0,buffer);
		//RTC_Read_Calendar(3);	
		sprintf(buffer, "%02x/%02x/%02x %3s ", date, month, year,days[day-1]);
		display_xy(1,0,buffer);
    }
}

