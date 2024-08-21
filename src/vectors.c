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


Vector2 normalizeVector2(Vector2 vector2){
    float magnitude = sqrt(vector2.x*vector2.x + vector2.y*vector2.y)+.01;
    return (Vector2){vector2.x/magnitude,vector2.y/magnitude};
}

Vector3 normalizeVector3(Vector3 vector3){
    float magnitude = sqrt(vector3.x*vector3.x + vector3.y*vector3.y + vector3.z*vector3.z)+.01;
    return (Vector3){vector3.x/magnitude,vector3.y/magnitude,vector3.z/magnitude};
}

float distanceBetweenVector2(Vector2 v1, Vector2 v2){
    return sqrt(pow(v1.x-v2.x,2) + pow(v1.y-v2.y,2));  
}

float distanceBetweenVector3(Vector3 v1, Vector3 v2){
    return sqrt(pow(v1.x-v2.x,2) + pow(v1.y-v2.y,2) + pow(v1.z-v2.z,2) );  
}
