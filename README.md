<div align="center">
    <img src="imgs/Chip-8_Banner.png" alt="CHIP-8 Emulator Banner">
</div>

#### This project is a fully functional emulator for the CHIP-8 system written in C++ to run `.ch8` roms.

<div align="center">
    <img src="imgs/CHIP-8_Gameplay.gif" alt="CHIP-8 Gameplay" width="100%">
</div>

## Background

CHIP-8 is an interpreted programming language from the 70s that was used on 8-bit microcomputers like the COSMAC VIP and the Telmac 1800 to run games. I built this emulator as a way to strengthen my C++ skills and my understanding of computer architecture. 

## Features

-   **Instruction Set:** Implements all 35 CHIP-8 opcodes for full ROM compatibility.
-   **SDL:** Uses [SDL2](https://github.com/libsdl-org/SDL) library to handle rendering, sound, and keyboard input.
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

Before building, you must install SDL2. Using MSYS2 on windows:

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

The command looks like this (while in the root directory of the project):
```
./chip8_emulator ./roms/[name of the rom you want to run].ch8
```

## Keyboard Mapping
The keyboard is 16 keys in a 4x4 layout. The mapping looks like this:
```
|---------------|       |---------------|
| 1 | 2 | 3 | C |       | 1 | 2 | 3 | 4 |
|---|---|---|---|       |---|---|---|---|
| 4 | 5 | 6 | D |       | Q | W | E | R |
|---|---|---|---|  ==>  |---|---|---|---|
| 7 | 8 | 9 | E |       | A | S | D | F |
|---|---|---|---|       |---|---|---|---|
| A | 0 | B | F |       | Z | X | C | V |
|---------------|       |---------------|
```

## Architecture

The `Chip8` class encapsulates the virtual machine's state, including its 4 kB of virtual memory, 16 registers, a program counter, index register, stack, and display and sound timers. The `chip8.cpp` file initializes the virtual machine state and defines the `instruction_cycle()` method, which implements the fetch-decode-execute process for all 35 CHIP-8 opcodes. Additionally, `chip8.cpp` provides various getter and setter methods, along with `load_rom()` for loading `.ch8` ROMs into the emulator.

`main.cpp` serves as the emulator's core loop, handling SDL tasks such as window and renderer management, audio output, and user input during the emulator's runtime.


## Credits
-   [Guide to making a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) by [Tobias V. Langhoff](https://github.com/tobiasvl)

-   [CHIP-8 Test Suite](https://github.com/Timendus/chip8-test-suite) by [Timendus](https://github.com/Timendus)

-   8ce Attorney by [SystemLogoff](https://systemlogoff.com/index.php)

-   br8kout by SharpenedSpoon

-   Flight Runner by [TodPunk](https://github.com/TodPunk/)

-   Ghost Escape by [Tom Rintjema](https://github.com/TomRintjema/)

-   Outlaw by [John Earnst](https://github.com/JohnEarnest)

-   Snake by [John Earnst](https://github.com/JohnEarnest)

-   More ROMs available at [johnearnst.github.io](https://johnearnest.github.io/chip8Archive/?sort=platform)
