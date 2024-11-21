#include "ray.h"

ray_t rays[NUM_RAYS];



bool isRayFacingDown(float angle){ return angle > 0 && angle < PI;}
bool isRayFacingUp(float angle){ return !isRayFacingDown(angle);}
bool isRayFacingRight(float angle){ return angle < HALF_PI || angle > (PI * 1.5); }
bool isRayFacingLeft(float angle){ return !isRayFacingRight(angle); }

void castRay(float rayAngle, int i){
    
    normalizeAngle(&rayAngle);

    float xintercept, yintercept;
    float xstep, ystep;
    /**
     * HORIZONTAL RAY_GRID INTERSECTION CODE
     */

    bool foundHorzWallHit = false;
    float horzWallHitX = 0;
    float horzWallHitY = 0;
    int horzWallTexture = 0;

    yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
    yintercept += isRayFacingDown(rayAngle) ? TILE_SIZE : 0;

    xintercept = player.x + (yintercept - player.y) / tan(rayAngle);
    
    // calc x step and y step
    ystep = TILE_SIZE;
    ystep *= isRayFacingUp(rayAngle) ? -1 : 1;

    xstep = TILE_SIZE / tan(rayAngle);
    xstep *= (isRayFacingLeft(rayAngle) && xstep > 0) ? -1 : 1;
    xstep *= (isRayFacingRight(rayAngle) && xstep < 0) ? -1 : 1;

    float nextHorzTouchX = xintercept;
    float nextHorzTouchY = yintercept;

    // increment x and y step until a wall is found
    while (isInsideMap(nextHorzTouchX, nextHorzTouchY)) {
        
        float xToCheck = nextHorzTouchX;
        float yToCheck = nextHorzTouchY + (isRayFacingUp(rayAngle) ? -1 : 0);

        if(mapHasWallAt(xToCheck, yToCheck)){
            // hit a wall
            horzWallHitX = nextHorzTouchX;
            horzWallHitY = nextHorzTouchY;
            horzWallTexture = getMapAt( (int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
            foundHorzWallHit = true;
            break;
        } else {
            nextHorzTouchX += xstep;
            nextHorzTouchY += ystep;
        }
    }

    /**
     * VERTICAL RAY_GRID INTERSECTION CODE
     */
    bool foundVertWallHit = false;
    float vertWallHitX = 0;
    float vertWallHitY = 0;
    int vertWallTexture = 0;

    // find x-coord of the closest horz grid intersection
    xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
    xintercept += isRayFacingRight(rayAngle) ? TILE_SIZE : 0;

    yintercept = player.y + (xintercept - player.x) * tan(rayAngle);
    
    // calc x step and y step
    xstep = TILE_SIZE;
    xstep *= isRayFacingLeft(rayAngle) ? -1 : 1;

    ystep = TILE_SIZE * tan(rayAngle);
    ystep *= (isRayFacingUp(rayAngle) && ystep > 0) ? -1 : 1;
    ystep *= (isRayFacingDown(rayAngle) && ystep < 0) ? -1 : 1;

    float nextVertTouchX = xintercept;
    float nextVertTouchY = yintercept;


    // increment x and y step until a wall is found
    while(isInsideMap(nextVertTouchX, nextVertTouchY )){
         
        float xToCheck = nextVertTouchX + (isRayFacingLeft(rayAngle) ? -1 : 0);
        float yToCheck = nextVertTouchY;

        if(mapHasWallAt(xToCheck, yToCheck)){
            // hit a wall
            vertWallHitX = nextVertTouchX;
            vertWallHitY = nextVertTouchY;
            vertWallTexture = getMapAt( (int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
            foundVertWallHit = true;
            break;
        } else {
            nextVertTouchX += xstep;
            nextVertTouchY += ystep;
        }
    }

    // get delta from vert and horz and store shortest
    float horzHitDist = foundHorzWallHit
    ? distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY)
    : FLT_MAX;
    float vertHitDist = foundVertWallHit
    ? distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
    : FLT_MAX;

    // only store the smallest of the distance and x,y
    if(vertHitDist < horzHitDist){
        rays[i].distance = vertHitDist;
        rays[i].wallHitX = vertWallHitX;
        rays[i].wallHitY = vertWallHitY;
        rays[i].texture = vertWallTexture;
        rays[i].wasHitVertical = true;
    } else {
        rays[i].distance = horzHitDist;
        rays[i].wallHitX = horzWallHitX;
        rays[i].wallHitY = horzWallHitY;
        rays[i].texture = horzWallTexture;
        rays[i].wasHitVertical = false;
    }
    rays[i].rayAngle = rayAngle;

}

void castAllRays() {

    for(int col = 0;col < NUM_RAYS; col++){
        
        // uses arctan to have evenly spaced rays to avoid distortion
        float rayAngle = player.rotationAngle + atan((col - (NUM_RAYS * 0.5)) / DIST_PROJ_PLANE);
        castRay(rayAngle, col);
        
    }
}

void renderMapRays() {
    
    for(int i =0; i < NUM_RAYS; i++){
        drawLineDDA(
            player.x * MINIMAP_SCALE_FACTOR,
            player.y * MINIMAP_SCALE_FACTOR,
            rays[i].wallHitX * MINIMAP_SCALE_FACTOR,
            rays[i].wallHitY * MINIMAP_SCALE_FACTOR,
            0xFF0000FF
        );
    }
}


