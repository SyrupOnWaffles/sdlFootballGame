#ifndef PHYSICS_H
#define PHYSICS_H

#include "vectors.h"

struct Body{
    char name[8];
    Vector3 position;
    Vector3 velocity;
    float frame;
};


#endif