#pragma once

#include "chip8.h"

typedef void (*chip8Func)(void);

typedef struct{
    chip8Func table[0xF + 1];
    chip8Func table0[0xE + 1];
    chip8Func table8[0xE + 1];
    chip8Func tableE[0xE + 1];
    chip8Func tableF[0x65 + 1];
} chip8FuncTable;

// Initialize function table
void init_chip8FuncTable(chip8FuncTable* t);
// Get function from table 0
void get_from_table0(chip8FuncTable* t, Chip8* c8);
// Get function from table 8
void get_from_table8(chip8FuncTable* t, Chip8* c8);
// Get function from table E
void get_from_tableE(chip8FuncTable* t, Chip8* c8);
// Get function from table F
void get_from_tableF(chip8FuncTable* t, Chip8* c8);