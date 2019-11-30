#include <avr/io.h>

#define SPI_PORTX PORTB
#define SPI_DDRX DDRB

#define SPI_MISO 3 // Выход ведомого, вход ведущего
#define SPI_MOSI 2 // выход ведущего, вход ведового 2
#define SPI_SCK 1  // Синхронизация 1
#define SPI_SS 0   // Если на ведовом 0, то откликается. 0

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
