#pragma once

#include <cstdint>

#define ROM_ADDRESS 0x200

// Chip 8 Architect
typedef struct{
    uint8_t registers[16]{};
    uint8_t memory[4096]{};
    uint16_t index{};
    uint16_t pc{};
    uint16_t stack[16]{};
    uint8_t sp{};
    uint8_t delay_timer{};
    uint8_t sound_timer{};
    uint8_t inputs[16]{};
    uint32_t display[64*32]{};
    uint16_t opcode{};
} Chip8;

// Initialize Chip 8
void init_chip8(Chip8* c8);

// load ROM
void LoadROM(Chip8* c8, char const* filename);

// Instructions

// CLS
void OP_00E0(Chip8* c8);
// RET
void OP_00EE(Chip8* c8);
// JP addr
void OP_1nnn(Chip8* c8);
// CALL add
void OP_2nnn(Chip8* c8);
// SE Vx, byte
void OP_3xkk(Chip8* c8);
// SNE Vx, byte
void OP_4xkk(Chip8* c8);
// Se Vx, Vy
void OP_5xy0(Chip8* c8);
// LD Vx, byte
void OP_6xkk(Chip8* c8);
// ADD Vx, byte
void OP_7xkk(Chip8* c8);
// LD Vx, Vy
void OP_8xy0(Chip8* c8);
// OR Vx, Vy
void OP_8xy1(Chip8* c8);
// AND Vx, Vy
void OP_8xy2(Chip8* c8);
// XOR Vx, Vy
void OP_8xy3(Chip8* c8);
// ADD Vx, Vy
void OP_8xy4(Chip8* c8);
// SUB Vx, Vy
void OP_8xy5(Chip8* c8);
// SHR Vx
void OP_8xy6(Chip8* c8);
// SUBN Vx, Vy
void OP_8xy7(Chip8* c8);
// SHL Vx {, Vy}
void OP_8xyE(Chip8* c8);
// SNE Vx, Vy
void OP_9xy0(Chip8* c8);
// LD I, addr
void OP_Annn(Chip8* c8);
// JP V0, addr
void OP_Bnnn(Chip8* c8);
// RND Vx, byte
void OP_Cxkk(Chip8* c8);
// DRW Vx, Vy, nibble
void OP_Dxyn(Chip8* c8);
// SKP Vx
void OP_Ex9E(Chip8* c8);
// SKNP Vx
void OP_ExA1(Chip8* c8);
// LD Vx, DT
void OP_Fx07(Chip8* c8);
// LD Vx, K
void OP_Fx0A(Chip8* c8);
// LD DT, Vx
void OP_Fx15(Chip8* c8);
// LD ST, Vx
void OP_Fx18(Chip8* c8);
// ADD I, Vx
void OP_Fx1E(Chip8* c8);
// LD F, Vx
void OP_Fx29(Chip8* c8);
// LD B, Vx
void OP_Fx33(Chip8* c8);
// LD [I], Vx
void OP_Fx55(Chip8* c8);
// LD Vx, [I]
void OP_Fx65(Chip8* c8);