#pragma once

#include <cstdint>

#define RED (color){255,0,0,255}
#define GREEN (color){0,255,0,255}
#define BLUE (color){0,0,255,255}

struct color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};