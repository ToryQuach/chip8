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
void OP_00E0(Chip8* c8){
    memset(c8->display, 0, DISPLAY_RESOLUTION);
}
void OP_00EE(Chip8* c8){
    --(c8->sp);
    c8->pc = (c8->stack)[c8->sp];
}
void OP_1nnn(Chip8* c8){
    uint16_t address = c8->opcode & 0x0FFF;
    c8->pc = address;
}
void OP_2nnn(Chip8* c8){
    uint16_t address = c8->opcode & 0x0FFF;
    (c8->stack)[c8->sp] = c8->pc;
    ++(c8->sp);
    c8->pc = address;
}
void OP_3xkk(Chip8* c8){

}
void OP_4xkk(Chip8* c8){

}
void OP_5xy0(Chip8* c8){

}
void OP_6xkk(Chip8* c8){

}
void OP_7xkk(Chip8* c8){

}
void OP_8xy0(Chip8* c8){

}
void OP_8xy1(Chip8* c8){

}
void OP_8xy2(Chip8* c8){

}
void OP_8xy3(Chip8* c8){

}
void OP_8xy4(Chip8* c8){

}
void OP_8xy5(Chip8* c8){

}
void OP_8xy6(Chip8* c8){

}
void OP_8xy7(Chip8* c8){

}
void OP_8xyE(Chip8* c8){

}
void OP_9xy0(Chip8* c8){

}
void OP_Annn(Chip8* c8){

}
void OP_Bnnn(Chip8* c8){

}
void OP_Cxkk(Chip8* c8){

}
void OP_Dxyn(Chip8* c8){

}
void OP_Ex9E(Chip8* c8){

}
void OP_ExA1(Chip8* c8){

}
void OP_Fx07(Chip8* c8){

}
void OP_Fx0A(Chip8* c8){

}
void OP_Fx15(Chip8* c8){

}
void OP_Fx18(Chip8* c8){

}
void OP_Fx1E(Chip8* c8){

}
void OP_Fx29(Chip8* c8){

}
void OP_Fx33(Chip8* c8){

}
void OP_Fx55(Chip8* c8){

}
void OP_Fx65(Chip8* c8){

}