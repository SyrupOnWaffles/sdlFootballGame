
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdbool.h>

#include "color.h"
#include "rendering.h"
#include "time.h"
#include "physics.h"
#include "vectors.h"
#include "input.h"

float deltaTime,frameRate;
int lastFrameTime = 0;

int gameIsRunning = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Joystick *controller1;

SDL_Texture* dino1Texture = NULL;
SDL_Texture* dino2Texture = NULL;
SDL_Texture* shadowTexture = NULL;
SDL_Texture* lennaTexture = NULL;
SDL_Texture* ballTexture = NULL;


#define WINDOW_WIDTH 1152
#define WINDOW_HEIGHT 864
int windowWidth = WINDOW_WIDTH, windowHeight = WINDOW_HEIGHT;

struct Body bodies[3] = {
    {"ball",{0,0,0}}, //ball
    {"p1",{-500,0,0}}, //player 
    {"p2",{500,0,0}}, //player 
};
struct Input p1input;
struct Input p2input;
struct Camera camera;

void playerMovement(struct Body *player,struct Input *input){
    // player movement
    float groundSlowDownSpeed = 6;
    float groundAccel = .2;
    float groundFriction = .3;
    float jumpForce = .1;

    if(player->position.z <= 0){
        player->position.z = 0;
        player->velocity.z *= -groundFriction;
        if(input->space == true){
            player->velocity.z = jumpForce;
        }
    }
    player->velocity.z -= deltaTime * .2;

    player->velocity.y += input->direction.y * deltaTime * groundAccel;
    player->velocity.x += input->direction.x * deltaTime * groundAccel;

    player->velocity.x /= 1 + deltaTime * groundSlowDownSpeed;
    player->velocity.y /= 1 + deltaTime * groundSlowDownSpeed;


    float avgVelo = (fabs(player->velocity.x)+fabs(player->velocity.y))/2;
    if(avgVelo>.001){
        player->frame += deltaTime * 10;
        if(player->frame>10){
            player->frame = 4;
        }
    }else{
        player->frame += deltaTime * 5;
        if(player->frame>4){
            player->frame = 0;
        }
    }

    player->position.y += player->velocity.y;
    player->position.x += player->velocity.x;
    player->position.z += player->velocity.z;
}

void ballMovement(struct Body *ball){
    float groundSlowDownSpeed = 6;
    float airSlowDownSpeed = 1;
    float groundFriction = 2;
    bool kickLock = false;
    
    for (int i = 0; i < sizeof(bodies) / sizeof(bodies[0]); i++)
    {
        if(&bodies[i]!=ball){
            float dist = distanceBetweenVector3(bodies[i].position,ball->position);
            if(dist < 24 && kickLock==false){
                // Vector3 hitVector = {input.hitDirection.x/10,input.hitDirection.y/10,player.velocity.z+.1};
                Vector3 hitVector = {bodies[i].velocity.x*4,bodies[i].velocity.y*2,.09 + (bodies[i].velocity.x +  bodies[i].velocity.y)/2};
                ball->velocity = hitVector;
                kickLock = true;
            }
            if(dist > 100){
                kickLock= false;
            }
        }
    }
    


    if(ball->position.z <= 0){
        ball->position.z = 0;
        ball->velocity.z /= -groundFriction;
    }

    if((int)ball->position.z==0){
        ball->velocity.x /= 1 + deltaTime * groundSlowDownSpeed;
        ball->velocity.y /= 1 + deltaTime * groundSlowDownSpeed;
    }else{
        ball->velocity.x /= 1 + deltaTime * airSlowDownSpeed;
        ball->velocity.y /= 1 + deltaTime * airSlowDownSpeed;
    }
    ball->velocity.z -= deltaTime * .2;

    ball->position.y += ball->velocity.y;
    ball->position.x += ball->velocity.x;
    ball->position.z += ball->velocity.z;
}

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

    return true;
}

