#pragma once
#include "Memory.h"
class MBC1 : public Memory
{
public:
	MBC1(uint8_t* rom, uint8_t romBanks, uint8_t ramBanks);
	~MBC1();
	uint8_t read8(uint16_t add);
	void write8(uint16_t add, uint8_t val);

	uint8_t* exram = nullptr;
	uint8_t romBanks = 0;
	uint8_t ramBanks = 0;

	uint8_t RAMG = 0;
	uint8_t BANK1 = 0;
	uint8_t BANK2 = 0;
	uint8_t MODE = 0;
};

