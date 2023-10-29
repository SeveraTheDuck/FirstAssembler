#include "../headers/constants.h"
#include "../headers/videomemory.h"

#define SetBlackColor(renderer) \
    SDL_SetRenderDrawColor((renderer), 0, 0, 0, 0);

#define SetGreyColor(renderer) \
    SDL_SetRenderDrawColor((renderer), 80, 80, 80, 255);

#define SetRedColor(renderer)  \
    SDL_SetRenderDrawColor((renderer), 255, 0, 0, 255);

void PrintMemory (const Processor_t* const ram_array)
{
    SDL_Window  * window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    MakeScreen (&window, &renderer);

    SetBlackColor  (renderer);
    SDL_RenderClear(renderer);

    ColorPoints (ram_array, renderer);

    PresentScreen (renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MakeScreen (SDL_Window** window, SDL_Renderer** renderer)
{
    if (SDL_Init (SDL_INIT_EVERYTHING) != 0)
    {
        fprintf (stderr, "SDL_Init error %s", SDL_GetError());
        return;
    }

    *window = SDL_CreateWindow ("RAM visualize",
                                SCREEN_X_POSITION,
                                SCREEN_Y_POSITION,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);
    if (*window == nullptr)
    {
        fprintf (stderr, "SDL_CreateWindow error %s", SDL_GetError());
        return;
    }

    *renderer = SDL_CreateRenderer (*window, -1,
                SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == nullptr)
    {
        fprintf (stderr, "SDL_CreateRenderer error %s", SDL_GetError());
        return;
    }
}

void ColorPoints (const Processor_t * const ram_array,
                        SDL_Renderer* const renderer)
{
    SDL_Rect big_px = {.x = 0, .y = 0,
                       .w = BIG_PX_SIZE,
                       .h = BIG_PX_SIZE};

    SetGreyColor (renderer);
    for (int y = 0; y < (int) RAM_AMOUNT / ELEM_IN_LINE; ++y)
    {
        for (int x = 0; x < ELEM_IN_LINE; ++x)
        {
            big_px.x = x * SCALE;
            big_px.y = y * SCALE;
            SDL_RenderDrawRect (renderer, &big_px);

            if (*(ram_array + ELEM_IN_LINE * y + x))
            {
                SetRedColor (renderer);
                SDL_RenderFillRect (renderer, &big_px);
                SetGreyColor (renderer);
            }
        }
    }
}

void PresentScreen (SDL_Renderer* const renderer)
{
    SDL_Event event = {};

    SDL_RenderPresent(renderer);
    while (true)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            return;
        }
    }
}

#undef SetBlackColor
#undef SetGreyColor
#undef SetRedColor
