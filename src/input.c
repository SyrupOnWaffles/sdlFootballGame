#include "vectors.h"
#include "SDL2/SDL.h"
struct Input{
    bool w,a,s,d,space;
    Vector2 direction;
    Vector2 hitDirection;
};


void processInput(struct Input* input, uint8_t inputNumber, int *gameEndSignal, SDL_Joystick** controllers) {  
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
                case SDL_QUIT:
                    *gameEndSignal = false;
                    break;
                case SDL_KEYDOWN:
                    if(inputNumber==0){
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            *gameEndSignal = false;
                            break;
                        case SDLK_w:
                            input->w = true;
                            break;
                        case SDLK_a:
                            input->a = true;
                            break;
                        case SDLK_s:
                            input->s = true;
                            break;
                        case SDLK_d:
                            input->d = true;
                            break;
                        case SDLK_SPACE:
                            input->space = true;
                            break;
                        }
                    }
                    break;
                case SDL_KEYUP:
                    if(inputNumber==0){
                    switch (event.key.keysym.sym){
                        case SDLK_w:
                            input->w = false;
                            break;
                        case SDLK_a:
                            input->a = false;
                            break;
                        case SDLK_s:
                            input->s = false;
                            break;
                        case SDLK_d:
                            input->d = false;
                            break;
                        case SDLK_SPACE:
                            input->space = false;
                            break;
                    }
                    }
                    break;
                case SDL_JOYAXISMOTION:
                    if(inputNumber==1){
                        if( event.jaxis.axis == 0 )
                        {
                            float val = (float)event.jaxis.value/32767;
                            if(fabs(val)>.05){
                                input->direction.x = val;

                            }else{
                                input->direction.x = 0;
                            }
                        }
                        if( event.jaxis.axis == 1 )
                        {
                            float val = -(float)event.jaxis.value/32767;
                            if(fabs(val)>.05){
                                input->direction.y = val;

                            }else{
                                input->direction.y = 0;
                            }
                        }
                        }
                break;
            
            case SDL_JOYDEVICEADDED:
            
                printf("connected %i\n",event.jdevice.which);
                controllers[event.jdevice.which] = SDL_JoystickOpen(event.jdevice.which);
                break;
            case SDL_JOYDEVICEREMOVED:
                printf("disconnected %i\n",event.jdevice.which);
                controllers[event.jdevice.which] = NULL;
                break;
            };

        }
    if(inputNumber==0){
        input->direction = (Vector2){input->d + -input->a,input->w + -input->s};
        input->direction = normalizeVector2(input->direction);
    }else{
        input->space = SDL_JoystickGetButton(controllers[0],0);
    }
}
