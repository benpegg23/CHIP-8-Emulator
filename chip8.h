#ifndef CHIP8_H
#define CHIP8_H
#include <cstdint>

class Chip8 {
  public:
    Chip8();
    void initialize();
  private:
    uint8_t memory[4096]; // virtual memory
};

#endif