/*
 * maytinh.cpp
 *
 * Created: 27/07/2020 09:08:08
 * Author : Admin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
unsigned char maphim[16]={'1','2','3','+','4','5','6','-','7','8','9','*','=','0','/'};
unsigned char maquet[4]={0b11111110,0b11111101,0b11111011,0b11110111};
char so=20,count=0, operator1 = 'a';
int operand1=0, operand2=0, *operand, result;
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
					so =i*4+j-4;	
					count++	;
				}
			}
		}
		
	}
}
void LCD_cmd(unsigned char cmd)
{
	
	//PORTC &= ~(0x20);   //RS=0 0100
	//PORTC &= ~(0x40);   //RW=0
	//PORTC |= (0x80);    //RE=1
	PORTC = 0b10000000;
	_delay_ms(1);
	PORTD = cmd ;
	//PORTC &= ~(0x80); //RE=0
	PORTC = 0b00000000;
	

}
void LCD_data(unsigned char data)
{
	PORTD = data ;
	//PORTC |= (0x20); //RS=1 0010
	//PORTC &= ~(0x40);// 0100
	//PORTC |= (0x80); // 1000
	PORTC = 0b10100000;
	_delay_ms(1);
	//PORTC &= ~(0x80);// 0111
	PORTC = 0b00100000;
}
void lcd_init()
{
	PORTC=0xff;
	PORTD=0xff;
	_delay_ms(10);
	LCD_cmd(0x38); // 8bit 2 hang
	LCD_cmd(0x0e); //bat man hinh va tat con tro
	//LCD_cmd(0x06); //con tro dich phai
	LCD_cmd(0x01); // xoa man hinh
	LCD_cmd(0x80); // dua con tro ve vi tri ban dau
}
void display_string(char *str)
{
	for (int i=0; str[i]!=0; i++)
	{
		LCD_data(str[i]);
	}
}

void display_xy(char row,char pos,char *str)
{
	if (row==0 && pos<16)
	{
		LCD_cmd((pos & 0x0f)|0x80);
	}
	else if (row==1 && pos < 16)
	{
		LCD_cmd((pos & 0x0f)|0xc0);
	}
	display_string(str);
}
int num_generator(char data , int *operand)
{
	int temp;
	temp = data - '0';
	*operand = *operand*10 + temp;
}
void write_result(int num)
{
	int i=0 ,j ,rem;
	char rev_num[20];
	do 
	{
		rem = num%10;
		num = num/10;
		rev_num[i] =  (char) rem + '0';
		i++;
	} while (num>0);
	for (j=i-1; j>=0; j--)
	{
		LCD_cmd(0xc0);
		LCD_data(rev_num[j]);
	}
}
int main(void)
{
	char c = 20,d=0;
	DDRD = 0xff;
	DDRC = 0xff;
	DDRA = 0x0f;
	DDRB = 0xff;
	PORTB = 0x00;
	lcd_init();
	operand = &operand1;
	while (1)
	{
		kiemtra();
		if (count != 0)
		{		
				LCD_data(maphim[so]);
				if ((maphim[so]=='+') || (maphim[so]=='-') || (maphim[so]=='*') || (maphim[so]=='/') )
				{
					operand = &operand2;
					operator1 = maphim[so];
					
				}
				if ((maphim[so]!='+') && (maphim[so]!='-') && (maphim[so]!='*') && (maphim[so]!='/') && (maphim[so]!='='))
						{
							num_generator(maphim[so],operand);
						}
				if (maphim[so] == '=')
				{
					if (operator1 == '+')
					{
						result = operand1 + operand2;
					}
					if (operator1 == '-')
					{
						result = operand1 - operand2;
					}
					if (operator1 == '*')
					{
						result = operand1 * operand2;
					}
					if (operator1 == '/')
					{
						result = operand1 / operand2;
					}
					write_result(result);
					
				}
				_delay_ms(15);
				count = 0;
				d++;
		}
		if (d>15)
		{
			d=15;
			LCD_cmd(0x18);
		}

		//display_xy(0,0,"Hello word!");
		//display_xy(1,0,"My name is Duc!");
	}

}

