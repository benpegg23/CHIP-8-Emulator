<div align="center">
    <img src="imgs/Chip-8_Banner.png" alt="CHIP-8 Emulator Banner">
</div>

This project is a fully functional emulator for the CHIP-8 system written in C++ to run `.ch8` roms. 

<div align="center">
    <img src="imgs/CHIP-8_Gameplay.gif" alt="CHIP-8 Gameplay" width="100%">
</div>

## Background

CHIP-8 is an interpreted programming language from the 70s that was used on 8-bit microcomputers like the COSMAC VIP and the Telmac 1800 to run games. I built this emulator as a way to strengthen my C++ skills and my understanding of computer architecture. 

## Features

-   **Instruction Set:** Implements all 35 CHIP-8 opcodes for full ROM compatibility.
-   **SDL2:** Uses SDL2 library to handle rendering, sound, and keyboard input.
-   **Display:** Renders the original 64x32 monochrome display in an upscaled 640x320 window.
-   **Keyboard:** Maps the 16-key hexadecimal keypad to a standard QWERTY layout for input.
-   **Memory:** 4 kB of virtual memory.
-   **Registers:** 16 8-bit general-purpose registers (`V0` to `VF`).
-   **Program Counter (PC):** Points to the next instruction to execute.
-   **Index Register (I):** Stores memory addresses for various operations.
-   **Stack:** A 16-level stack to handle subroutine calls.
-   **Timers:** Includes a delay timer and a sound timer that decrement at 60Hz.
-   **Clock Speed:** The CPU runs at approximately 720Hz. The main event loop is synchronized to the display's refresh rate to keep game speeds consistent.
-   **ROM Loading:** ROMs are loaded into the emulator by providing the file path as a command-line argument.
-   **Fontset:** The original hexadecimal font (0-F) required by CHIP-8 is pre-loaded into the emulator's memory.

## Building and Running

### Prerequisites

Before building, you must install the SDL2 development library. Using MSYS2 on windows:

1.  Open the `MSYS2 MINGW64` terminal.

2.  Install SDL2 using `pacman`:
    ```bash
    pacman -S mingw-w64-x86_64-SDL2
    ```

This setup ensures that the `sdl2-config` is in your `PATH`.

### Building

Navigate to the root directory in your terminal and run this command:
```
mingw32-make
```
This will generate an executable called `chip8_emulator.exe`.

### Running
To run games, run the executable with the file path of the `.ch8` ROM you want to run as an argument. 

While in the root directory, run this command:
```
./chip8_emulator ./roms/[name of the rom you want to run].ch8
```



