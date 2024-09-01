#ifndef INPUT_H
#define INPUT_H

#include "vectors.h"
#include "SDL2/SDL.h"

struct Input{
    bool w,a,s,d,space;
    Vector2 direction;
    Vector2 hitDirection;
};


void processInput(struct Input* input, uint8_t inputNumber, int *gameEndSignal, SDL_Joystick** controllers);

#endif