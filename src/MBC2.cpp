#include "MBC2.h"
#include <algorithm>

MBC2::MBC2(uint8_t* rom, uint16_t romBanks, uint8_t ramBanks, uint8_t* exram) : Memory(rom), romBanks(romBanks), ramBanks(ramBanks), exram(exram) {};

MBC2::~MBC2()
{
	if (exram) delete[] exram;
}

uint8_t MBC2::read8(uint16_t add)
{
	if (add < 0x4000)
		return rom[add];
	else if (add < 0x8000)
		return rom[add - 0x4000 + (ROMB % romBanks) * 0x4000];
	else if (add >= 0xA000 && add < 0xC000)
	{
		if (RAMG == 0x0A)
			return exram[(add - 0xA000) % 0x200] & 0x0F;
		else
			return 0xFF;
	}
	else
		return __super::read8(add);
}

void MBC2::write8(uint16_t add, uint8_t val)
{
	if (add < 0x4000)
	{
		if (add & 0x100)
			ROMB = std::max(val & 0x1F, 1);
		else
			RAMG = val & 0x0F;
	}
	else if (add >= 0xA000 && add < 0xC000)
		if (RAMG == 0x0A)
			exram[(add - 0xA000) % 0x200] = val & 0x0F;
		else
			return;
	else
		__super::write8(add, val);
}