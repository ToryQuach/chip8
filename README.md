BUILDING CHIP-8 EMULATOR

Build
-----
    cmake -B build .
    cmake --build build

Run
---
    ./build/chip8 <Scale> <Delay-ms> <ROM>

Example:

    ./build/chip8 10 3 ROMs/test_opcode.ch8

Note: `<Delay-ms>` is the delay between CPU cycles in milliseconds, not the
instruction rate in Hz.

Assembling Octo (.8o) sources
-----------------------------
A `.8o` file is Octo assembly *source text*, not a CHIP-8 binary. It must be
assembled into a `.ch8` file before the emulator can run it; loading the raw
text just executes ASCII bytes as opcodes (blank/garbage screen).

Use `octo-cli` from C-Octo (https://github.com/JohnEarnest/c-octo):

    git clone https://github.com/JohnEarnest/c-octo
    make -C c-octo cli
    ./c-octo/build/octo-cli ROMs/test_opcode.8o ROMs/test_opcode.ch8
    ./build/chip8 10 3 ROMs/test_opcode.ch8
