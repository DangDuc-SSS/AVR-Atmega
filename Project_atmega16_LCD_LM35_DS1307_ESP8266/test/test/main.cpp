#define  F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#define BAUD_PRESCALE (((F_CPU / (baudrate * 16UL))) - 1)
uint8_t s = 0;
char A[25];
char B[16];
int b=0;
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
	//LCD_cmd(0x38); // 8bit 2 hang
	//LCD_cmd(0x0c); //bat man hinh va tat con tro
	//LCD_cmd(0x06); //con tro dich phai
	//LCD_cmd(0x01); // xoa man hinh
	//LCD_cmd(0x80); // dua con tro ve vi tri ban dau
	//LCD_cmd(0x33);
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

void Uart_Init(long baudrate)
{
	UCSRB |= (1 << RXEN)|(1 << TXEN);
	UCSRC |= (1 << URSEL)|(1 << UCSZ1)|(1 << UCSZ0);
	UBRRL = BAUD_PRESCALE;
	UBRRH = (BAUD_PRESCALE >> 8);
}

void gui(char c)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = c;
}
void guikitu(char *str)
{
	unsigned char i=0;
	while(str[i]!=0)
	{
		gui(str[i]);
		i++;
	}
}

void ADC_init()
{
	DDRA = 0x00;
	ADMUX |= (1<<REFS0)|(1<<MUX0);
	ADCSRA |= (1<<ADEN)|(1<<ADPS0)|(1<<ADPS1);

}

int read_adc()
{
	ADCSRA |= (1<<ADSC);
	while (!(ADCSRA & (1<<ADIF)));  
	_delay_ms(1);  
	return ADCW;
}

void select_init()
{
		DDRD =0x00;
		//DDRA = 0xff;
		/* Replace with your application code */
		MCUCR |= (1 << ISC01 )|(1 << ISC11 );
		GICR = (1 << INT0)|(1 << INT1);
		sei();
}

char second,minute,hour,day,date,month,year;
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
void RTC_Read_Clock(char read_clock_address)
{
	I2C_Start(0xD0);/* Start I2C communication with RTC */
	I2C_Write(read_clock_address);	/* Write address to read */
	I2C_Repeated_Start(0xD1);/* Repeated start with device read address */

	second = I2C_Read_Ack();	/* Read second */
	minute = I2C_Read_Ack();	/* Read minute */
	hour = I2C_Read_Nack();		/* Read hour with Nack */
	I2C_Stop();			/* Stop i2C communication */
}

void RTC_Read_Calendar(char read_calendar_address)
{
	I2C_Start(0xD0);
	I2C_Write(read_calendar_address);
	I2C_Repeated_Start(0xD1);

	day = I2C_Read_Ack();		/* Read day */
	date = I2C_Read_Ack();		/* Read date */
	month = I2C_Read_Ack();		/* Read month */
	year = I2C_Read_Nack();		/* Read the year with Nack */
	I2C_Stop();			/* Stop i2C communication */
}
void Timer1()
{
	TCCR1B=(1<CS10)|(1<<CS12);
	TCNT1=1;
	TIMSK=(1<<TOIE1);
	sei();
}
int main(void)
{
	int c=35;
	LCD_init();
	ADC_init();
	select_init();
	Uart_Init(9600);
	char buffer[20];
	char* days[8]= {0,"Cn","Hai","Ba","Tu","Nam","Sau","Bay"};
	Timer1();
	I2C_Init();
	
	while(1)
	{	
		PORTA |= (1 << PA6)|(1 << PA7);
		if (!(PINA & (1<<6)))
		{
			c++;
		}
		if (!(PINA & (1<<7)))
		{
			c--;
		}
		
			
		int dem = 0, tong = 0;
		for (uint8_t i=0; i<10; i++)
		{
			read_adc();
			_delay_ms(10);
			tong += read_adc();
			dem++;
		}
		_delay_ms(10);
		int a = tong/dem;
		b=(int)(a*0.488756);	
		
		
		if (s == 0)
		{
			RTC_Read_Clock(0);
			sprintf(buffer, "<< Time %02x:%02x:%02x    ", (hour & 0b00111111), minute, second);
			display_xy(0,0,buffer);
			RTC_Read_Calendar(3);
			sprintf(buffer, "  %x-%x-%x %s   ", date, month, year, days[day]);
			display_xy(1,0,buffer);
		}
		else
		{
			sprintf(A,"nhiet do:%d\337c >>",b);
			display_xy(0,0,A);
			sprintf(B,"LC:%d",c);
			display_xy(1,9,B);
			if (b > c)
			{
				display_xy(1,0,"hot!!!   ");
			}
			else
				display_xy(1,0,"nomarl   ");
		}
		
		if (b > c)
		{
			DDRA |= (1<< 2);
			PORTA |= (1<< PA2) ;
		}
		else
		{
			PORTA &= ~(1<< PA2) ;
		}
	}
}
ISR(INT0_vect)
{
	s=0;

}

ISR(INT1_vect)
{
	s=1;
}
ISR (TIMER1_OVF_vect ){
	TCNT1=1;
	char C[25]="";
	sprintf(C,"%d",b);
	guikitu(C);
}