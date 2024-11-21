#ifndef RAY_H
#define RAY_H

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include "defs.h"
#include "player.h"
#include "utils.h"

typedef struct {
    float rayAngle;
    float wallHitX;
    float wallHitY;
    float distance;
    bool wasHitVertical;
    int texture;
} ray_t;

// allow access or definition / initialization to be done outside of this
extern ray_t rays[NUM_RAYS];


bool isRayFacingDown(float angle);
bool isRayFacingUp(float angle);
bool isRayFacingRight(float angle);
bool isRayFacingLeft(float angle);
void castRay(float rayAngle, int i);
void castAllRays(void);
void renderMapRays(void);

#endif