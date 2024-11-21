#include "sprite.h"

#define NUM_SPRITES 6

static sprite_t sprites[NUM_SPRITES] = {
    { .x = 100, .y = 100, .texture = 9 }, //barrel
    { .x = 150, .y = 200, .texture = 9 }, //barrel
    { .x = 300, .y = 400, .texture = 10 }, //light
    { .x = 300, .y = 400, .texture = 11 }, //table
    { .x = 600, .y = 400, .texture = 12 }, //guard
    { .x = 650, .y = 450, .texture = 13 }, //armror
};

void renderSpriteProjection(void) {
    sprite_t visibleSprites[NUM_SPRITES];
    int numVisibleSprites = 0;

    for(int i = 0; i < NUM_SPRITES; i++){
        float angleSpritePlayer = player.rotationAngle - atan2(sprites[i].y - player.y, sprites[i].x - player.x);

        // make sure angle is always between 0 - 180
        if(angleSpritePlayer > PI){
            angleSpritePlayer -= TWO_PI;
        }
        if(angleSpritePlayer < -PI){
            angleSpritePlayer += TWO_PI;
        }
        angleSpritePlayer = fabs(angleSpritePlayer);

        // render if the angle is in range of fov
        // but also add some padding to that range so that the sprite
        // doesn't just disappear
        const float EPSILON = 0.2;
        if(angleSpritePlayer < HALF_FOV_ANGLE + EPSILON){
            sprites[i].visible = true;
            sprites[i].angle = angleSpritePlayer;
            sprites[i].distance = distanceBetweenPoints(sprites[i].x, sprites[i].y, player.x, player.y);
            visibleSprites[numVisibleSprites] = sprites[i];
            numVisibleSprites++;
        } else {
            sprites[i].visible = false;
        }
    }

    // sort sprites by distance
    for(int i=0; i < numVisibleSprites - 1; i++){
        for(int j = i +1; j < numVisibleSprites; j++){
            if(visibleSprites[i].distance < visibleSprites[j].distance){
                sprite_t temp = visibleSprites[i];
                visibleSprites[i] = visibleSprites[j];
                visibleSprites[j] = temp;
            }
        }
    }


    // draw projected sprites
    for(int i=0; i < numVisibleSprites; i++){
        sprite_t sprite = visibleSprites[i];

        // calc sprite projected height
        // if sprites were same w / and height then they could be same value but
        // leaving this in case we have images that are not same size
        int spriteImageHeight = TILE_SIZE;
        int spriteImageWidth = TILE_SIZE;

        // fixes fisheye effect, else use just rays[i].distance
        float correctedDistance = sprite.distance * cos(sprite.angle);

        float spriteHeight = (spriteImageHeight / correctedDistance) * DIST_PROJ_PLANE;
        float spriteWidth = (spriteImageWidth / correctedDistance) * DIST_PROJ_PLANE;


        float halfSpriteHeight = (spriteHeight * 0.5);
        float halfSpriteWidth = (spriteWidth * 0.5);

        float spriteTopY = HALF_WINDOW_HEIGHT - halfSpriteHeight;
        spriteTopY = spriteTopY < 0 ? 0 : spriteTopY;

        float spriteBottomY = HALF_WINDOW_HEIGHT + halfSpriteHeight;
        spriteBottomY = spriteBottomY > WINDOW_HEIGHT ? WINDOW_HEIGHT : spriteBottomY;

        // need the angle between the player and the sprite
        float spriteAngle = atan2(sprite.y - player.y, sprite.x - player.x) - player.rotationAngle;
        // get adjacent from that angle to get x offset 
        float spriteScreenPosX = tan(spriteAngle) * DIST_PROJ_PLANE;

        float spriteLeftX = HALF_WINDOW_WIDTH + spriteScreenPosX - halfSpriteWidth;
        float spriteRightX = spriteLeftX + spriteWidth;

        // query w and h of texture
        int textureWidth = upng_get_width(textures[sprite.texture]);
        int textureHeight = upng_get_height(textures[sprite.texture]);

        color_t* spriteTextureBuffer = (color_t*)upng_get_buffer(textures[sprite.texture]);

        // draw the pixels
        for(int x = spriteLeftX; x< spriteRightX; x ++){
            // figure out how many pixels to draw on the screen
            // for the sprite since it will be different if closer to
            // player or farther
            float texelWidth = (textureWidth / spriteWidth);
            int textureOffsetX = (x - spriteLeftX) * texelWidth;

            for(int y=spriteTopY; y < spriteBottomY; y++){
                
                if(x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT){
                    float texelHeight = textureHeight / spriteHeight;
                    int distanceFromTop = y + halfSpriteHeight - HALF_WINDOW_HEIGHT;
                    int textureOffsetY = distanceFromTop * texelHeight;

                    color_t texelColor = spriteTextureBuffer[(textureWidth * textureOffsetY) + textureOffsetX];
                    // use ray collision dist to know if something else is colliding closer than sprite, such as the walls
                    bool notOccluded = sprite.distance < rays[x].distance;
                    if(notOccluded && texelColor != 0xFFFF00FF){
                        drawPixel(x,y, texelColor);
                    }
                }
            }
        }
        
    }

}

void renderMapSprites(void){
    for(int i = 0; i < NUM_SPRITES; i++){
        drawRect(
            sprites[i].x * MINIMAP_SCALE_FACTOR,
            sprites[i].y * MINIMAP_SCALE_FACTOR,
            2,
            2,
            (sprites[i].visible) ? 0xFF00FFFF : 0xFF444444
        );
    }
}   