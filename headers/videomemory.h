#ifndef VIDEOMEMORY_H
#define VIDEOMEMORY_H

#include "constants.h"
#include <SDL.h>

const size_t SCREEN_X_POSITION = 100;
const size_t SCREEN_Y_POSITION = 100;
const size_t SCREEN_WIDTH      = 640;
const size_t SCREEN_HEIGHT     = 480;

const int ELEM_IN_LINE = 32;
const int BIG_PX_SIZE  = 8;
const int SCALE        = SCREEN_HEIGHT / ELEM_IN_LINE;

void PrintMemory (const Processor_t* const ram_array);

void MakeScreen (SDL_Window** window, SDL_Renderer** renderer);

void ColorPoints (const Processor_t * const ram_array,
                        SDL_Renderer* const renderer);

void PresentScreen (SDL_Renderer* const renderer);

#endif
