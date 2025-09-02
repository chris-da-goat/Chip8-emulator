# Chip8-emulator

A simple CHIP-8 emulator written in C++ using SDL3 for graphics and input.
This project implements the full CHIP-8 instruction set, including timers, input handling, and sprite drawing, allowing classic CHIP-8 ROMs to be played.
This was really fun to make and I plan to move onto making more complex emulators at some point in the future such as a gameboy emulator.


## Features
  * Full CHIP-8 instruction set (35 opcodes implemented)
  * Classic 16-key hexadecimal keypad mapped to the keyboard
  * SDL3 rendering with scalable window size
  * Cycle delay control for smoother or faster emulation
  * Collision detection and sprite drawing with XOR logic


## Key mappings
Original CHIP8:        Keyboard:
  1 2 3 C       →        1 2 3 4
  4 5 6 D       →        Q W E R
  7 8 9 E       →        A S D F
  A 0 B F       →        Z X C V


### Requirements

  * A C++17 (or newer) compiler
  * SDL3 installed

### Compile
'''
g++ c8main.cpp chip8.cpp c8display.cpp -o chip8 -lSDL3
'''

### Usage
'''
./chip8 <Scale> <Delay> <ROM>
Scale: Window scaling factor (e.g. 10 for 640×320 output)
Delay: Cycle delay in ms (controls emulation speed)
ROM: Path to a CHIP-8 ROM file

Example: ./chip8emu 10 1 ./Downloads/pong.ch8
'''




