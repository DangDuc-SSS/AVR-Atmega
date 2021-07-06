/*
 * UART.c
 *
 * Created: 30/06/2021 07:46:54
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#define BUADRATE 9600
#define BUAD_INIT ((F_CPU/(16UL*BUADRATE))-1)
void Timer0_Delay_ms(int time)
{
	int i = 0;
	for (i = 0; i < time*5; i++)
	{
		TCNT0 = 55;
		TCCR0 = 1 << CS01;
		while(!(TIFR&(1<<TOV0)));
		TCCR0 = 0;
		TIFR |= 1 << TOV0;
	}
	 
}
void UART_Init()
{
	DDRD |= (1 <<PD1);
	UBRRL = BUAD_INIT;
	UBRRH = BUAD_INIT >> 8;
	UCSRB |= (1 << TXEN)|(1 << RXEN)|(1 << TXCIE);
	UCSRC |= (1 < URSEL)|(1 << UCSZ1)|(1 << UCSZ0);

}
void Sent_data(char data)
{
	while(!(UCSRA&(1 << UDRE)));
	UDR = data;
	while(!(UCSRA&(1 << TXC)));
}
char Rx_data()
{
	while(!(UCSRA&(1 << RXC)));
	return UDR;
}
unsigned char Sent_String(char *data)
{
	uint8_t i = 0;
	while (data[i])
	{
		Sent_data(data[i]);
		i++;
	}
}
int main(void)
{
    /* Replace with your application code */
	UART_Init();

	//sei();
    while (1) 
    {
		char rx = Rx_data();
		Sent_data(rx);
    }
}
ISR(USART_RXC_vect)
{

}