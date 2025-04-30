#pragma once
#include <cstdint>
#include <map>
#include <stdexcept>

class SM83;

class Interrupts
{
private:
public:
	SM83* sm83;

	bool EIqueued = false;
	bool IME = false;
	uint8_t IE = 0x00;	// 0xFFFF
	uint8_t IF = 0xE1;	// 0xFF0F
	const uint8_t MASK_INT_VBLANK = (1 << 0);
	const uint8_t MASK_INT_STAT = (1 << 1);
	const uint8_t MASK_INT_TIMER = (1 << 2);
	const uint8_t MASK_INT_SERIAL = (1 << 3);
	const uint8_t MASK_INT_JOYPAD = (1 << 4);

	const uint16_t INT_VBLANK = 0x40;
	const uint16_t INT_STAT = 0x48;
	const uint16_t INT_TIMER = 0x50;
	const uint16_t INT_SERIAL = 0x58;
	const uint16_t INT_JOYPAD = 0x60;

	const std::map<uint8_t, uint16_t> INT_ADDR = {
		{ MASK_INT_VBLANK, INT_VBLANK },
		{ MASK_INT_STAT, INT_STAT },
		{ MASK_INT_TIMER, INT_TIMER },
		{ MASK_INT_SERIAL, INT_SERIAL },
		{ MASK_INT_JOYPAD, INT_JOYPAD }
	};

	void EI();
	void DI();

	void requestInterrupt(uint8_t interrupt);
	void serviceInterrupt(uint8_t interrupt);
	bool checkInterrupts();
};

