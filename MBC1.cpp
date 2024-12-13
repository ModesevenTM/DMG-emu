#include "MBC1.h"
#include <algorithm>

MBC1::MBC1(uint8_t* rom, uint8_t romBanks, uint8_t ramBanks) : Memory(rom), romBanks(romBanks), ramBanks(ramBanks)
{
	if(ramBanks) exram = new uint8_t[ramBanks * 0x2000];
};

MBC1::~MBC1()
{
	if(exram) delete[] exram;
}

uint8_t MBC1::read8(uint16_t add)
{
	if (add < 0x4000)
		return rom[add + MODE * ((BANK2 << 5) % romBanks) * 0x4000];
	else if (add < 0x8000)
		return rom[add - 0x4000 + ((BANK1 | (BANK2 << 5)) % romBanks) * 0x4000];
	else if (add >= 0xA000 && add < 0xC000)
	{
		if (RAMG == 0x0A)
			return exram[add - 0xA000 + ((MODE * BANK2) % ramBanks) * 0x2000];
		else
			return 0xFF;
	}
	else
		__super::read8(add);
}

void MBC1::write8(uint16_t add, uint8_t val)
{
	if (add < 0x2000)
		RAMG = val & 0x0F;
	else if (add < 0x4000)
		BANK1 = std::max(val & 0x1F, 1);
	else if (add < 0x6000)
		BANK2 = val & 0x03;
	else if (add < 0x8000)
		MODE = val & 0x01;
	else if (add >= 0xA000 && add < 0xC000)
		if (RAMG == 0x0A)
			exram[add - 0xA000 + ((MODE * BANK2) % ramBanks) * 0x2000] = val; // check if RAM enabled
		else
			return;
	else
		__super::write8(add, val);
}