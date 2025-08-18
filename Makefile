# Simple Makefile for building the CHIP-8 emulator with MinGW g++ and SDL2 on Windows

# --- Configuration for PowerShell ---
# This Makefile assumes you have installed SDL2 via MSYS2 pacman in the default location.
# If your MSYS2 is installed elsewhere, please update this path.
MSYS_MINGW_PREFIX := C:/msys64/mingw64
# ---

# Compiler / flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I"$(MSYS_MINGW_PREFIX)/include/SDL2"
LDFLAGS := -L"$(MSYS_MINGW_PREFIX)/lib"
LDLIBS := -lmingw32 -lSDL2main -lSDL2

# Sources / target
SRCS := src/main.cpp src/chip8.cpp
TARGET := chip8_emulator.exe

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	-$(RM) $(TARGET)
