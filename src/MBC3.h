#pragma once
#include "Memory.h"
class MBC3 : public Memory
{
public:
	MBC3(uint8_t* rom, uint8_t romBanks, uint8_t ramBanks, uint8_t* exram);
	~MBC3();
	uint8_t read8(uint16_t add);
	void write8(uint16_t add, uint8_t val);

	uint8_t* exram = nullptr;
	uint8_t romBanks = 0;
	uint8_t ramBanks = 0;

	uint8_t RAMG = 0;
	uint8_t ROMB = 1;
	uint8_t RAMB = 0;

	uint8_t LATCH = 1;
	uint8_t LATCH_PREV = 0;

	struct {
		uint8_t S = 0;
		uint8_t M = 0;
		uint8_t H = 0;
		uint8_t DL = 0;
		uint8_t DH = 0;
	} RTC;;
};