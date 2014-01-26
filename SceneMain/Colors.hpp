#ifndef COLORS_HPP
#define COLORS_HPP
#include "commons.hpp"

enum Color {
    WHITE = 0,
    RED,
    GREEN,
    BLUE,
    NUM_COLORS
};

vec3f getColor(Color col);


#endif // COLORS_HPP
