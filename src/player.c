#include "player.h"


player_t player = {
	.x = WINDOW_WIDTH,
    .y = WINDOW_HEIGHT,
    .width = 5,
    .height = 5,
    .turnDirection = 0,
    .walkDirection = 0,
    .rotationAngle = HALF_PI,    //90 deg
    .walkSpeed = 125,             // pixels per sec
    .turnSpeed = 60 * DEG2RAD // radians per sec
};

void movePlayer(float deltaTime) {
    
    player.rotationAngle += player.turnDirection * (player.turnSpeed * deltaTime);
    normalizeAngle(&player.rotationAngle);
    float moveStep = player.walkDirection * (player.walkSpeed * deltaTime);

    float newPlayerX = player.x + cos(player.rotationAngle) * moveStep;
    float newPlayerY = player.y + sin(player.rotationAngle) * moveStep;

    if(!mapHasWallAt(newPlayerX, newPlayerY)){
        player.x = newPlayerX;
        player.y = newPlayerY;
    }

}

void renderMapPlayer(void){

    // drawRect(
    //     (player.x - (player.width * 0.5)) * MINIMAP_SCALE_FACTOR,
    //     (player.y - (player.height * 0.5)) * MINIMAP_SCALE_FACTOR,
    //     player.width * MINIMAP_SCALE_FACTOR,
    //     player.height * MINIMAP_SCALE_FACTOR,
    //     0xFFFFFFFF
    // );
    drawCircle(
        (player.x - (player.width * 0.5)) * MINIMAP_SCALE_FACTOR,
        (player.y - (player.height * 0.5)) * MINIMAP_SCALE_FACTOR,
        (player.width * 2) * MINIMAP_SCALE_FACTOR,
        0xFFFFFFFF
    );
    
}