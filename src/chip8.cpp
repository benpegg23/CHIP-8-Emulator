#include "chip8.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>

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
  /* decode and execute
   * x: second nibble, access register Vx
   * y: third nibble, access register Vy
   * N: fourth nibble, 4-bit number
   * NN: third, fourth nibble (second byte), 8-bit number
   * NNN: second, third, fourth nibble, 12-bit number/address
   * use bitmasks and shifts to isolate the right nibble(s)
  */
  uint8_t x = (instruction & 0x0F00) >> 4*2; 
  uint8_t y = (instruction & 0x00F0) >> 4*1;
  uint8_t N = instruction & 0x000F;
  uint8_t NN = instruction & 0x00FF;
  uint16_t NNN = instruction & 0x0FFF;
  switch((instruction & 0xF000) >> 4*3) {
    case 0x0: // {0NNN, 00E0, 00EE}
      if (NNN == 0x0E0) { // 00E0 (clear screen)
        std::memset(display, 0, sizeof(display));
      } else if (NNN == 0x0EE){ // 00EE (return from subroutine using a stack pop)
        PC = stack[--SP];
      } 
      break;
    case 0x1: // {1NNN} (jump)
      PC = NNN; 
      break;
    case 0x2: // {2NNN} (call subroutine)
      stack[SP] = PC;
      SP++;
      PC = NNN;
      break;
    case 0x3: // {3XNN} (skip if Vx = NN)
      if (V[x] == NN){
        PC += 2;
      }
      break;
    case 0x4: // {4XNN} (skip if Vx != NN)
      if (V[x] != NN){
        PC += 2;
      }
      break;
    case 0x5: // {5XY0} (skip if Vx = Vy)
      if (V[x] == V[y]){
        PC += 2;
      }
      break;
    case 0x6: // {6XNN}
      V[x] = NN; // (set)
      break;
    case 0x7: // {7XNN}
      V[x] += NN; // (add)
      break;
    case 0x8: { // {8XY0, 8XY1, 8XY2, 8XY3, 8XY4, 8XY5, 8XY6, 8XY7, 8XYE}
      uint8_t temp_x;
      uint8_t temp_y;
      bool carry;
      if (N == 0){ // 8XY0 (set Vx = Vy)
        V[x] = V[y];
      } else if (N == 0x1){ // 8XY1 (Vx = Vx OR Vy)
        V[x] = V[x] | V[y];
      } else if (N == 0x2){ // 8XY2 (Vx = Vx AND Vy)
        V[x] = V[x] & V[y];
      } else if (N == 0x3){ // 8XY3 (Vx = Vx XOR Vy)
        V[x] = V[x] ^ V[y];
      } else if (N == 0x4){ // 8XY4 (Vx = Vx + Vy)
        carry = (V[x] + V[y] > 255);
        V[x] += V[y];
        V[0xF] = carry;
      } else if (N == 0x5){ // 8XY5 (Vx = Vx - Vy)
        carry = (V[x] >= V[y]);
        V[x] -= V[y];
        V[0xF] = carry;
      } else if (N == 0x6){ // 8XY6 (Right Shift)
        //V[x] = V[y]; **configurable, some games require that Vx is set to Vy before shifting**
        carry = (V[x] & 0x0001);
        V[x] >>= 1; 
        V[0xF] = carry;
      } else if (N == 0x7){ // 8XY7 (Vx = Vy - Vx)
        carry = (V[y]>= V[x]);
        V[x] = V[y] - V[x];
        V[0xF] = carry;
      } else if (N == 0xE){ // 8XYE (Left Shift)
        //V[x] = V[y]; //**configurable, some games require that Vx is set to Vy before shifting**
        carry = (V[x] & 0x0080);
        V[x] <<= 1;
        V[0xF] = carry;
      } 
      break;
    }
    case 0x9: // {9XY0} (skip if Vx != Vy)
      if (V[x] != V[y]){
        PC += 2;
      }
      break;
    case 0xA: // {ANNN} (set index)
      I = NNN;
      break;
    case 0xB: // {BNNN} (jump with offset) 
      PC = NNN + V[0];
      break;
    case 0xC: { // {CXNN} (random number)
      int random = rand() % (NN + 1);
      V[x] = random & NN;
      break;
    }
    case 0xD:{ // {DXYN} (draw)
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
    }
    
    case 0xE: // {EX9E, EXA1}
      if (NN == 0x9E){ // EX9E (skip if Vx key pressed)
        if (keypad[V[x]]){
          PC += 2;
        }
      } else if (NN == 0xA1){ // EXA1 (skip if Vx key not pressed)
        if (!keypad[V[x]]){
          PC += 2;
        }
      }
      break;
    case 0xF: // {FX07, FX15, FX18, FX1E, FX0A, FX29, FX33, FX55, FX65}
      if (NN == 0x07){ // FX07 (set Vx to delay timer)
        V[x] = delay_timer;
      } else if (NN == 0x15){ // FX15 (set delay timer to Vx)
        delay_timer = V[x];
      } else if (NN == 0x18){ // FX18 (set sound timer to Vx)
        sound_timer = V[x];
      } else if (NN == 0x1E){ // FX1E (add Vx to index)
        I += V[x];
        if (I >= 0x1000){
          V[0xF] = 1;
        } else {
          V[0xF] = 0;
        }
      } else if (NN == 0x0A){ // FX0A (get key)
        bool key_pressed = false;
        for (int i = 0; i < sizeof(keypad); i++){
          if (keypad[i]) {
            key_pressed = true;
            V[x] = i;
            break;
          }
        }
        if (!key_pressed){
          PC -= 2;
        }
      } else if (NN == 0x29){ // FX29 (font character)
        I = 0x50 + V[x] * 5;
      } else if (NN == 0x33){ // FX33 (binary-coded decimal conversion)
        memory[I] = V[x] / 100; // (truncates cuz memory is int array)
        memory[I + 1] = (V[x] / 10) % 10;
        memory[I + 2] = V[x] % 10;
      } else if (NN == 0x55){ // FX55 (store memory)
        for (int j = 0; j <= x; j ++){
          memory[I + j] = V[j];
          // memory[I + j + 1] = V[j]; // (ambiguous instruction, used for old ROMs)
        }


      } else if (NN == 0x65){ // FX65 (load memory)
        for (int j = 0; j <= x; j ++){
          V[j] = memory[I + j];
          // V[j] = memory[I + j + 1]; // (ambiguous instruction, used for old ROMs)
        }
      } 
      break;
    default: 
      std::cout << "Unknown opcode: " << instruction << "\n";
      exit(1);
      
  }

}

void Chip8::load_rom(const char *file){
  std::ifstream rom_file(file, std::ios::binary);
  if (!rom_file){
    std::cout << "ROM file load error";
    return;
  }
  rom_file.seekg(0, std::ios::end);
  uint16_t rom_size = rom_file.tellg();
  rom_file.seekg(0);
  rom_file.read((char*)&memory[0x200], rom_size);
  rom_file.close();
}

void Chip8::debug(){
  for (int i = 0; i < 16; i++){
    std::cout << "V" << i << ": 0x" << std::hex << (int)V[i] << "\n";
  }
}

const bool* Chip8::get_display(){
  return &display[0][0];
}


/*
*** Used for testing ***
uint16_t Chip8::get_PC(){
  return PC;
}
*/
