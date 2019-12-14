#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

void USART_Init() {
	UBRR0H = 0;
	UBRR0L = 103;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (0 << TXCIE0);
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
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

void i2cInit(){
	TWBR = 8;
	TWSR = (0 << TWPS1) | (0 << TWPS0);
}

void i2cStop(){
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2cStart(){
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

char i2cReciveLast() {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

char i2cRecive(){
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

void i2cSend(char send){
	TWDR = send;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void i2cSendStr(const char* str) {
	unsigned char i = 0;
	while (str[i] != '\0') {
		i2cSend(str[i]);
		i++;
	}
}

unsigned int CRC16(unsigned char *ptr, unsigned char length) 
{ 
      unsigned int crc = 0xFFFF; 
      uint8_t s = 0x00; 

      while(length--) {
        crc ^= *ptr++; 
        for(s = 0; s < 8; s++) {
          if((crc & 0x01) != 0) {
            crc >>= 1; 
            crc ^= 0xA001; 
          } else crc >>= 1; 
        } 
      } 
      return crc; 
} 

// const char device_addr = 0b10100010;
const char device_addr = 0xB8;

int getTem() {
	unsigned char buf[8];
	for(int s = 0; s < 8; s++) buf[s] = 0x02;

	i2cStart();
	i2cSend(device_addr); // 0-w, 1-r
	i2cSend(0x03); // Function code - read data//low temp
	i2cSend(0x00);
	i2cSend(0x04);
	i2cStop();
		
	_delay_ms(0.01);

	i2cStart();
	i2cSend(device_addr + 1); // 0-w, 1-r

	for (int i = 0; i < 0x08; i++) {
		if (i == 7) 
			buf[i] = i2cReciveLast();
		else 
			buf[i] = i2cReciveLast();
	}	
	i2cStop();

	char* str = "0000000000000000000000000000000000000000000000000000";
	sprintf(str, "%d, %d, %d, %d, %d, %d, %d, %d", 
	buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
	UART_Send_Str(str);

	// unsigned int Rcrc = buf[7] << 8;
	
	// float t, h;
	
	// Rcrc += buf[6];

	// // if (Rcrc == CRC16(buf, 6)) {
	// 	unsigned int temperature = ((buf[4] & 0x7F) << 8) + buf[5];
	// 	t = temperature / 10.0;
	// 	t = ((buf[4] & 0x80) >> 7) == 1 ? t * (-1) : t;

	// 	unsigned int humidity = (buf[2] << 8) + buf[3];
	// 	h = humidity / 10.0;
	// 	char* str = "0000000000000000000000000000000000000000000";
	// 	sprintf(str, "{%d, %d, %d, %d, %d, %d, %d, %d}", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
	// 	UART_Send_Str(str);
	// } else {
		// UART_Send_Str("WRONG CRC");
	// }
}

void main(void) {
	USART_Init();
	i2cInit();
	
	while(1) {
		// UART_Send_Str("New cirle");
		getTem();
	   	_delay_ms(1000);
	}
}