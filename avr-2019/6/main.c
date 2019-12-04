#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <stdlib.h>

#include "spi.h"
#include "game.h"

volatile uint16_t ms_passed = 0;
volatile field Field;
volatile int gameOverFlag = 0;

ISR(TIMER0_COMPA_vect) {
    cli();
    ms_passed += 1;
    ms_passed %= 41;
    if (ms_passed == 40) {
        generate_meteor();
        gameOverFlag = drawField();
        if (!gameOverFlag)
            updater();
    }
    sei();
}

ISR(INT0_vect) {
    cli();
    shoot();
    sei();
}

void ADC_Init() {
 	ADMUX  |= (1 << REFS0) | (1 << ADLAR);
 	ADCSRA |= (1 << ADSC)  | (1 << ADEN) | (1 << ADATE);
}

void MAX7219_Init() {
    for (int i = 0; i < 15; ++i) {
        SPI_WriteLine(i, 0x00000000);
    }
    SPI_WriteLine(0x09, 0x00000000);
    SPI_WriteLine(0x0A, 0x0F0F0F0F);
    SPI_WriteLine(0x0B, 0x07070707);
    SPI_WriteLine(0x0C, 0x01010101);
}

void TIMER0_Init() {
    TCNT0 = 0;
    OCR0A = 62;
    TCCR0B = (1 << CS01) | (1 << CS00);
    TCCR0A = (1 << WGM01);
    TIMSK0 = (1 << OCIE0A);
}

void BUTTON_Init() {
    EICRA = (0 << ISC00) | (1 << ISC01);
	EIMSK = (1 << INT0);
}

int main() {
	ADC_Init();
    SPI_Init();
    MAX7219_Init();
    TIMER0_Init();
    BUTTON_Init();
    sei();

    FIELD_Init();

    while(1) {
        if (gameOverFlag == 1) break;
    	uint8_t high = ADCH;
        uint8_t x = high / 32;
        if ((high % 32) > 4 && (high % 32) < 28)
            Field.ship = x;
    }
    cli();
    // sound and UART instraction;
}
