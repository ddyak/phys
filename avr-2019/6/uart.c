#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>


void USART_Init() {
	UBRR0H = 0;
	UBRR0L = 103;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (0 << RXCIE0) | (0 << TXCIE0);
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

uint8_t USART_Receive() {
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

// ISR( USART0_RX_vect ) {
// 	cli();
// 	UART_Send_Str("ISR");
// 	unsigned char rxbyte = USART_Receive();
// 	UART_Send_Str(rxbyte);
// 	sei();
// }


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