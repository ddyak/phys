#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define F_CPU 16000000UL
#define BOUD 9600
#define CO (F_CPU/BOUD/16 - 1)


#include <util/delay.h>


int USART_Receive() {
//	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}


ISR( USART0_RX_vect ) {
	cli();
	unsigned char rxbyte = USART_Receive();
	UART_Send_Str("ISR");
	if (rxbyte == '0') PORTB = 0;
	if (rxbyte == '1') PORTB = 128;
	sei();
//	reti();
}

void USART_Init() {
	UBRR0H = 0;
	UBRR0L = 103;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (0 << TXCIE0);
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
	sei();
}

void USART_Transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0))){};
	UDR0 = data;
}

void UART_Send_Str(const char* str) {
	unsigned char i = 0;
	while (str[i] != '\0') {
		USART_Transmit(str[i]);
		i++;
	}
	USART_Transmit('\n');
}

void Init_ADC() {
 	ADMUX |= (1<<REFS0) | (1<<ADLAR);
 	ADCSRA |= (1<<ADSC) | (1<<ADEN) | (1<<ADATE);
}

void main() {
	Init_ADC();
	USART_Init();

	DDRB = 0xff;
	PORTB = 0x00;
	
	while(1) {
		int low = ADCL; 
		int high = ADCH;
		int res = (high << 2) | (low >> 6);	
		char* str = "00000";
		sprintf(str, "%d", res);
		UART_Send_Str(str);
	}
}
