#include "addresses.h"
#include "chip8.h"
#include <string.h>

void init_chip8(Chip8* c8){
    // Initialize every fields to 0
    memset(c8, 0, sizeof(Chip8));
    
    // Set pc to ROM start
    c8->pc = ROM_ADDRESS;
}