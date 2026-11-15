#include "addresses.h"
#include "chip8.h"
#include "fontset.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void init_chip8(Chip8* c8){
    // Initialize every fields to 0
    memset(c8, 0, sizeof(Chip8));
    
    // Set pc to ROM start
    c8->pc = ROM_ADDRESS;

    // Load font
    for (long i = 0; i < FONTSET_SIZE; ++i)
        c8->memory[CHARSET_ADDRESS + i] = FONTSET[i];
}
int LoadROM(Chip8* c8, char const* filename){
    // Open file in read byte mode
    FILE* file = fopen(filename, "rb");
    if (file == NULL){
        perror("Failed to load ROM");
        return 1;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    if (size > MEMORY_SIZE - ROM_ADDRESS){
        perror("Loaded ROM is too big");
        fclose(file);
        return 1;
    }

    // Malloc buffer
    char* buffer = malloc(sizeof(char) * size);
    if (buffer == NULL){
        perror("Malloc failed");
        fclose(file);
        return 1;
    }
    
    // Read file
    size_t bytesRead = fread(buffer, 1, (size_t)size, file);
    if (bytesRead != (size_t)size){
        perror("Error reading the file");
        free(buffer);
        fclose(file);
        return 1;
    }
    fclose(file);

    // Load data to ROM
    for (long i = 0; i < size; ++i)
        c8->memory[ROM_ADDRESS + i] = buffer[i];

    free(buffer);
    return 0;
}