void processInput(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                gameIsRunning = false;
                break;
                case SDL_JOYAXISMOTION:
                            if( event.jaxis.axis == 0 )
                            {
                                float val = (float)event.jaxis.value/32767;
                                if(fabs(val)>.05){
                                    p2input.direction.x = val;

                                }else{
                                    p2input.direction.x = 0;
                                }
                            }
                            if( event.jaxis.axis == 1 )
                            {
                                float val = -(float)event.jaxis.value/32767;
                                if(fabs(val)>.05){
                                    p2input.direction.y = val;

                                }else{
                                    p2input.direction.y = 0;
                                }
                            }

                break;
            
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        gameIsRunning = false;
                        break;
                    case SDLK_w:
                        p1input.w = true;
                        break;
                    case SDLK_a:
                        p1input.a = true;
                        break;
                    case SDLK_s:
                        p1input.s = true;
                        break;
                    case SDLK_d:
                        p1input.d = true;
                        break;
                    case SDLK_SPACE:
                        p1input.space = true;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym){
                    case SDLK_w:
                        p1input.w = false;
                        break;
                    case SDLK_a:
                        p1input.a = false;
                        break;
                    case SDLK_s:
                        p1input.s = false;
                        break;
                    case SDLK_d:
                        p1input.d = false;
                        break;
                    case SDLK_SPACE:
                        p1input.space = false;
                        break;
                }
                break;
            case SDL_JOYDEVICEADDED:
                printf("connected\n");
                controller1 = SDL_JoystickOpen(0);
                break;
            case SDL_JOYDEVICEREMOVED:
                printf("disconnected\n");
                controller1 = NULL;
                break;
            };

        }
    p1input.direction = (Vector2){p1input.d + -p1input.a,p1input.w + -p1input.s};
    p1input.direction = normalizeVector2(p1input.direction);
    p2input.space = SDL_JoystickGetButton(controller1,0);
}

void init(void) {
    dino1Texture = IMG_LoadTexture(renderer,"resources/dino1.png");
    dino2Texture = IMG_LoadTexture(renderer,"resources/dino2.png");
    shadowTexture = IMG_LoadTexture(renderer,"resources/shadow.png");
    ballTexture = IMG_LoadTexture(renderer,"resources/ball.png");
    lennaTexture = IMG_LoadTexture(renderer,"resources/lenna.png");

    camera.width = WINDOW_WIDTH;
    camera.height = WINDOW_HEIGHT;
    camera.zoom = 1;
}

void update(void) {
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    playerMovement(&bodies[1],&p1input);

    playerMovement(&bodies[2],&p2input);

    ballMovement(&bodies[0]);

    int x,y;
    SDL_GetMouseState(&x,&y);
    Vector3 mouseWorldSpace = screenSpaceToWorldSpace(camera,(Vector2){x,y});

    camera.position = (Vector3){(bodies[1].position.x+bodies[2].position.x)/2, (bodies[1].position.y+bodies[2].position.y)/2, (bodies[1].position.z+bodies[2].position.z)/2};  
    camera.position.x -= camera.width/2;
    camera.position.y += camera.height/2;

    bodies[0].frame += (bodies[0].velocity.x+bodies[0].velocity.y)/2*2.5;
}

