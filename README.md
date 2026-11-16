# CHIP-8 Emulator

A CHIP-8 interpreter written in C, using SDL3 for graphics, input, and windowing.

![Language](https://img.shields.io/badge/language-C11-blue)
![Graphics](https://img.shields.io/badge/graphics-SDL3-green)
![Build](https://img.shields.io/badge/build-CMake-orange)

---

## Features

- 4 KB memory, 16 general-purpose `V` registers, and a 16-level call stack
- Full 64×32 monochrome display with sprite drawing and collision detection
- Built-in hexadecimal font set
- Delay and sound timers
- 16-key hexadecimal keypad mapped to the keyboard
- Function-pointer opcode dispatch table
- Loads standard `.ch8` ROMs

## Requirements

- A C11 compiler (`gcc` or `clang`)
- [CMake](https://cmake.org/) 3.16 or newer
- [SDL3](https://github.com/libsdl-org/SDL) development libraries (with a CMake config package)

Make sure SDL3 can be found by CMake, either because it is installed system-wide or by passing its
location with `-DCMAKE_PREFIX_PATH=/path/to/SDL3`.

## Build

```sh
cmake -B build .
cmake --build build
```

The executable is written to `build/chip8`.

## Usage

```sh
./build/chip8 <scale> <delay-ms> <rom>
```

| Argument    | Description                                              |
| ----------- | -------------------------------------------------------- |
| `scale`     | Window scale factor (e.g. `10` gives a 640×320 window)    |
| `delay-ms`  | Delay between CPU cycles, in milliseconds                |
| `rom`       | Path to a `.ch8` ROM file                                |

Example:

```sh
./build/chip8 10 3 ROMs/test.ch8
```

> **Note:** `<delay-ms>` is the delay between CPU cycles in milliseconds, not the instruction rate in Hz.

### Controls

The CHIP-8 keypad is mapped to the following keys:

```text
 CHIP-8 keypad            Keyboard
 ┌───┬───┬───┬───┐        ┌───┬───┬───┬───┐
 │ 1 │ 2 │ 3 │ C │        │ 1 │ 2 │ 3 │ 4 │
 ├───┼───┼───┼───┤        ├───┼───┼───┼───┤
 │ 4 │ 5 │ 6 │ D │        │ Q │ W │ E │ R │
 ├───┼───┼───┼───┤        ├───┼───┼───┼───┤
 │ 7 │ 8 │ 9 │ E │        │ A │ S │ D │ F │
 ├───┼───┼───┼───┤        ├───┼───┼───┼───┤
 │ A │ 0 │ B │ F │        │ Z │ X │ C │ V │
 └───┴───┴───┴───┘        └───┴───┴───┴───┘
```

Press <kbd>Esc</kbd> or close the window to quit.

## Finding ROMs

A great collection of CHIP-8 games, demos, and test programs is available at
[kripod/chip8-roms](https://github.com/kripod/chip8-roms). Download or clone the repository and
pass any `.ch8` file to the emulator:

```sh
git clone https://github.com/kripod/chip8-roms
./build/chip8 10 3 chip8-roms/games/Pong.ch8
```

## Supported opcodes

| Opcode | Instruction        | Description                                                        |
| ------ | ------------------ | ------------------------------------------------------------------ |
| `00E0` | `CLS`              | Clear the display.                                                 |
| `00EE` | `RET`              | Return from a subroutine.                                          |
| `1nnn` | `JP addr`          | Jump to address `nnn`.                                             |
| `2nnn` | `CALL addr`        | Call subroutine at address `nnn`.                                  |
| `3xkk` | `SE Vx, byte`      | Skip the next instruction if `Vx == kk`.                           |
| `4xkk` | `SNE Vx, byte`     | Skip the next instruction if `Vx != kk`.                           |
| `5xy0` | `SE Vx, Vy`        | Skip the next instruction if `Vx == Vy`.                           |
| `6xkk` | `LD Vx, byte`      | Set `Vx = kk`.                                                     |
| `7xkk` | `ADD Vx, byte`     | Set `Vx = Vx + kk`.                                                |
| `8xy0` | `LD Vx, Vy`        | Set `Vx = Vy`.                                                     |
| `8xy1` | `OR Vx, Vy`        | Set `Vx = Vx OR Vy`.                                               |
| `8xy2` | `AND Vx, Vy`       | Set `Vx = Vx AND Vy`.                                              |
| `8xy3` | `XOR Vx, Vy`       | Set `Vx = Vx XOR Vy`.                                              |
| `8xy4` | `ADD Vx, Vy`       | Set `Vx = Vx + Vy`, with `VF` set to the carry.                    |
| `8xy5` | `SUB Vx, Vy`       | Set `Vx = Vx - Vy`, with `VF` set to 1 if there is no borrow.      |
| `8xy6` | `SHR Vx`           | Shift `Vx` right by 1; `VF` gets the least significant bit.        |
| `8xy7` | `SUBN Vx, Vy`      | Set `Vx = Vy - Vx`, with `VF` set to 1 if there is no borrow.      |
| `8xyE` | `SHL Vx`           | Shift `Vx` left by 1; `VF` gets the most significant bit.          |
| `9xy0` | `SNE Vx, Vy`       | Skip the next instruction if `Vx != Vy`.                           |
| `Annn` | `LD I, addr`       | Set `I = nnn`.                                                     |
| `Bnnn` | `JP V0, addr`      | Jump to address `nnn + V0`.                                        |
| `Cxkk` | `RND Vx, byte`     | Set `Vx = random byte AND kk`.                                     |
| `Dxyn` | `DRW Vx, Vy, n`    | Draw an `n`-byte sprite at (`Vx`, `Vy`); `VF` is set on collision. |
| `Ex9E` | `SKP Vx`           | Skip the next instruction if the key in `Vx` is pressed.           |
| `ExA1` | `SKNP Vx`          | Skip the next instruction if the key in `Vx` is not pressed.       |
| `Fx07` | `LD Vx, DT`        | Set `Vx` to the value of the delay timer.                          |
| `Fx0A` | `LD Vx, K`         | Wait for a key press and store the key value in `Vx`.              |
| `Fx15` | `LD DT, Vx`        | Set the delay timer to `Vx`.                                       |
| `Fx18` | `LD ST, Vx`        | Set the sound timer to `Vx`.                                       |
| `Fx1E` | `ADD I, Vx`        | Set `I = I + Vx`.                                                  |
| `Fx29` | `LD F, Vx`         | Set `I` to the location of the hex sprite for digit `Vx`.          |
| `Fx33` | `LD B, Vx`         | Store the BCD digits of `Vx` at `I`, `I + 1`, and `I + 2`.         |
| `Fx55` | `LD [I], Vx`       | Store registers `V0` through `Vx` in memory starting at `I`.       |
| `Fx65` | `LD Vx, [I]`       | Load registers `V0` through `Vx` from memory starting at `I`.      |

## Project structure

```text
chip8/
├── CMakeLists.txt      # Build configuration
├── include/            # Headers
│   ├── addresses.h     # Memory layout constants
│   ├── chip8.h         # Emulator state and API
│   ├── fontset.h       # Built-in hex font
│   ├── platform.h      # SDL3 platform layer
│   └── utils.h         # Helper utilities
├── src/
│   ├── main.c          # Entry point and main loop
│   ├── chip8.c         # CPU, memory, and opcodes
│   ├── platform.c      # SDL3 window, rendering, and input
│   └── utils.c         # Helper utilities
└── ROMs/               # Test ROMs
```
