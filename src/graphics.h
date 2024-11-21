#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include "defs.h"

bool initializeWindow(void);
void destroyWindow();

void clearColorBuffer(color_t color);
void renderColorBuffer();

void drawPixel(int x, int y, color_t color);
void drawRect(int x, int y, int width, int height, color_t color);
void drawLineDDA(int x0, int y0, int x1, int y1, color_t color);
void drawCircle(int x, int y, float radius, color_t color);

void changeColorIntensity(color_t* color, float factor);

#endif