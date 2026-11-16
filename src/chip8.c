#include "addresses.h"
#include "chip8.h"
#include "fontset.h"
#include "utils.h"
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
void Cycle(Chip8* c8){
    c8->opcode = ((c8->memory[c8->pc]) << 8) | (c8->memory[c8->pc + 1]);
    
    c8->pc += 2;

}
void OP_00E0(Chip8* c8){
    memset(c8->display, 0, DISPLAY_RESOLUTION);
}
void OP_00EE(Chip8* c8){
    --(c8->sp);
    c8->pc = c8->stack[c8->sp];
}
void OP_1nnn(Chip8* c8){
    uint16_t address = c8->opcode & 0x0FFF;
    c8->pc = address;
}
void OP_2nnn(Chip8* c8){
    uint16_t address = c8->opcode & 0x0FFF;
    c8->stack[c8->sp] = c8->pc;
    ++(c8->sp);
    c8->pc = address;
}
void OP_3xkk(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t byte = c8->opcode & 0x00FF;

    if (c8->registers[Vx] == byte)
        c8->pc += 2;
}
void OP_4xkk(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t byte = c8->opcode & 0x00FF;

    if (c8->registers[Vx] != byte)
        c8->pc += 2;
}
void OP_5xy0(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (c8->opcode & 0x00F0) >> 4;

    if (c8->registers[Vx] == (c8->registers)[Vy])
        c8->pc += 2;
}
void OP_6xkk(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t byte = c8->opcode & 0x00FF;

    c8->registers[Vx] = byte;
}
void OP_7xkk(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t byte = c8->opcode & 0x00FF;

    c8->registers[Vx] += byte;
}
void OP_8xy0(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (c8->opcode & 0x00F0) >> 4;

    c8->registers[Vx] = c8->registers[Vy];
}
void OP_8xy1(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (c8->opcode & 0x00F0) >> 4;

    c8->registers[Vx] |= c8->registers[Vy];

}
void OP_8xy2(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (c8->opcode & 0x00F0) >> 4;

    c8->registers[Vx] &= c8->registers[Vy];
}
void OP_8xy3(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (c8->opcode & 0x00F0) >> 4;

    c8->registers[Vx] ^= c8->registers[Vy];
}
void OP_8xy4(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (c8->opcode & 0x00F0) >> 4;

    uint16_t sum = c8->registers[Vx] + c8->registers[Vy];
    if (sum > 255)
        c8->registers[0xF] = 1;
    else
        c8->registers[0xF] = 0;

    c8->registers[Vx] = sum & 0xFF;
}
void OP_8xy5(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (c8->opcode & 0x00F0) >> 4;

    if (c8->registers[Vx] > c8->registers[Vy])
        c8->registers[0xF] = 1;
    else
        c8->registers[0xF] = 0;

    c8->registers[Vx] -= c8->registers[Vy];
}
void OP_8xy6(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;

    c8->registers[0xF] = c8->registers[Vx] & 0x1;

    c8->registers[Vx] >>= 1;
}
void OP_8xy7(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (c8->opcode & 0x00F0) >> 4;

    if (c8->registers[Vy] > c8->registers[Vx])
        c8->registers[0xF] = 1;
    else
        c8->registers[0xF] = 0;
    
    c8->registers[Vy] = c8->registers[Vy] - c8->registers[Vx];
}
void OP_8xyE(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;

    c8->registers[0xF] = (c8->registers[Vx] & 0x80) >> 7;

    c8->registers[Vx] <<= 1;
}
void OP_9xy0(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (c8->opcode & 0x00F0) >> 4;

    if (c8->registers[Vx] != c8->registers[Vy])
        c8->pc += 2;
}
void OP_Annn(Chip8* c8){
    uint16_t address = c8->opcode & 0x0FFF;

    c8->index = address;
}
void OP_Bnnn(Chip8* c8){
    uint16_t address = c8->opcode & 0x0FFF;

    c8->pc = c8->registers[0] + address;
}
void OP_Cxkk(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t byte = (c8->opcode & 0x00FF);

    c8->registers[Vx] = random_u8() & byte;
}
void OP_Dxyn(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (c8->opcode & 0x00F0) >> 4;
    uint8_t height = (c8->opcode & 0x000F);

    uint8_t xPos = (c8->registers[Vx]) % DISPLAY_WIDTH;
    uint8_t yPos = (c8->registers[Vy]) % DISPLAY_HEIGHT;
    
    c8->registers[0xF] = 0;

    for (int row = 0; row < height; ++row){
        uint8_t spriteByte = c8->memory[c8->index + row];
        for (int col = 0; col < 8; ++col){
            uint8_t spritePixel = spriteByte & (0x80 >> col);
            uint8_t* screenPixel = &(c8->display[(yPos + row) * DISPLAY_WIDTH + (xPos + col)]);

            if (spritePixel){
                if (*screenPixel == 0xFFFFFFFF)
                    c8->registers[0xF] = 1;

                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}
void OP_Ex9E(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;

    uint8_t key = c8->registers[Vx];
    if (c8->inputs[key])
        c8->pc += 2;
}
void OP_ExA1(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;

    uint8_t key = c8->registers[Vx];
    if (!(c8->inputs[key]))
        c8->pc += 2;

}
void OP_Fx07(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;

    c8->registers[Vx] = c8->delay_timer;
}
void OP_Fx0A(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    if (c8->inputs[0])
        c8->registers[Vx] = 0;
    else if (c8->inputs[1])
        c8->registers[Vx] = 1;
    else if (c8->inputs[2])
        c8->registers[Vx] = 2;
    else if (c8->inputs[3])
        c8->registers[Vx] = 3;
    else if (c8->inputs[4])
        c8->registers[Vx] = 4;
    else if (c8->inputs[5])
        c8->registers[Vx] = 5;
    else if (c8->inputs[6])
        c8->registers[Vx] = 6;
    else if (c8->inputs[7])
        c8->registers[Vx] = 7;
    else if (c8->inputs[8])
        c8->registers[Vx] = 8;
    else if (c8->inputs[9])
        c8->registers[Vx] = 9;
    else if (c8->inputs[10])
        c8->registers[Vx] = 10;
    else if (c8->inputs[11])
        c8->registers[Vx] = 11;
    else if (c8->inputs[12])
        c8->registers[Vx] = 12;
    else if (c8->inputs[13])
        c8->registers[Vx] = 13;
    else if (c8->inputs[14])
        c8->registers[Vx] = 14;
    else if (c8->inputs[15])
        c8->registers[Vx] = 15;
    else
        c8->pc -= 2;
}
void OP_Fx15(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;

    c8->delay_timer = c8->registers[Vx];
}
void OP_Fx18(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;

    c8->sound_timer = c8->registers[Vx];

}
void OP_Fx1E(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;

    c8->index += c8->registers[Vx];
}
void OP_Fx29(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t digit = c8->registers[Vx];

    c8->index = CHARSET_ADDRESS + (5*digit);
}
void OP_Fx33(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    uint8_t value = c8->registers[Vx];

    c8->memory[c8->index + 2] = value % 10;
    value /= 10;

    c8->memory[c8->index + 1] = value % 10;
    value /= 10;

    c8->memory[c8->index] = value % 10;
}
void OP_Fx55(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    for (int i = 0; i < Vx; ++i)
        c8->memory[c8->index + i] = c8->registers[i];
}
void OP_Fx65(Chip8* c8){
    uint8_t Vx = (c8->opcode & 0x0F00) >> 8;
    for (int i = 0; i < Vx; ++i)
        c8->registers[i] = c8->memory[c8->index + i];
}
void OP_NULL(Chip8* c8){}
void init_chip8FuncTable(chip8FuncTable* t){
    t->table[0x0] = &get_from_table0;
    t->table[0x1] = &OP_1nnn;
    t->table[0x2] = &OP_2nnn;
    t->table[0x3] = &OP_3xkk;
    t->table[0x4] = &OP_4xkk;
    t->table[0x5] = &OP_5xy0;
    t->table[0x6] = &OP_6xkk;
    t->table[0x7] = &OP_7xkk;
    t->table[0x8] = &get_from_table8;
    t->table[0x9] = &OP_9xy0;
    t->table[0xA] = &OP_Annn;
    t->table[0xB] = &OP_Bnnn;
    t->table[0xC] = &OP_Cxkk;
    t->table[0xD] = &OP_Dxyn;
    t->table[0xE] = &get_from_tableE;
    t->table[0xF] = &get_from_tableF;

    for (int i = 0; i <= 0xE; ++i){
        t->table0[i] = &OP_NULL;
        t->table8[i] = &OP_NULL;
        t->tableE[i] = &OP_NULL;
    }

    t->table0[0x0] = &OP_00E0;
    t->table0[0xE] = &OP_00EE;
    
    t->table8[0x0] = &OP_8xy0;
    t->table8[0x1] = &OP_8xy1;
    t->table8[0x2] = &OP_8xy2;
    t->table8[0x3] = &OP_8xy3;
    t->table8[0x4] = &OP_8xy4;
    t->table8[0x5] = &OP_8xy5;
    t->table8[0x6] = &OP_8xy6;
    t->table8[0x7] = &OP_8xy7;
    t->table8[0xE] = &OP_8xyE;

    t->tableE[0x1] = &OP_ExA1;
    t->table0[0xE] = &OP_Ex9E;

    for (int i = 0; i <= 0x65; ++i)
        t->tableF[i] = &OP_NULL;

    t->tableF[0x07] = &OP_Fx07;
    t->tableF[0x0A] = &OP_Fx0A;
    t->tableF[0x15] = &OP_Fx15;
    t->tableF[0x18] = &OP_Fx18;
    t->tableF[0x1E] = &OP_Fx1E;
    t->tableF[0x29] = &OP_Fx29;
    t->tableF[0x33] = &OP_Fx33;
    t->tableF[0x55] = &OP_Fx55;
    t->tableF[0x65] = &OP_Fx65;
}
void get_from_table0(chip8FuncTable* t, Chip8* c8){
    t->table0[c8->opcode & 0x000F]();
}
void get_from_table8(chip8FuncTable* t, Chip8* c8){
    t->table8[c8->opcode & 0x000F]();
}
void get_from_tableE(chip8FuncTable* t, Chip8* c8){
    t->tableE[c8->opcode & 0x000F]();
}
void get_from_tableF(chip8FuncTable* t, Chip8* c8){
    t->tableF[c8->opcode & 0x000F]();
}