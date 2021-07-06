#define  F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sfr_defs.h>

uint8_t s = 0;
char A[25];
char B[20];
//char C[20];
int  b=0;
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
	LCD_cmd(0x0c); //bat man hinh va tat con tro
	LCD_cmd(0x06); //con tro dich phai
	LCD_cmd(0x01); // xoa man hinh
	LCD_cmd(0x80); // dua con tro ve vi tri ban dau
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

void UART_Init()
{
	UBRRH=0;
	UBRRL=51;
	UCSRA=0x00;
	UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	sei();
}
unsigned char nhan()
{
	while ( !(UCSRA & (1<<RXC)) ) ;
	return UDR;
}
void gui(char c)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = c;
}
void guikitu(char*str)
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
	MCUCR |= (1 << ISC01 )|(1 << ISC00 );
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
uint8_t I2C_Repeated_Start(char r_address)
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
void I2C_Stop()
{
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);
	while(TWCR&(1<<TWSTO));
}
void RTC_Read_Clock(char read_clock_address)
{
	I2C_Start(0xD0);
	I2C_Write(read_clock_address);
	I2C_Repeated_Start(0xD1);

	second = I2C_Read_Ack();
	minute = I2C_Read_Ack();
	hour = I2C_Read_Nack();
	I2C_Stop();
}

void RTC_Read_Calendar(char read_calendar_address)
{
	I2C_Start(0xD0);
	I2C_Write(read_calendar_address);
	I2C_Repeated_Start(0xD1);

	day = I2C_Read_Ack();
	date = I2C_Read_Ack();
	month = I2C_Read_Ack();
	year = I2C_Read_Nack();
	I2C_Stop();
}

void Timer1()
{
	TCCR1B=(1<<CS10)|(1<<CS12);
	TCNT1=15000;
	TIMSK=(1<<TOIE1);
	sei();
}

int main(void)
{
	int c=25;
	LCD_init();
	ADC_init();
	select_init();
	char buffer[20];
	char* days[8]= {0,"Cn","Hai","Ba","Tu","Nam","Sau","Bay"};
	
	I2C_Init();
	UART_Init();
	Timer1();
	
	
	while(1)
	{
		PORTA = (1 << PA6)|(1 << PA7);
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
			display_xy(1,0,"normal   ");
		}
		
		if (b > c)
		{
			DDRA |= (1<< 2);
			PORTA |= (1<< PA2) ;
			_delay_ms(50);
			
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
	TCNT1=15000;
	char C[25]="";
	sprintf(C,"%d\n",b);
	guikitu(C);
}