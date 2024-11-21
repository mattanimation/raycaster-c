#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include <stdbool.h>
#include "defs.h"
#include "graphics.h"
#include "map.h"
#include "utils.h"

typedef struct {
    float x;
    float y;
    float width;
    float height;
    int turnDirection;   // -1 for left +1 for right
    int walkDirection;   // -1 for back +1 for front
    float rotationAngle;
    float walkSpeed;
    float turnSpeed;     //angles per sec
} player_t;

extern player_t player;

float deg2rad(int deg);
void movePlayer(float deltaTime);
void renderMapPlayer(void);

#endif