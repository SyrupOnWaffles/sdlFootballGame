#ifndef VECTORS_H
#define VECTORS_H

#include <stdio.h>
#include <stdbool.h>
#include "math.h"

typedef struct Vector2
{
    float x,y;
} Vector2;

typedef struct Vector3
{
    float x,y,z;
} Vector3;


Vector2 normalizeVector2(Vector2 vector2);

Vector3 normalizeVector3(Vector3 vector3);

float distanceBetweenVector2(Vector2 v1, Vector2 v2);

float distanceBetweenVector3(Vector3 v1, Vector3 v2);

Vector2 vectorBetweenVector2(Vector2 v1, Vector2 v2);

Vector3 vectorBetweenVector3(Vector3 v1, Vector3 v2);

#endif