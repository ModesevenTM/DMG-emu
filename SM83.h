#pragma once
#include "Memory.h"
#include "Registers.h"
#include "Instructions.h"
#include "Interrupts.h"
#include "Timer.h"
#include "Joypad.h"
#include "PPU.h"

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

	bool halted = false;

	SM83(Memory* memory);

	void step();

	int mCycles = (1 << 20);
	int tCycles = (1 << 22);
};