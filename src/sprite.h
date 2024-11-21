#ifndef SPRITE_H
#define SPRITE_H

#include <math.h>
#include <stdbool.h>
#include "defs.h"
#include "graphics.h"
#include "player.h"
#include "ray.h"
#include "textures.h"
#include "upng.h"
#include "utils.h"

typedef struct {
    float x;
    float y;
    float distance;
    float angle;
    bool visible;
    int texture; // index of the texture in the texture map
} sprite_t;


void renderSpriteProjection(void);
void renderMapSprites(void);

#endif