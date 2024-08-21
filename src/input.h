#ifndef INPUT_H
#define INPUT_H

#include "vectors.h"

struct Input{
    bool w,a,s,d,space;
    Vector2 direction;
    Vector2 hitDirection;
};

#endif