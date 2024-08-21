#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdbool.h>
#include "math.h"

#include "color.h"
#include "vectors.h"

struct Camera{
    Vector3 position;
    float width,height;
    float zoom;
};

struct renderCopyParams
{
    SDL_Renderer *sdlrenderer;
    SDL_Texture *texture;
    SDL_Rect *sourceRect;
    SDL_Rect *destinationRect;
    double angle;
    SDL_RendererFlip flip;
    float depth;
    rgbColor color;
};

int compareByY(const void* a, const void* b) 
{ 
    return ((struct renderCopyParams*)b)->depth
           - ((struct renderCopyParams*)a)->depth; 
} 

Vector2 worldSpaceToScreenSpace(struct Camera cam, Vector3 worldSpaceCoords){
    Vector2 screenSpaceCoords = {
    ((int)worldSpaceCoords.x - cam.position.x),
    (-(int)worldSpaceCoords.y + -(int)worldSpaceCoords.z/2 + cam.position.y + cam.position.z/2),
    };
    return screenSpaceCoords;
}

Vector3 screenSpaceToWorldSpace(struct Camera cam, Vector2 screenSpaceCoords){
    Vector3 worldSpaceCoords = {
    (int)screenSpaceCoords.x + cam.position.x,
    -((int)screenSpaceCoords.y - cam.position.y),
    };
    return worldSpaceCoords;
}
