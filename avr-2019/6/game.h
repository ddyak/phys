#pragma once

#include <stdint.h>

typedef struct {
    uint32_t x;
    uint32_t y;
} meteor;

typedef struct {
    uint32_t x;
    uint32_t y;
} shot;

typedef struct {
    uint8_t shot_counter;
    shot shots[10];
    uint8_t meteor_counter;
    meteor meteors[32];
    
    int ship;
} field;

void FIELD_Init();

void shoot();

void generate_meteor();

int drawField();

void updateField();
