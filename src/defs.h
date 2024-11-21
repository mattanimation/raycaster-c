#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>
#include <stdint.h>

#define PI 3.14159265
#define TWO_PI 6.28318530
#define HALF_PI 1.5707963

#define	TILE_SIZE 64

#define MINIMAP_SCALE_FACTOR 0.2

// 16:10 - laptop ratio
#define WINDOW_WIDTH 768
#define WINDOW_HEIGHT 480

#define HALF_WINDOW_WIDTH (WINDOW_WIDTH * 0.5)
#define HALF_WINDOW_HEIGHT (WINDOW_HEIGHT * 0.5)

#define DEG2RAD (PI / 180)
#define FOV_ANGLE (60 * DEG2RAD)
#define HALF_FOV_ANGLE (FOV_ANGLE * 0.5)
#define DIST_PROJ_PLANE (HALF_WINDOW_WIDTH / tan(HALF_FOV_ANGLE))

#define NUM_RAYS WINDOW_WIDTH

#define FPS 30
#define FRAME_TIME_LENGTH (1000 / FPS)

//#define TEXTURE_WIDTH 64
//#define TEXTURE_HEIGHT 64


typedef uint32_t color_t;

#endif