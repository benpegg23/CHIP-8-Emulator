#include "chip8.h"
#include <iostream>

int main(int argc, char* argv[]){
  Chip8 chip8;

  if (argc != 2){
    std::cout << "No ROMs\n";
    return 1;
  }

  chip8.load_rom(argv[1]);

  chip8.debug();
  for (int i = 0; i < 10; i++){
    chip8.instruction_cycle();
  }
  chip8.debug();

  /* 
  *** Used this for testing ***
  if(chip8.get_PC() == 0x200){
    cout << "Success!\n";
  } else {
    cout << "Failure :(\n";
    cout << chip8.get_PC();
    cout << "\n";
  */

}