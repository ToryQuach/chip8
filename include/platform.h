#pragma once

#include <SDL3/SDL.h>
#include <stdint.h>

static const SDL_Keycode keymap[16] = {
    SDLK_X, // 0x0
    SDLK_1, // 0x1
    SDLK_2, // 0x2
    SDLK_3, // 0x3
    SDLK_Q, // 0x4
    SDLK_W, // 0x5
    SDLK_E, // 0x6
    SDLK_A, // 0x7
    SDLK_S, // 0x8
    SDLK_D, // 0x9
    SDLK_Z, // 0xA
    SDLK_C, // 0xB
    SDLK_4, // 0xC
    SDLK_R, // 0xD
    SDLK_F, // 0xE
    SDLK_V  // 0xF
};

typedef struct{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
} Platform;

// Initialize Platform
int init_platform(Platform* p, char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);

// Distructor Platform
void destroy_platform(Platform* p);

// Update Platform
void update_platform(Platform* p, void const* buffer, int* pitch);

// Proccess Input
bool proccess_input(uint8_t* keys);