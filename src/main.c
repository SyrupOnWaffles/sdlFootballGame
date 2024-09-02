
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <stdbool.h>

#include "color.h"
#include "rendering.h"
#include "time.h"
#include "physics.h"
#include "vectors.h"
#include "input.h"
#include "tilemap.h"

bool isServer = true;

float deltaTime,frameRate;
int lastFrameTime = 0;

int gameIsRunning = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Joystick *controllers[4];

// textures
SDL_Texture* dino1Texture = NULL;
SDL_Texture* dino2Texture = NULL;
SDL_Texture* shadowTexture = NULL;
SDL_Texture* lennaTexture = NULL;
SDL_Texture* ballTexture = NULL;

#define WINDOW_WIDTH 1152
#define WINDOW_HEIGHT 864
int windowWidth = WINDOW_WIDTH, windowHeight = WINDOW_HEIGHT;

struct Input p1input;
struct Input p2input;

struct Camera camera;

struct Tilemap tilemap = {NULL,22,26,};

int initializeWindow(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(
        "Dino Football",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    
    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }
    
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    return false;
                }
    return true;
}

void init(void) {
    dino1Texture = IMG_LoadTexture(renderer,"resources/images/dino1.png");
    dino2Texture = IMG_LoadTexture(renderer,"resources/images/dino2.png");
    shadowTexture = IMG_LoadTexture(renderer,"resources/images/shadow.png");
    ballTexture = IMG_LoadTexture(renderer,"resources/images/ball.png");
    lennaTexture = IMG_LoadTexture(renderer,"resources/images/lenna.png");

    tilemap.textureMap = IMG_LoadTexture(renderer,"resources/images/tilemap.png");

    camera.width = WINDOW_WIDTH;
    camera.height = WINDOW_HEIGHT;
    camera.zoom = 1;

    for (int i = 0; i < 128; i++)
    {
       for (int y = 0; y < 128; y++)
        {
            int test = rand()%100;

            if(test > 95){
                tilemap.map[y][i] = (Vector2){2,12};
            }else if (test > 90)
            {
                tilemap.map[y][i] = (Vector2){1,12};
            }else{
                tilemap.map[y][i] = (Vector2){0,12};
            }
        } 
    }
    
}

void update(void) {
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
    playerMovement(&bodies[1],&p1input,deltaTime);

    ballMovement(&bodies[0],deltaTime);

    int x,y;
    SDL_GetMouseState(&x,&y);
    Vector3 mouseWorldSpace = screenSpaceToWorldSpace(camera,(Vector2){x,y});

    // camera.position = (Vector3){(bodies[1].position.x+bodies[0].position.x)/2, (bodies[1].position.y+bodies[0].position.y)/2, (bodies[1].position.z+bodies[0].position.z)/2};  
    
    Vector3 vectorBetweenPlayer = vectorBetweenVector3(camera.position,bodies[1].position);
    
    camera.position.x += vectorBetweenPlayer.x*deltaTime;
    camera.position.y += vectorBetweenPlayer.y*deltaTime;
    camera.position.z += vectorBetweenPlayer.z*deltaTime;

}

