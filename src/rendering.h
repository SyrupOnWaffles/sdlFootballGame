#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdbool.h>
#include "math.h"

#include "color.h"
#include "vectors.h"
#include "tilemap.h"

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

int compareByY(const void* a, const void* b); 

Vector2 worldSpaceToScreenSpace(struct Camera cam, Vector3 worldSpaceCoords);

Vector3 screenSpaceToWorldSpace(struct Camera cam, Vector2 screenSpaceCoords);

void drawTileMap(SDL_Renderer* renderer, struct Camera camera, struct Tilemap* tilemap, Vector2 destSize, Vector2 offset);

#endif