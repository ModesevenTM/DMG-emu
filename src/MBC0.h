#pragma once
#include "Memory.h"
#include <string>

class MBC0 : public Memory
{
public:
	MBC0(uint8_t* rom) : Memory(rom) {};
};

