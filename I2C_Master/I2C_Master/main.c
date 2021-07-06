/*
 * I2C_Master.c
 *
 * Created: 01/07/2021 06:45:47
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

void I2C_Init()
{
	TWBR = 32;
}
char I2C_Start()
{
	uint8_t status = 0;
	TWCR = (1 << TWSTA)|(1 << TWEN)|(1 << TWINT);
	while(!(TWCR&(1 << TWINT)));
	status = TWSR&(0xF8);
	if(status != 0x08)
		return 0;
	return 1;
}
char I2C_Rep_Start()
{
	uint8_t status = 0;
	TWCR = (1 << TWSTA)|(1 << TWEN)|(1 << TWINT);
	while(!(TWCR&(1 << TWINT)));
	status = TWSR&(0xF8);
	if(status != 0x10)
		return 0;
	return 1;
}
char I2C_Address(char w_address)
{
	uint8_t status = 0;
	TWDR = w_address;
	TWCR = (1 << TWINT)|(1 << TWEN);
	while(!(TWCR&(1 << TWINT)));
	status = TWSR&(0xF8);
	if(status == 0x18)	
		return 0;
	if(status == 0x20)
		return 1;
	else
		return 2;
	
}
char I2C_Write(char data)
{
	uint8_t status = 0;
	TWDR = data;
	TWCR = (1 << TWINT)|(1 << TWEN);
	while(!(TWCR&(1 << TWINT)));
	status = TWSR&(0xF8);
	if(status==0x28)
		return 0;
	if(status == 0x30)
		return 1;
	return 2;

}
void I2C_Stop()
{
	TWCR =(1 << TWINT)|(1 << TWEN)|(1 << TWSTO);
	while((TWCR&(1 << TWSTO)));
	
}
int main(void)
{
    /* Replace with your application code */;
	I2C_Init();
    while (1) 
    {
			
			I2C_Start();
			I2C_Write(0x02);
			I2C_Write('D');
			I2C_Write('U');
			I2C_Write('C');
			I2C_Stop();
			_delay_ms(1500);
    }
}

