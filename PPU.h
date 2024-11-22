#pragma once
#include <cstdint>

class SM83;

class PPU
{
public:
	uint8_t LCDC = 0x91;	// 0xFF40
	uint8_t STAT = 0x85;	// 0xFF41
	uint8_t SCY = 0;		// 0xFF42
	uint8_t SCX = 0;		// 0xFF43
	uint8_t LY = 0;			// 0xFF44
	uint8_t LYC = 0;		// 0xFF45
	uint8_t BGP = 0xFC;		// 0xFF47
	uint8_t OBP0 = 0xFF;	// 0xFF48
	uint8_t OBP1 = 0xFF;	// 0xFF49
	uint8_t WY = 0;			// 0xFF4A
	uint8_t WX = 0;			// 0xFF4B

	SM83* sm83;

	void compareLY_LYC();
};

