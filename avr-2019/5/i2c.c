#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

char data;


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

int USART_Receive() {
	// while (!(UCSR0A & (1 << RXC0)));
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

void i2cInit(){
	TWBR = 5;
	TWSR = (0<<TWPS1)|(0<<TWPS0);
}

void i2cStop(){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void i2cStart(){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

char i2cReciveLast() {
	TWCR = (1<<TWINT|1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

char i2cRecive(){
	TWCR = (1<<TWINT|1<<TWEN|1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

void i2cSend(char send){
	TWDR = send;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void i2cSendStr(char str[]){
	unsigned char i = 0;

	while (str[i]!='\0')
	{
		i2cSend(str[i]);
		i++;
	}
}

void main(void){
	data = '0';
	char addr = 0b10100010;
	char recieveData;
	USART_Init();
	
	i2cInit();
	
	while(1)
 	{
		i2cStart();
		i2cSend(addr);
		i2cSend(0);
		i2cSendStr("hello world");
		i2cStop();
		
	   	_delay_ms(5000);
		
		i2cStart();
		i2cSend(addr);
		i2cSend(0);
		i2cStart();
		i2cSend(addr+1);
		for(int i = 1; i < 10; i++){
			recieveData = i2cRecive();
			USART_Transmit(recieveData);
	    }
	    recieveData = i2cReciveLast();
	    USART_Transmit(recieveData);
		i2cStop();
	}
}