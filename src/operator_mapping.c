#include "operator_mapping.h"

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