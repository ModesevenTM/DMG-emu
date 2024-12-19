#pragma once
#include <cstdint>

class SM83;

class Memory
{
public:
	uint8_t* rom;
	uint8_t vram[0x2000];
	uint8_t wram[0x2000];
	uint8_t oam[0xA0];
	uint8_t hram[0x7F];

	uint8_t OAMDMA = 0xFF;	// 0xFF46
	uint8_t BOOT = 0x01;	// 0xFF50

	SM83* sm83;

	Memory(uint8_t* rom);
	virtual ~Memory();

	virtual uint8_t read8(uint16_t add);
	virtual void write8(uint16_t add, uint8_t val);
	uint16_t read16(uint16_t add);
	void write16(uint16_t add, uint16_t val);
	void oamDMA(uint8_t src);
};

