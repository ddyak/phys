#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

const char device_addr = 0xB8;

void USART_Init() {
	UBRR0H = 0;
	UBRR0L = 103;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (0 << TXCIE0);
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void USART_Transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
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

void TWI_Init() { // (F_CPU / F_SCL) - 16) / (2 * pow(4, twps))
	TWBR = 0x80;
	TWSR = (0 << TWPS1) | (0 << TWPS0);
}

void TWI_Start() {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA) ;
	while (!(TWCR & (1 << TWINT)));
}

void TWI_Stop() {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

char TWI_ReciveLast() {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

char TWI_Recive() {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

void TWI_Send(char send) {
	TWDR = send;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

unsigned int CRC16(unsigned char *ptr, unsigned char length) { 
	unsigned int crc = 0xFFFF; 
	uint8_t s = 0x00; 

	while (length--) {
		crc ^= *ptr++; 
		for (s = 0; s < 8; s++) {
			if ((crc & 0x01) != 0) {
				crc >>= 1; 
				crc ^= 0xA001; 
			} else {
				crc >>= 1;
			} 
		} 
	} 
	return crc;
} 

void printMeasurements() {
	unsigned char buf[8];

	TWI_Start();
	TWI_Send(device_addr); // 0-w, 1-r
	_delay_us(20);
	TWI_Stop();
	_delay_us(1000);
	TWI_Start();
	TWI_Send(device_addr);
	TWI_Send(0x03);
	TWI_Send(0x00);
	TWI_Send(0x04);
	TWI_Stop();
	_delay_us(20);
	TWI_Start();
	TWI_Send(device_addr + 1);
	for (int i = 0; i < 0x07; i++) buf[i] = TWI_Recive();
	buf[7] = TWI_ReciveLast();
	TWI_Stop();

	unsigned int Rcrc = (buf[7] << 8) + buf[6];

	if (Rcrc == CRC16(buf, 6)) {
		float t, h;
		unsigned int temperature = ((buf[4] & 0x7F) << 8) + buf[5];
		t = temperature / 10.0;
		t = ((buf[4] & 0x80) >> 7) == 1 ? t * (-1) : t;

		unsigned int humidity = (buf[2] << 8) + buf[3];
		h = humidity / 10.0;

		char str[50];
		sprintf(str, "{\"Humidity\": %.2f, \"Temperature\": %.2f}", h, t);
		UART_Send_Str(str);
	} else {
		UART_Send_Str("WRONG CRC");
	}
}

int main() {
	USART_Init();
	TWI_Init();
	
	while(1) {
		printMeasurements();
	   	_delay_ms(1000);
	}

	return 0;
}
