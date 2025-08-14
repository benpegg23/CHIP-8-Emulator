#include "chip8.h"
#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]){
  Chip8 chip8;

  if (argc != 2){
    std::cout << "No ROMs\n";
    return 1;
  }

  chip8.load_rom(argv[1]);

  // pointers to SDL window, renderer, and event
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Event event;

  // initalize SDL, error checking
  if (SDL_Init(SDL_INIT_VIDEO) != 0){ 
    std::cout << "SDL error: " << SDL_GetError() << "\n";
    return 1;
  }

  // create SDL window, use 640x320 so window is bigger, each 10x10 block represents 1 pixel
  window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN);

  // error checking for window
  if (!window){
    std::cout << "SDL error while creating window: " << SDL_GetError() << "\n";
    return 1;
  }

  // create SDL renderer, -1 as index means that SDL will choose render driver, accelerated uses gpu, presentvsync adjusts for display refresh rate
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // error checking for renderer
  if (!renderer){
    std::cout << "SDL error while creating renderer: " << SDL_GetError() << "\n";
    return 1;
  }
  
  // event loop
  bool running = true;
  while(running){
    while (SDL_PollEvent(&event)){
      if (event.type == SDL_QUIT){
        running = false;
      }
    }
    SDL_SetRenderDrawColor()

  }
  



  chip8.debug();
  for (int i = 0; i < 20; i++){
    chip8.instruction_cycle();
  }
  chip8.debug();


  return 0;
}