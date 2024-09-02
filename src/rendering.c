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

int compareByY(const void* a, const void* b) 
{ 
    return ((struct renderCopyParams*)b)->depth
           - ((struct renderCopyParams*)a)->depth; 
} 

Vector2 worldSpaceToScreenSpace(struct Camera cam, Vector3 worldSpaceCoords){
    cam.position.x -= cam.width/2;
    cam.position.y += cam.height/2;

    Vector2 screenSpaceCoords = {
    ((int)worldSpaceCoords.x - cam.position.x),
    (-(int)worldSpaceCoords.y + -(int)worldSpaceCoords.z/2 + cam.position.y + cam.position.z/2),
    };
    
    return screenSpaceCoords;
}

Vector3 screenSpaceToWorldSpace(struct Camera cam, Vector2 screenSpaceCoords){
    cam.position.x -= cam.width/2;
    cam.position.y += cam.height/2;

    Vector3 worldSpaceCoords = {
    (int)screenSpaceCoords.x + cam.position.x,
    -((int)screenSpaceCoords.y - cam.position.y),
    };
    return worldSpaceCoords;
}

void drawTileMap(SDL_Renderer* renderer, struct Camera camera, struct Tilemap* tilemap, Vector2 destSize,Vector2 offset){
    int mapSizeX,mapSizeY;
    SDL_QueryTexture(tilemap->textureMap, NULL, NULL, &mapSizeX, &mapSizeY);
    int tileSizeX = mapSizeX/tilemap->tileXCount;
    int tileSizeY = mapSizeY/tilemap->tileYCount;

    for (int y = 0; y < 128; y++)
    {
        for (int x = 0; x < 128; x++)
        {

            SDL_Rect sourceRect = {tileSizeX*tilemap->map[y][x].x,tileSizeY*tilemap->map[y][x].y,tileSizeX,tileSizeY};

            Vector2 destVector2 = worldSpaceToScreenSpace(camera, (Vector3){destSize.x*x +offset.x,-destSize.y*y +offset.y,0});

            SDL_RenderCopy(renderer,tilemap->textureMap,&sourceRect,&(SDL_Rect){destVector2.x,destVector2.y,destSize.x,destSize.y});        
        }
    }
    
}