#ifndef PHYSICS_H
#define PHYSICS_H

#include "vectors.h"
#include "input.h"

struct Body{
    char name[8];
    Vector3 position;
    Vector3 velocity;
    float frame;
};
extern struct Body bodies[2];
void playerMovement(struct Body *player,struct Input *input, float deltaTime);
void ballMovement(struct Body *ball, float deltaTime);
#endif