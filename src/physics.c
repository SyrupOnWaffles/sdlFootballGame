#include "vectors.h"
#include "input.h"

struct Body{
    char name[8];
    Vector3 position;
    Vector3 velocity;
    float frame;
};

struct Body bodies[2] = {
    {"ball",{0,0,1000}}, //ball
    {"p1",{-500,0,0}}, //player 
};

void playerMovement(struct Body *player,struct Input *input, float deltaTime){
    // player movement
    float groundSlowDownSpeed = 6;
    float groundAccel = .2;
    float groundFriction = .2;
    float jumpForce = .1;

    if(player->position.z <= 0){
        player->position.z = 0.01;
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

    player->position.y += player->velocity.y * deltaTime * 10000;
    player->position.x += player->velocity.x * deltaTime * 10000;
    player->position.z += player->velocity.z * deltaTime * 10000;
}

void ballMovement(struct Body *ball, float deltaTime){
    float groundSlowDownSpeed = 3;
    float airSlowDownSpeed = 1;
    float groundFriction = .5;
    bool kickLock = false;
    
    for (int i = 0; i < sizeof(bodies) / sizeof(bodies[0]); i++)
    {
        if(&bodies[i]!=ball){
            float dist = distanceBetweenVector3(bodies[i].position,ball->position);
            if(dist < 24 && kickLock==false){
                // Vector3 hitVector = {input.hitDirection.x/10,input.hitDirection.y/10,player.velocity.z+.1};
                Vector3 hitVector = {bodies[i].velocity.x*4,bodies[i].velocity.y*2,bodies[i].velocity.z+.1};
                ball->velocity = hitVector;
                kickLock = true;
                
            }
            if(dist > 100){
                kickLock= false;
            }
        }
    }

    if(ball->position.z <= 0){
        ball->position.z = .01;
        ball->velocity.z *= -groundFriction;
        ball->velocity.x *= 1 + deltaTime * groundSlowDownSpeed;
        ball->velocity.y *= 1 + deltaTime * groundSlowDownSpeed;
    }
    else{
        ball->velocity.x /= 1 + deltaTime * airSlowDownSpeed;
        ball->velocity.y /= 1 + deltaTime * airSlowDownSpeed;

    }
    ball->velocity.z -= deltaTime * .2;

    ball->position.y += ball->velocity.y * deltaTime * 10000;
    ball->position.x += ball->velocity.x * deltaTime * 10000;
    ball->position.z += ball->velocity.z * deltaTime * 10000;


    ball->frame += (ball->velocity.x+ball->velocity.y)/2*2.5 * deltaTime * 10000;
}
