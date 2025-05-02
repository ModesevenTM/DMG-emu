#pragma once
#include "Memory.h"
class MBC2 : public Memory
{
public:
	MBC2(uint8_t* rom, uint8_t romBanks, uint8_t ramBanks, uint8_t* exram);
	~MBC2();
	uint8_t read8(uint16_t add);
	void write8(uint16_t add, uint8_t val);

	uint8_t* exram = nullptr;
	uint8_t romBanks = 0;
	uint8_t ramBanks = 0;

	uint8_t RAMG = 0;
	uint8_t ROMB = 1;
};

