#pragma once
#include <cstdint>

class SM83;

class Audio
{
public:
	uint8_t regs[0x30] = {
		0x80, 0xBF, 0xF3, 0xFF, 0xBF, // NR10-NR14
		0,
		0x3F, 0x00, 0xFF, 0xBF, // NR21-NR24
		0x7F, 0xFF, 0x9F, 0xFF, 0xBF, // NR30-NR34
		0,
		0xFF, 0, 0, 0xBF, // NR41-NR44
		0x77, 0xF3, 0xF1, // NR50-NR52
		0, 0, 0, 0, 0, 0, 0, 0, 0, // unused
		0, 0, 0, 0, 0, 0, 0, 0,	// WAV01-07
		0, 0, 0, 0, 0, 0, 0, 0	// WAV08-15
	};
	
	// register pointers
	uint8_t* NR1 = &regs[0x00];	// Channel 1
	uint8_t* NR2 = &regs[0x06];	// Channel 2
	uint8_t* NR3 = &regs[0x0A];	// Channel 3
	uint8_t* NR4 = &regs[0x10];	// Channel 4
	uint8_t* NR5 = &regs[0x14];	// Control registers
	uint8_t* WAV = &regs[0x20];	// Wave Pattern RAM

	SM83* sm83 = nullptr;

	void step();

	Audio() {};
};