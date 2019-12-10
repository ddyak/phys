#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SPI_PORTX PORTB
#define SPI_DDRX DDRB

#define SPI_MISO 3 // Выход ведомого, вход ведущего
#define SPI_MOSI 2 // выход ведущего, вход ведового
#define SPI_SCK 1  // Синхронизация
#define SPI_SS 0   // Если на ведовом 0, то откликается.

// #define /DIN 2 /CLK 1 /CS 0

volatile uint16_t ms_passed = 0;

ISR(TIMER0_COMPA_vect) {
    cli();
    ms_passed += 1;
    ms_passed %= 41;
    if (ms_passed == 40)
        drawField();  
    sei();
}

ISR(INT0_vect) {
    cli();
    shoot();
    sei();
}

void SPI_Init(void)
{
    /*настройка портов ввода-вывода
   все выводы, кроме MISO выходы*/
    SPI_DDRX |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS) | (0 << SPI_MISO);
    SPI_PORTX |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS) | (1 << SPI_MISO);

    /*разрешение spi,старший бит вперед,мастер, режим 0*/
    SPCR = (1 << SPE) | (0 << DORD) | (1 << MSTR) | (0 << CPOL) | (0 << CPHA) | (0 << SPR1) | (0 << SPR0);
    SPSR = (1 << SPI2X);
}

void SPI_WriteLine(uint8_t addr, uint32_t data)
{
    SPI_PORTX &= ~(1 << SPI_SS);
    for (int i = 0; i < 4; ++i) {
        SPDR = addr;
        while (!(SPSR & (1 << SPIF)));
    
        SPDR = (data >> (i * 8));
        while (!(SPSR & (1 << SPIF)));
    }
    SPI_PORTX |= (1 << SPI_SS);
}


void ADC_Init() {
 	ADMUX |= (1<<REFS0) | (1<<ADLAR);
 	ADCSRA |= (1<<ADSC) | (1<<ADEN) | (1<<ADATE);
}

void MAX7219_Init() {
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

void clear() {
    for (int i = 1; i <= 8; ++i)
        SPI_WriteLine(i, 0);  
}

typedef struct {
    uint32_t x;
    uint32_t y;
} shot; 

typedef struct {
    uint8_t shot_counter;
    shot shots[10];

    int ship;
} field;

field Field;

void shoot() {
    shot piu;
    piu.x = Field.ship;
    piu.y = 0x02000000;
    
    Field.shot_counter += 1;
    Field.shot_counter %= 10;

    Field.shots[Field.shot_counter] = piu;
}

uint32_t upd(uint32_t line) {
    if (line & 0x00000080) {
        line = 0;
    } 
    
    if (line & 0x80808000) 
        line >>= 15;
    else
        line <<= 1;
    return line;
}

void drawField() 
{
    uint32_t field[8];
    for (int i = 0; i < 8; ++i)
        field[i] = 0;
    for (int i = 0; i < 10; ++i) {
        Field.shots[i].y = upd(Field.shots[i].y);
        field[Field.shots[i].x] |= Field.shots[i].y;
    }
    field[Field.ship - 1] |= 0x01000000;
    field[Field.ship]     |= 0x03000000;
    field[Field.ship + 1] |= 0x01000000;
    
    clear();
    for (int i = 0; i < 8; ++i)
        SPI_WriteLine(i + 1, field[i]);
}

int main() 
{
	EICRA = (0 << ISC00) | (1 << ISC01);
	EIMSK = (1 << INT0);

    ADC_Init();
    SPI_Init();
    MAX7219_Init();
    TIMER0_Init();
    clear();

    sei();
    
    while(1) {
    	uint8_t high = ADCH;
        uint8_t x = high / 32;
        Field.ship = x;
    }
    
}
