#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "defs.h"
#include "graphics.h"
#include "map.h"
#include "player.h"
#include "ray.h"
#include "sprite.h"
#include "textures.h"
#include "wall.h"

bool isGameRunning = false;
int ticksLastFrame = 0;


//color_t* wallTexture = NULL;
//color_t* textures[NUM_TEXTURES];


void setup() {
    

    // see textures.h for upng loading
    loadTextures();
    // assign textures to array
    // textures[0] = (color_t*) REDBRICK_TEXTURE;
    // textures[1] = (color_t*) PURPLESTONE_TEXTURE;
    // textures[2] = (color_t*) MOSSYSTONE_TEXTURE;
    // textures[3] = (color_t*) GRAYSTONE_TEXTURE;
    // textures[4] = (color_t*) COLORSTONE_TEXTURE;
    // textures[5] = (color_t*) BLUESTONE_TEXTURE;
    // textures[6] = (color_t*) WOOD_TEXTURE;
    // textures[7] = (color_t*) EAGLE_TEXTURE;

    //create the wall texture
    // wallTexture = (color_t*) malloc( sizeof(color_t) * (color_t)TEXTURE_WIDTH * (color_t)TEXTURE_HEIGHT);
    // for(int x =0; x < TEXTURE_WIDTH; x++){
    //     for(int y =0; y < TEXTURE_HEIGHT; y++) {
    //         wallTexture[(TEXTURE_WIDTH * y) + x] = (x % 8 && y % 8) ? 0xFF0000FF : 0xFF000000;
    //     }
    // }

}

void processInput() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type){
        case SDL_QUIT: {
            //x clicked
            isGameRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if(event.key.keysym.sym == SDLK_ESCAPE)
                isGameRunning = false;
            if(event.key.keysym.sym == SDLK_UP)
                player.walkDirection = +1;
            if(event.key.keysym.sym == SDLK_DOWN)
                player.walkDirection = -1;
            if(event.key.keysym.sym == SDLK_RIGHT)
                player.turnDirection = +1;
            if(event.key.keysym.sym == SDLK_LEFT)
                player.turnDirection = -1;
            break;
            
        }
        case SDL_KEYUP: {
            if(event.key.keysym.sym == SDLK_UP)
                player.walkDirection = 0;
            if(event.key.keysym.sym == SDLK_DOWN)
                player.walkDirection = 0;
            if(event.key.keysym.sym == SDLK_RIGHT)
                player.turnDirection = 0;
            if(event.key.keysym.sym == SDLK_LEFT)
                player.turnDirection = 0;
            break;
            
        }

    }
}



void update() {
    // waste some time until target frame time length is met
    int timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);
    
    // only delay if we are running too fast
    if(timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH){
        SDL_Delay(timeToWait);
    }

    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f; //in seconds
    ticksLastFrame = SDL_GetTicks();


    movePlayer(deltaTime);
    castAllRays();

}



void render(){
    
    clearColorBuffer(0xFF000000);

    // render walls and sprites
    //renderFloorAndCeilProjection(); //TODO: fix this its wonky
    renderWallProjection();
    renderSpriteProjection();

    // display minimap
    renderMapGrid();
    renderMapRays();
    renderMapPlayer();
    renderMapSprites();
    
    renderColorBuffer();
}


void releaseResources(){
    freeTextures();
    destroyWindow();
}


int main(){

    // TODO:
    // - [ ] magic
    isGameRunning = initializeWindow();
    setup();
    while(isGameRunning){
        processInput();
        update();
        render();
    }

    //cleanup
    releaseResources();

    return EXIT_SUCCESS; //0
}
