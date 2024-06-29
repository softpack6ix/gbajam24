#ifndef UTILS_H
#define UTILS_H

#include "bn_fixed_point.h"


// Utils
bn::fixed lerp(bn::fixed a, bn::fixed b, bn::fixed t)
{
    return a + t * (b - a);
}

int mod(int a, int b) {
    return ((a % b) + b) % b;
}




#endif