#include "chip8.h"
#include <cstring>

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
  // decode


}

/*
*** Used for testing ***
uint16_t Chip8::get_PC(){
  return PC;
}
*/
