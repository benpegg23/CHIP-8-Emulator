#include "chip8.h"
#include <SDL2/SDL.h>
#include <iostream>

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
    while (SDL_PollEvent(&event)){
      // check for quitting
      if (event.type == SDL_QUIT){
        running = false;
      }
      /* ====KEY MAPPINGS====
          =CHIP8=     =QWERTY=
          1 2 3 C     1 2 3 4 
          4 5 6 D     Q W E R
          7 8 9 E     A S D F
          A 0 B F     Z X C V
      */
      // check for keypresses
      if (event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym){
          case SDLK_1:
            chip8.set_keypad(0x1, true);
            break;
          case SDLK_2:
            chip8.set_keypad(0x2, true);
            break;
          case SDLK_3:
            chip8.set_keypad(0x3, true);
            break;
          case SDLK_4:
            chip8.set_keypad(0xC, true);
            break;
          case SDLK_q:
            chip8.set_keypad(0x4, true);
            break;
          case SDLK_w:
            chip8.set_keypad(0x5, true);
            break;
          case SDLK_e:
            chip8.set_keypad(0x6, true);
            break;
          case SDLK_r:
            chip8.set_keypad(0xD, true);
            break;
          case SDLK_a:
            chip8.set_keypad(0x7, true);
            break;
          case SDLK_s:
            chip8.set_keypad(0x8, true);
            break;
          case SDLK_d:
            chip8.set_keypad(0x9, true);
            break;
          case SDLK_f:
            chip8.set_keypad(0xE, true);
            break;
          case SDLK_z:
            chip8.set_keypad(0xA, true);
            break;
          case SDLK_x:
            chip8.set_keypad(0x0, true);
            break;
          case SDLK_c:
            chip8.set_keypad(0xB, true);
            break;
          case SDLK_v:
            chip8.set_keypad(0xF, true);
            break;
        } 
      }
      if (event.type == SDL_KEYUP){
        switch (event.key.keysym.sym){
          case SDLK_1:
            chip8.set_keypad(0x1, false);
            break;
          case SDLK_2:
            chip8.set_keypad(0x2, false);
            break;
          case SDLK_3:
            chip8.set_keypad(0x3, false);
            break;
          case SDLK_4:
            chip8.set_keypad(0xC, false);
            break;
          case SDLK_q:
            chip8.set_keypad(0x4, false);
            break;
          case SDLK_w:
            chip8.set_keypad(0x5, false);
            break;
          case SDLK_e:
            chip8.set_keypad(0x6, false);
            break;
          case SDLK_r:
            chip8.set_keypad(0xD, false);
            break;
          case SDLK_a:
            chip8.set_keypad(0x7, false);
            break;
          case SDLK_s:
            chip8.set_keypad(0x8, false);
            break;
          case SDLK_d:
            chip8.set_keypad(0x9, false);
            break;
          case SDLK_f:
            chip8.set_keypad(0xE, false);
            break;
          case SDLK_z:
            chip8.set_keypad(0xA, false);
            break;
          case SDLK_x:
            chip8.set_keypad(0x0, false);
            break;
          case SDLK_c:
            chip8.set_keypad(0xB, false);
            break;
          case SDLK_v:
            chip8.set_keypad(0xF, false);
            break;
        }
      }
    }

    // decrement timer
    chip8.update_timers();
    
    // execute 12 instruction cycles per frame to reach cpu clock speed of 720 Hz
    if (!chip8.get_halted()){
      for (int i = 0; i < 12; i++){
        chip8.instruction_cycle();
      }
    } else {
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
  */
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  

  return 0;
}

