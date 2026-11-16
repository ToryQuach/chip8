#include "platform.h"
#include <string.h>

void init_platform(Platform* p, char const* title, int* windowWidth, int* windowHeight, int* textureWidth, int* textureHeight){
    SDL_Init(SDL_INIT_VIDEO);

    memset(p, 0, sizeof(p));

    p->window = SDL_CreateWindow(title, *windowWidth, *windowHeight, 0);
    if (!(p->window)) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    p->renderer = SDL_CreateRenderer(p->window, NULL);
    if (!(p->renderer)) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(p->window);
        SDL_Quit();
        return -1;
    }

    p->texture = SDL_CreateTexture(
        p->renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight
    );
    if (!(p->texture)) {
        SDL_Log("SDL_CreateTexture failed: %s", SDL_GetError());
        SDL_DestroyWindow(p->renderer);
        SDL_DestroyWindow(p->window);
        SDL_Quit();
        return -1;
    }
}
void destroy_platform(Platform* p){
    SDL_DestroyTexture(p->texture);
    SDL_DestroyRenderer(p->renderer);
    SDL_DestroyWindow(p->window);
    SDL_Quit();
}
void update_platform(Platform* p, void const* buffer, int* pitch){
    SDL_UpdateTexture(p->texture, NULL, buffer, *pitch);
    SDL_RenderClear(p->renderer);
    SDL_RenderTexture(p->renderer, p->texture, NULL, NULL);
    SDL_RenderPresent(p->renderer);
}

bool proccess_input(Platform* p, uint8_t* keys){
    bool quit = false;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                quit = true;
                break;

            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
            {
                if (event.key.key == SDLK_ESCAPE && event.type == SDL_EVENT_KEY_DOWN)
                {
                    quit = true;
                    break;
                }

                uint8_t state = (event.type == SDL_EVENT_KEY_DOWN) ? 1 : 0;

                for (int i = 0; i < 16; i++)
                {
                    if (event.key.key == keymap[i])
                    {
                        keys[i] = state;
                        break;
                    }
                }
            } break;
        }
    }

    return quit;
}