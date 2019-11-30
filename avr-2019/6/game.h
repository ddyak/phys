#pragma once

#include <stdint.h>

void clear();

typedef struct {
    uint32_t x;
    uint32_t y;
} shot; 

typedef struct {
    uint8_t shot_counter;
    shot shots[10];

    int ship;
} field;

void shoot();

uint32_t upd(uint32_t line);

void drawField();