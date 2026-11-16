#include "chip8.h"
#include "platform.h"
#include <time.h>
#include <stdio.h>

double get_time_sec(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Usage: %s <Scale> <Delay> <ROM>", argv[0]);
        return 1;
    }

    int scale = SDL_atoi(argv[1]);
    double delay = SDL_atoi(argv[2]) / 1000.0;
    const char *rom_path = argv[3];

    Platform platform;
    if (init_platform(&platform, "Chip-8 Emulator", DISPLAY_WIDTH * scale, DISPLAY_HEIGHT * scale, DISPLAY_WIDTH, DISPLAY_HEIGHT) != 0){
        perror("Failed to initialize platform.\n");
        return 1;
    }


    Chip8 chip8;
    init_chip8(&chip8);
    if (load_ROM(&chip8, rom_path) != 0){
        perror("Failed to load ROM");
        destroy_platform(&platform);
        return 1;
    }

    int display_pitch = sizeof(chip8.display[0]) * DISPLAY_WIDTH;

    double last_cycle_time = get_time_sec();
    bool quit = false;
    
    while (!quit)
    {
        quit = proccess_input(chip8.inputs);
        double current_time = get_time_sec();
        double dt = current_time - last_cycle_time;
        
        if (dt <= delay)
            continue;
        
        last_cycle_time = current_time;
        cycle(&chip8);
        update_platform(&platform, chip8.display, &display_pitch);
    }
    

    destroy_platform(&platform);
    return 0;
}