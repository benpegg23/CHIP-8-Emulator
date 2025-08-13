#include "chip8.h"
#include <iostream>
#include <SDL.h>

int main(int argc, char* argv[]){
  Chip8 chip8;

  if (argc != 2){
    std::cout << "No ROMs\n";
    return 1;
  }

  chip8.load_rom(argv[1]);

  chip8.debug();
  for (int i = 0; i < 20; i++){
    chip8.instruction_cycle();
  }
  chip8.debug();


  return 0;
}