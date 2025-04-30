#pragma once
#include <cstdint>

class Registers
{
public:
	const uint8_t AF_NOFLAG = 0x00;// no flag, used for JR instruction only
	const uint8_t AF_C = (1 << 4); // carry flag
	const uint8_t AF_H = (1 << 5); // half carry flag
	const uint8_t AF_N = (1 << 6); // subtract flag
	const uint8_t AF_Z = (1 << 7); // zero flag
	
	uint16_t pc = 0x0100;
	uint16_t sp = 0xFFFE;

	union {
		struct {
			uint8_t f;
			uint8_t a;
		};
		uint16_t af;
	};

	union {
		struct {
			uint8_t c;
			uint8_t b;
		};
		uint16_t bc;
	};

	union {
		struct {
			uint8_t e;
			uint8_t d;
		};
		uint16_t de;
	};

	union {
		struct {
			uint8_t l;
			uint8_t h;
		};
		uint16_t hl;
	};

	Registers();

	void setFlag(uint8_t flag, bool value);
	bool getFlag(uint8_t flag);
};

