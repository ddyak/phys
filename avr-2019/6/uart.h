#pragma once
#include <stdint.h>

uint8_t USART_Receive();

void USART_Init();

void USART_Transmit(char data);

void UART_Send_Str(const char* str);