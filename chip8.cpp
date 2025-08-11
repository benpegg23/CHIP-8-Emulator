#include "chip8.h"
#include <cstring>
#include <iostream>

Chip8::Chip8(){
  initialize();
}

void Chip8::initialize(){
  std::memset(memory, 0, sizeof(memory)); // initialize virtual memory
  uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
  // loading the font into memory, 0x050 to 0x09F (virtual memory location) is conventional location
  for (int i = 0; i < 80; i++){ 
    memory[0x050 + i] = font[i];
  }
  // initialize stack pointer to bottom of stack
  SP = 0;
  // initialize display to be off
  std::memset(display, 0, sizeof(display));
  // initialize keys to be not pressed
  std::memset(keypad, 0, sizeof(keypad));
  // initialize the PC to point to start of where chip-8 programs begin
  PC = 0x200;
  // initalize I, timers, stack, instruction, and registers
  I = 0;
  delay_timer = 0;
  sound_timer = 0;
  std::memset(stack, 0, sizeof(stack));
  std::memset(V, 0, sizeof(V));
}

void Chip8::instruction_cycle(){
  // fetch (instructions are 16 bits)
  uint16_t instruction = memory[PC]; 
  instruction = instruction << 8; 
  instruction += memory[PC+1]; 
  PC += 2;
  // decode and execute
  // x: second nibble, access register Vx
  // y: third nibble, access register Vy
  // N: fourth nibble, 4-bit number
  // NN: third, fourth nibble (second byte), 8-bit number
  // NNN: second, third, fourth nibble, 12-bit number/address
  // use bitmasks and shifts to isolate the right nibble(s)
  uint8_t x = (instruction & 0x0F00) >> 4*2; 
  uint8_t y = (instruction & 0x00F0) >> 4*1;
  uint8_t N = instruction & 0x000F;
  uint8_t NN = instruction & 0x00FF;
  uint16_t NNN = instruction & 0x0FFF;
  switch((instruction & 0xF000) >> 4*3) {
    case 0x0: // {0NNN, 00E0, 00EE}
      if (N == 0x0) { // 00E0 (clear screen)
        std::memset(display, 0, sizeof(display));
      } /*else if (N == 0xE){ // 00EE

      } else { // 0NNN

      }*/
      break;
    case 0x1: // {1NNN} 
      PC = NNN; // (jump)
      break;
    case 0x2: // {2NNN}

      break;
    case 0x3: // {3XNN}

      break;
    case 0x4: // {4XNN}

      break;
    case 0x5: // {5XY0}

      break;
    case 0x6: // {6XNN}
      V[x] = NN; // (set)
      break;
    case 0x7: // {7XNN}
      V[x] += NN; // (add)
      break;
    case 0x8: // {8XY0, 8XY1, 8XY2, 8XY3, 8XY4, 8XY5, 8XY6, 8XY7, 8XYE}

      break;
    case 0x9: // {9XY0}

      break;
    case 0xA: // {ANNN}
      I = NNN; // (set index)
      break;
    case 0xB: // {BNNN}

      break;
    case 0xC: // {CXNN}

      break;
    case 0xD: // {DXYN}
      uint8_t x_start = V[x] % 64;
      uint8_t y_start = V[y] % 32;
      V[0xF] = 0;
      int8_t sprite;
      uint8_t pixel;
      for (int row = 0; row < N; row++){
        if (row + y_start >= 32){ // check for clipping at bottom
          break;
        }
        sprite = memory[I + row];
        for (int col = 0; col < 8; col++){
          if (col + x_start >= 64){ // check for clipping at right side
            break;
          }
          if (sprite < 0){ // find msb of sprite
            pixel = 1;
          } else {
            pixel = 0;
          }
          sprite <<= 1;
          if (pixel && display[row + y_start][col + x_start]){ // pixel and display on at same position
            V[0xF] = 1;
          }
          display[row + y_start][col + x_start] = pixel ^ display[row + y_start][col + x_start]; // use XOR to turn pixels on/off
        }
      }

      break;
    case 0xE: // {EX9E, EXA1}

      break;
    case 0xF: // {FX07, FX15, FX18, FX1E, FX0A, FX29, FX33, FX55, FX65}

      break;
    default: 
      std::cout << "Unknown opcode: " << instruction << "\n";
      
  }

}

/*
*** Used for testing ***
uint16_t Chip8::get_PC(){
  return PC;
}
*/
