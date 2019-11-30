#include "game.h"
#include "spi.h"

extern field Field;

void clear() {
    for (int i = 1; i <= 8; ++i)
        SPI_WriteLine(i, 0);  
}

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