void render(void) {
    struct Body ball = bodies[0];
    struct Body player = bodies[1];
    struct renderCopyParams spriteQueue[6];

    SDL_RenderSetLogicalSize(renderer,camera.zoom * camera.width,camera.zoom * camera.height);

    // backgrouond
    SDL_SetRenderDrawColor(renderer, 200,200,200, 255);
    SDL_RenderClear(renderer);

    // drawTileMap(renderer,camera,&tilemap,(Vector2){32,32},(Vector2){-32*64,32*64});
    
    // lenna time 
    Vector3 lennaWorldSpace = {128,-128,0};
    Vector2 lennaScreenSpace = worldSpaceToScreenSpace(camera,lennaWorldSpace);
    spriteQueue[0] = (struct renderCopyParams){renderer,lennaTexture,NULL,&(SDL_Rect){lennaScreenSpace.x,lennaScreenSpace.y,128,128},0,SDL_FLIP_NONE,lennaWorldSpace.y,hsvToRgb((hsvColor){255,0,255})};
    
    //draw player 
    Vector2 playerScreenSpace = worldSpaceToScreenSpace(camera,player.position);
    spriteQueue[1] = (struct renderCopyParams){renderer,dino1Texture,&(SDL_Rect){(int)player.frame*24,0,24,24},&(SDL_Rect){playerScreenSpace.x,playerScreenSpace.y,64,64},0,SDL_FLIP_HORIZONTAL*(player.velocity.x<0),player.position.y,hsvToRgb((hsvColor){255,0,255})};
    
    // draw player shadow
    Vector3 shadowWorldSpace = {player.position.x,player.position.y+1,0};
    Vector2 shadowScreenSpace = worldSpaceToScreenSpace(camera,shadowWorldSpace);
    spriteQueue[2] = (struct renderCopyParams){renderer,shadowTexture,NULL,&(SDL_Rect){shadowScreenSpace.x,shadowScreenSpace.y+24,32,16},0,SDL_FLIP_NONE,100000,hsvToRgb((hsvColor){255,0,120+player.position.z/20})};

    // draw ball 
    Vector2 ballScreenSpace = worldSpaceToScreenSpace(camera,ball.position);
    spriteQueue[3] = (struct renderCopyParams){renderer,ballTexture,NULL,&(SDL_Rect){ballScreenSpace.x,ballScreenSpace.y+8,32,32},ball.frame,SDL_FLIP_NONE,ball.position.y,hsvToRgb((hsvColor){255,0,255})};
    
    // draw ball shadow
    shadowWorldSpace = (Vector3){ball.position.x,ball.position.y+1,0};
    shadowScreenSpace = worldSpaceToScreenSpace(camera,shadowWorldSpace);
    spriteQueue[4] = (struct renderCopyParams){renderer,shadowTexture,NULL,&(SDL_Rect){shadowScreenSpace.x,shadowScreenSpace.y+24,32,16},0,SDL_FLIP_NONE,100000,hsvToRgb((hsvColor){255,0,120+ball.position.z/20})};

    int n = sizeof(spriteQueue) / sizeof(spriteQueue[0]);
    qsort(spriteQueue, n, sizeof(struct renderCopyParams), compareByY); 

    for (int i = 0; i < n; i++)
    {
        if(spriteQueue[i].sdlrenderer == renderer){

            SDL_SetTextureColorMod(spriteQueue[i].texture,spriteQueue[i].color.r,spriteQueue[i].color.g,spriteQueue[i].color.b);
            SDL_Rect *destRect = spriteQueue[i].destinationRect;
            destRect->x -= destRect->w/2;
            destRect->y -= destRect->h/2;
            SDL_RenderCopyEx(spriteQueue[i].sdlrenderer,spriteQueue[i].texture,spriteQueue[i].sourceRect,destRect,spriteQueue[i].angle,NULL,spriteQueue[i].flip);
        }
    }

    // SDL_SetRenderDrawColor(renderer,0,0,255,255);
    // Vector2 debugLine = worldSpaceToScreenSpace(camera,(Vector3){player.position.x+player.velocity.x*2000,player.position.y+player.velocity.y*2000,player.position.z});
    // SDL_RenderDrawLine(renderer,playerScreenSpace.x,playerScreenSpace.y,debugLine.x,debugLine.y); 

    // SDL_SetRenderDrawColor(renderer,0,255,0,255);
    // debugLine = worldSpaceToScreenSpace(camera,(Vector3){ball.position.x+ball.velocity.x*2000,ball.position.y+ball.velocity.y*2000,ball.position.z+ball.velocity.z*2000});
    // SDL_RenderDrawLine(renderer,ballScreenSpace.x,ballScreenSpace.y,debugLine.x,debugLine.y); 

    SDL_RenderPresent(renderer);
}

void destroy_window(void) { 
    SDL_JoystickClose(controllers[0]);
    controllers[0] = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
}

int main(int argc, char* args[]) {
    gameIsRunning = initializeWindow();
    init();

    while (gameIsRunning) {
        deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0;
        printf("fps : %i\n",(int)(1000.0 / (SDL_GetTicks()-lastFrameTime)));

        lastFrameTime = SDL_GetTicks();
        processInput(&p1input, 0, &gameIsRunning,controllers);
        update();
        render();
    }

    destroy_window();

    return 0;
}