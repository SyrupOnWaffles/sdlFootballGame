#include <SDL2/SDL_image.h>
#include "vectors.h"

struct Tilemap
{
    SDL_Texture* textureMap;
    int tileXCount;
    int tileYCount;

    Vector2 map[128][128];
};