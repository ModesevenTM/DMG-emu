#pragma once
#include "Memory.h"
#include "Registers.h"
#include "Instructions.h"
#include "Interrupts.h"
#include "Timer.h"
#include "Joypad.h"
#include "PPU.h"

//class Memory;

class SM83
{
public:
	Memory* memory;
	Registers registers;
	Instructions instructions;
	Interrupts interrupts;
	Timer timer;
	Joypad joypad;
	PPU ppu;

	SM83(Memory* memory);

	int mCycles = (1 << 20);
	int tCycles = (1 << 22);
};