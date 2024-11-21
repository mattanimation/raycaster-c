#include "graphics.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static color_t* colorBuffer = NULL;
static SDL_Texture* colorBufferTexture;

bool initializeWindow() {

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL\n");
        return false;
    }
    // load in the current info of the display
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    // get screen size but with a little offset in size
    int fullScreenWidth = display_mode.w - (int)(display_mode.w * 0.1);
    int fullScreenHeight = display_mode.h - (int)(display_mode.h * 0.1);

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        fullScreenWidth,
        fullScreenHeight,
        SDL_WINDOW_BORDERLESS
    );

    if(!window){
        fprintf(stderr, "Error creating SDL window. \n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        fprintf(stderr, "Error creating SDL renderer. \n");
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


    // alloc the total color buffer size to hold the color buffer
    colorBuffer = (color_t*) malloc( sizeof(color_t) * (color_t)WINDOW_WIDTH * (color_t)WINDOW_HEIGHT);

    // create an SDL texture to disply the color buffer
    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32, //ARGB8888
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    return true;

}

void destroyWindow() {
    // clean up
    free(colorBuffer);
    SDL_DestroyTexture(colorBufferTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void clearColorBuffer(color_t color) {
    for(int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++){
        colorBuffer[i] = color;
    }
}

void renderColorBuffer(){
    SDL_UpdateTexture(
        colorBufferTexture,
        NULL,
        colorBuffer,
        (int)((color_t)WINDOW_WIDTH * sizeof(color_t))  //size in bytes of a row of the texture
    );
    // render the texture
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);

    SDL_RenderPresent(renderer);
}

void drawPixel(int x, int y, color_t color) {
	colorBuffer[(WINDOW_WIDTH * y) + x] = color;
}

void drawRect(int x, int y, int width, int height, color_t color){
	for(int i=x; i < (x + width); i++){
		for(int j=y; j < (y + height); j++){
			drawPixel(i, j, color);
		}
	}
	
}

void drawCirc(int xc, int yc, int x, int y, color_t color){
    drawPixel(xc+x, yc+y, color);
    drawPixel(xc-x, yc+y, color);
    drawPixel(xc+x, yc-y, color);
    drawPixel(xc-x, yc-y, color);
    drawPixel(xc+y, yc+x, color);
    drawPixel(xc-y, yc+x, color);
    drawPixel(xc+y, yc-x, color);
    drawPixel(xc-y, yc-x, color);
}

// Function for circle-generation
// using Bresenham's algorithm
void drawCircle(int x, int y, float radius, color_t color){

    int _x =0, _y = radius;
    float d = 3 - (2 * radius);
    drawCirc(x, y, _x, _y, color);
    while( _y >= _x){

        if(d > 0){
            _y--;
            d = d + 4 * (_x - _y) + 10;
        } else {
            d = d + 4 * _x + 6;
        }
        _x++;
        drawCirc(x, y, _x, _y, color);
    }
}

// basic DDA algo
void drawLineDDA(int x0, int y0, int x1, int y1, color_t color){

    int deltaX = (x1 - x0);
    int deltaY= (y1 - y0);

    int longestSide = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

    float xInc = deltaX / (float)longestSide;
    float yInc = deltaY / (float)longestSide;

    float currentX = x0;
    float currentY = y0;

    for(int i =0; i <= longestSide; i++){
        drawPixel(round(currentX), round(currentY), color);
        currentX += xInc;
        currentY += yInc;
    }

}

// change color intensity  based on a factor value between0 and 1
void changeColorIntensity(color_t* color, float factor){
    color_t a = (*color & 0xFF000000);
    color_t r = (*color & 0x00FF0000) * factor;
    color_t g = (*color & 0x0000FF00) * factor;
    color_t b = (*color & 0x000000FF) * factor;

    *color = a | (r & 0x00FF000000) | ( g & 0x0000FF00) | (b & 0x000000FF);
}


