#include "wall.h"

// modified from: https://lodev.org/cgtutor/raycasting2.html
void renderFloorAndCeilProjection(void){

    //FLOOR CASTING
    int h = WINDOW_HEIGHT;
    float planeY = 0.66;
    float planeX = 0.66 * cos(-player.rotationAngle) - planeY * sin(-player.rotationAngle);
    planeY = planeX * sin(-player.rotationAngle) + planeY * cos(-player.rotationAngle);
    float dirY = player.y + 5;
    float dirX = (player.x + 5) * cos(-player.rotationAngle) - dirY * sin(-player.rotationAngle); //cos(player.rotationAngle);
    dirY = (player.y + 5) * sin(-player.rotationAngle) + dirY * cos(-player.rotationAngle); //sin(player.rotationAngle);
    for(int y = 0; y < h; y++)
    {
      // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
      float rayDirX0 = dirX - planeX;
      float rayDirY0 = dirY - planeY;
      float rayDirX1 = dirX + planeX;
      float rayDirY1 = dirY + planeY;

      // Current y position compared to the center of the screen (the horizon)
      int p = y - HALF_WINDOW_HEIGHT;

      // Vertical position of the camera.
      float posZ = 0.5 * HALF_WINDOW_HEIGHT;

      // Horizontal distance from the camera to the floor for the current row.
      // 0.5 is the z position exactly in the middle between floor and ceiling.
      float rowDistance = posZ / p;

      // calculate the real world step vector we have to add for each x (parallel to camera plane)
      // adding step by step avoids multiplications with a weight in the inner loop
      float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / WINDOW_WIDTH;
      float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / WINDOW_WIDTH;

      // real world coordinates of the leftmost column. This will be updated as we step to the right.
      float floorX = player.x + rowDistance * rayDirX0;
      float floorY = player.y + rowDistance * rayDirY0;

      int floorTexNum = 3; // stone
      int ceilTexNum = 6; // wood
      int texture_width = upng_get_width(textures[floorTexNum]);
      int texture_height = upng_get_height(textures[floorTexNum]);
      color_t* floorTextureBuffer = (color_t*)upng_get_buffer(textures[floorTexNum]);
      color_t* ceilTextureBuffer = (color_t*)upng_get_buffer(textures[ceilTexNum]);

      for(int x = 0; x < WINDOW_WIDTH; ++x)
      {
        // the cell coord is simply got from the integer parts of floorX and floorY
        int cellX = (int)(floorX);
        int cellY = (int)(floorY);

        // get the texture coordinate from the fractional part
        int tx = (int)(texture_width * (floorX - cellX)) & (texture_width - 1);
        int ty = (int)(texture_height * (floorY - cellY)) & (texture_height - 1);

        floorX += floorStepX;
        floorY += floorStepY;

        // choose texture and draw the pixel
        //int floorTexture = 3;
        //int ceilingTexture = 6;
        //Uint32 color;

        // floor
        //color = texture[floorTexture][texWidth * ty + tx];
        //color = (color >> 1) & 8355711; // make a bit darker
        //buffer[y][x] = color;

        color_t texelColor;
        texelColor = floorTextureBuffer[(texture_width * ty) + tx];
        drawPixel(x, y, texelColor);

        //ceiling (symmetrical, at screenHeight - y - 1 instead of y)
        //color = texture[ceilingTexture][texWidth * ty + tx];
        //color = (color >> 1) & 8355711; // make a bit darker
        //buffer[screenHeight - y - 1][x] = color;

        texelColor = ceilTextureBuffer[(texture_width * ty) + tx];
        drawPixel(x, WINDOW_HEIGHT - y - 1, texelColor);

        
      }
    }

}

void renderWallProjection(void) {

    for(int x =0; x < NUM_RAYS; x++){

        // fixes fisheye effect, else use just rays[i].distance
        float correctedDistance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle);

        // calc projected wall height
        float wallHeight = (TILE_SIZE / correctedDistance) * DIST_PROJ_PLANE;

        // find the upper most pixel of the wall
        int wallTopY = HALF_WINDOW_HEIGHT - (wallHeight * 0.5);
        wallTopY = wallTopY < 0 ? 0 : wallTopY;

        // find the lower most pixel of the wall
        int wallBottomY = HALF_WINDOW_HEIGHT + (wallHeight * 0.5);
        wallBottomY = wallBottomY > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomY;

        // render ceiling
        // solid color
        for(int y = 0; y < wallTopY; y++){
            drawPixel(x, y, 0xFF333333);
        }

        // render the wall from wallTopY to wallBottomY
        // a bunch of small rows to fill the wall strip
        int textureOffsetX;
        if(rays[x].wasHitVertical) {
            textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
        } else {
            textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;
        }

        // get the correct texture id number from the map content
        //printf("textute: %d\n", rays[x].texture);
        int texNum = rays[x].texture - 1; //offset since the map has one index (0) off from the texture arr
        int texture_width = upng_get_width(textures[texNum]);
        int texture_height = upng_get_height(textures[texNum]);

        // render wall from top tp bottom
        for(int y = wallTopY; y < wallBottomY; y++){
            // set the color of the wall based on the color from the texture
            int distanceFromTop = y + (wallHeight * 0.5) - HALF_WINDOW_HEIGHT;
            int textureOffsetY = distanceFromTop * ((float)texture_height / wallHeight);
            color_t* wallTextureBuffer = (color_t*)upng_get_buffer(textures[texNum]);
            color_t texelColor = wallTextureBuffer[(texture_width * textureOffsetY) + textureOffsetX]; //textures ,wallTexture[(TEXTURE_WIDTH * textureOffsetY) + textureOffsetX];

            // change intensity to give shadow effect
            if(rays[x].wasHitVertical){
            	changeColorIntensity(&texelColor, 0.7);
            }

            //colorBuffer[(WINDOW_WIDTH * y) + x] = texelColor; //rays[i].wasHitVertical ? 0xFFFFFFFF : 0xFFCCCCCC;
            drawPixel(x, y, texelColor);
        }
        
        // render floor
        // solid color
        for(int y = wallBottomY; y < WINDOW_HEIGHT; y++){
            drawPixel(x, y, 0xFF777777);
        }

    }
}