void render(void) {
    struct Body ball = bodies[0];
    struct Body player = bodies[1];
    struct Body player2 = bodies[2];
    struct renderCopyParams spriteQueue[8];

    SDL_RenderSetLogicalSize(renderer,camera.zoom * camera.width,camera.zoom * camera.height);

    // backgrouond
    SDL_SetRenderDrawColor(renderer, 200,200,200, 255);
    SDL_RenderClear(renderer);
    
    // lenna time 
    Vector3 lennaWorldSpace = {128,-128,0};
    Vector2 lennaScreenSpace = worldSpaceToScreenSpace(camera,lennaWorldSpace);
    spriteQueue[(0)] = (struct renderCopyParams){renderer,lennaTexture,NULL,&(SDL_Rect){lennaScreenSpace.x,lennaScreenSpace.y,128,128},0,SDL_FLIP_NONE,lennaWorldSpace.y,hsvToRgb((hsvColor){255,0,255})};
    
    //draw player 
    Vector2 playerScreenSpace = worldSpaceToScreenSpace(camera,player.position);
    spriteQueue[1] = (struct renderCopyParams){renderer,dino1Texture,&(SDL_Rect){(int)player.frame*24,0,24,24},&(SDL_Rect){playerScreenSpace.x,playerScreenSpace.y,64,64},0,SDL_FLIP_HORIZONTAL*(player.velocity.x<0),player.position.y,hsvToRgb((hsvColor){255,0,255})};
    
    // draw player shadow
    Vector3 shadowWorldSpace = {player.position.x,player.position.y+1,0};
    Vector2 shadowScreenSpace = worldSpaceToScreenSpace(camera,shadowWorldSpace);
    spriteQueue[2] = (struct renderCopyParams){renderer,shadowTexture,NULL,&(SDL_Rect){shadowScreenSpace.x,shadowScreenSpace.y+24,32,16},0,SDL_FLIP_NONE,100000,hsvToRgb((hsvColor){255,0,120+player.position.z/20})};

    //draw player 2
    Vector2 player2ScreenSpace = worldSpaceToScreenSpace(camera,player2.position);
    spriteQueue[5] = (struct renderCopyParams){renderer,dino2Texture,&(SDL_Rect){(int)player2.frame*24,0,24,24},&(SDL_Rect){player2ScreenSpace.x,player2ScreenSpace.y,64,64},0,SDL_FLIP_HORIZONTAL*(player2.velocity.x<0),player2.position.y,hsvToRgb((hsvColor){255,0,255})};
    
    // draw player 2 shadow
    shadowWorldSpace = (Vector3){player2.position.x,player2.position.y+1,0};
    shadowScreenSpace = worldSpaceToScreenSpace(camera,shadowWorldSpace);
    spriteQueue[6] = (struct renderCopyParams){renderer,shadowTexture,NULL,&(SDL_Rect){shadowScreenSpace.x,shadowScreenSpace.y+24,32,16},0,SDL_FLIP_NONE,100000,hsvToRgb((hsvColor){255,0,120+player2.position.z/20})};


    // draw ball 
    Vector2 ballScreenSpace = worldSpaceToScreenSpace(camera,ball.position);
    spriteQueue[3] = (struct renderCopyParams){renderer,ballTexture,NULL,&(SDL_Rect){ballScreenSpace.x,ballScreenSpace.y,32,32},ball.frame,SDL_FLIP_NONE,ball.position.y,hsvToRgb((hsvColor){255,0,255})};
    

    // draw ball shadow
    shadowWorldSpace = (Vector3){ball.position.x,ball.position.y+1,0};
    shadowScreenSpace = worldSpaceToScreenSpace(camera,shadowWorldSpace);
    spriteQueue[4] = (struct renderCopyParams){renderer,shadowTexture,NULL,&(SDL_Rect){shadowScreenSpace.x,shadowScreenSpace.y+16,32,16},0,SDL_FLIP_NONE,100000,hsvToRgb((hsvColor){255,0,120+ball.position.z/20})};
    

    int n = sizeof(spriteQueue) / sizeof(spriteQueue[0]);
    qsort(spriteQueue, n, sizeof(struct renderCopyParams), compareByY); 
    // qsort(sprintf, n, sizeof(spriteQueue[0]), compare);
    for (int i = 0; i < sizeof(spriteQueue) / sizeof(spriteQueue[0]); i++)
    {
        if(spriteQueue[i].sdlrenderer == renderer){

            SDL_SetTextureColorMod(spriteQueue[i].texture,spriteQueue[i].color.r,spriteQueue[i].color.g,spriteQueue[i].color.b);
            SDL_Rect *destRect = spriteQueue[i].destinationRect;
            destRect->x -= destRect->w/2;
            destRect->y -= destRect->h/2;
            SDL_RenderCopyEx(spriteQueue[i].sdlrenderer,spriteQueue[i].texture,spriteQueue[i].sourceRect,destRect,spriteQueue[i].angle,NULL,spriteQueue[i].flip);
        }
    }

    SDL_SetRenderDrawColor(renderer,0,0,255,255);
    Vector2 debugLine = worldSpaceToScreenSpace(camera,(Vector3){player.position.x+player.velocity.x*2000,player.position.y+player.velocity.y*2000,player.position.z});
    SDL_RenderDrawLine(renderer,playerScreenSpace.x,playerScreenSpace.y,debugLine.x,debugLine.y); 

    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    debugLine = worldSpaceToScreenSpace(camera,(Vector3){ball.position.x+ball.velocity.x*2000,ball.position.y+ball.velocity.y*2000,ball.position.z+ball.velocity.z*2000});
    SDL_RenderDrawLine(renderer,ballScreenSpace.x,ballScreenSpace.y,debugLine.x,debugLine.y); 
    
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    debugLine = worldSpaceToScreenSpace(camera,(Vector3){player.position.x+p1input.hitDirection.x*600,player.position.y+p1input.hitDirection.y*600,player.position.z});
    SDL_RenderDrawLine(renderer,playerScreenSpace.x,playerScreenSpace.y,debugLine.x,debugLine.y); 

    SDL_RenderPresent(renderer);
}

void destroy_window(void) {
    SDL_JoystickClose(controller1);
    controller1 = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
}

int main(int argc, char* args[]) {
    struct timespec ts;

    gameIsRunning = initializeWindow();
    init();

    while (gameIsRunning) {
        timespec_get(&ts, TIME_UTC);
        long startTime = ts.tv_nsec;

        deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0;
        lastFrameTime = SDL_GetTicks();

        processInput();
        update();
        render();

        timespec_get(&ts, TIME_UTC);
        frameRate = 1000000000 / (ts.tv_nsec - startTime);
    }

    destroy_window();

    return 0;
}