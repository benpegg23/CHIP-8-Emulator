#include "chip8.h"
#include <iostream>
#include <SDL2/SDL.h>

void draw(Chip8 *chip8_obj, SDL_Renderer *renderer_ptr){
  SDL_SetRenderDrawColor(renderer_ptr, 255, 255, 255, 255);
  const bool* pixel = chip8_obj->get_display();
  SDL_Rect rect;
  rect.w = 10;
  rect.h = 10;
  for (int y = 0; y < 32; y++){
    for (int x = 0; x < 64; x++){
      if (*(pixel + 64*y + x)) {
        rect.x = x*10;
        rect.y = y*10;
        SDL_RenderFillRect(renderer_ptr, &rect);
      }
    }
  }
}

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
    // check for quitting
    while (SDL_PollEvent(&event)){
      if (event.type == SDL_QUIT){
        running = false;
      }
    }

    // execute 12 instruction cycles per frame to reach cpu clock speed of 720 Hz
    for (int i = 0; i < 12; i++){
      chip8.instruction_cycle();
    }

    // clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw stuff
    draw(&chip8, renderer);

    // update window
    SDL_RenderPresent(renderer);

    // add delay of 16 ms to refresh at 60 Hz
    SDL_Delay(16);

  }
  
  /*
  chip8.debug();
  for (int i = 0; i < 20; i++){
    chip8.instruction_cycle();
  }
  chip8.debug();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  */

  return 0;
}

