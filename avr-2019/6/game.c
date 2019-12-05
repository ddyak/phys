#include "spi.h"
#include "game.h"


extern field Field;
int splitter = 0;

const uint8_t MAX_SHOTS = 10;
const uint8_t MAX_METEORS = 32;
const uint8_t SPEED_METEOR_BY_SHOT = 20;

void clear() {
    for (int i = 1; i <= 8; ++i)
        SPI_WriteLine(i, 0);
}

void FIELD_Init() {
    for (int i = 0; i < MAX_SHOTS; ++i) {
        Field.shots[i].x = 0;
        Field.shots[i].y = 0;
    }
    
    for (int i = 0; i < MAX_METEORS; ++i) {
        Field.meteors[i].x = 0;
        Field.meteors[i].y = 0;
    }
}

void shoot() {
    shot piu;
    piu.x = Field.ship;
    piu.y = 0x02000000;
    
    Field.shot_counter += 1;
    Field.shot_counter %= MAX_SHOTS;

    Field.shots[Field.shot_counter] = piu;
}

void generate_meteor() {
    if (rand() % 52) return;

    meteor met;
    met.x = rand() % 8;
    met.y = 0x00000080;
    
    Field.meteor_counter += 1;
    Field.meteor_counter %= MAX_METEORS;

    Field.meteors[Field.meteor_counter] = met;
}

uint32_t upd_shot(uint32_t line) {
    if (line & 0x00000080) {
        line = 0;
    } 
    
    if (line & 0x80808000) 
        line >>= 15;
    else
        line <<= 1;
    return line;
}

uint32_t upd_meteor(uint32_t line) {
    if (line & 0x01000000) {
        line = 0;
    }
    if (line & 0x00010101) 
        line <<= 15;
    else
        line >>= 1;
    return line;
}

int isCollision(meteor met, shot piu) {
    if (met.x == piu.x) {
        return (met.y != 0) && ((met.y == piu.y) || (upd_meteor(met.y) == piu.y));
    }
    return 0;
}

int isGameOver(meteor met) {
    if (met.y == 0x02000000 && Field.ship == met.x)
        return 1;
    if (met.y == 0x01000000 && (Field.ship == (met.x - 1) || Field.ship == met.x || Field.ship == met.x + 1)) 
        return 1;
    return 0;
}

void updateField() {
    if (splitter++ % SPEED_METEOR_BY_SHOT == 0) {
        for (int i = 0; i < MAX_METEORS; ++i) {
            Field.meteors[i].y = upd_meteor(Field.meteors[i].y);
        }
    }

    for (int i = 0; i < MAX_SHOTS; ++i) {
        Field.shots[i].y = upd_shot(Field.shots[i].y);
    }
    
    for (int i = 0; i < MAX_SHOTS; ++i) {
        for (int j = 0; j < MAX_METEORS; ++j) {
            if (isCollision(Field.meteors[j], Field.shots[i])) {
                Field.meteors[j].y = 0;
                Field.shots[i].y = 0;
            }
        }
    }
}

int drawField() 
{
    uint32_t field[8];
    for (int i = 0; i < 8; ++i) {
        field[i] = 0;
    }

    for (int i = 0; i < MAX_SHOTS; ++i) {
        field[Field.shots[i].x] |= Field.shots[i].y;
    }
    for (int i = 0; i < MAX_METEORS; ++i) {
        field[Field.meteors[i].x] |= Field.meteors[i].y;
    }
    field[Field.ship - 1] ^= 0x01000000;
    field[Field.ship]     ^= 0x03000000;
    field[Field.ship + 1] ^= 0x01000000;
    
    clear();
    for (int i = 0; i < 8; ++i) {
        SPI_WriteLine(i + 1, field[i]);
    }
    
    for (int i = 0; i < MAX_METEORS; ++i) {
        if (isGameOver(Field.meteors[i]))
            return 1;
    }

    return 0;
}
