#include "MBC3.h"
#include <algorithm>

MBC3::MBC3(uint8_t* rom, uint8_t romBanks, uint8_t ramBanks, uint8_t* exram) : Memory(rom), romBanks(romBanks), ramBanks(ramBanks), exram(exram) {};

MBC3::~MBC3()
{
	if (exram) delete[] exram;
}

uint8_t MBC3::read8(uint16_t add)
{
	if (add < 0x4000)
		return rom[add];
	else if (add < 0x8000)
		return rom[add - 0x4000 + (ROMB % romBanks) * 0x4000];
	else if (add >= 0xA000 && add < 0xC000) // RAM or RTC
	{
		if (RAMG == 0x0A)
			if(RAMB < 0x08)
				return exram[add - 0xA000 + (RAMB % ramBanks) * 0x2000];
			else if (RAMB == 0x08)
				return RTC.S;
			else if (RAMB == 0x09)
				return RTC.M;
			else if (RAMB == 0x0A)
				return RTC.H;
			else if (RAMB == 0x0B)
				return RTC.DL;
			else if (RAMB == 0x0C)
				return RTC.DH;
			else
				return 0xFF;
		else
			return 0xFF;
	}
	else
		return __super::read8(add);
}

void MBC3::write8(uint16_t add, uint8_t val)
{
	if (add < 0x2000)
		RAMG = val & 0x0F;
	else if (add < 0x4000)
		ROMB = std::max(val & 0x7F, 1);
	else if (add < 0x6000)
		RAMB = val & 0x0F;
	else if (add < 0x8000)
	{
		LATCH_PREV = LATCH;
		LATCH = val;
		if (LATCH == 0x01 && LATCH_PREV == 0x00)
		{
			// handle RTC latch
		}
	}
	else if (add >= 0xA000 && add < 0xC000)
	{
		if (RAMG == 0x0A)
			if (RAMB < 0x08)
				exram[add - 0xA000 + (RAMB % ramBanks) * 0x2000] = val;
			else if (RAMB == 0x08)
				RTC.S = val;
			else if (RAMB == 0x09)
				RTC.M = val;
			else if (RAMB == 0x0A)
				RTC.H = val;
			else if (RAMB == 0x0B)
				RTC.DL = val;
			else if (RAMB == 0x0C)
				RTC.DH = val;
	}
	else
		__super::write8(add, val);
}