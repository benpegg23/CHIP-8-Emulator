#ifndef CHIP8_H
#define CHIP8_H
#include <cstdint>

class Chip8 {
  public:
    Chip8();
    void initialize();
    void instruction_cycle();
    void load_rom(const char *file);
    void debug();
    // uint16_t get_PC(); ***testing***
  private:
    uint8_t memory[4096]; // virtual memory
    uint8_t V[16]; // registers (V0 - VF)
    uint16_t PC; // program counter, points to next instruction in CPU cycle
    uint16_t I; // index register, holds addresses for memory operations
    uint16_t stack[16]; // limit stack size to 16 because we don't have that many nested subroutine calls
    uint16_t SP; // top of stack pointer
    uint8_t delay_timer; // decrement by one at 60 Hz
    uint8_t sound_timer; // also decrement at 60 Hz, beep sound when > 0 
    bool display[64][32]; // display, use booleans to represent on/off pixels
    bool keypad[16]; 
};

#endif