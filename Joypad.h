#pragma once
#include <cstdint>
#include <SDL.h>

class SM83;

class Joypad
{
private:
	const uint8_t KEY_A_RIGHT = (1 << 0);
	const uint8_t KEY_B_LEFT = (1 << 1);
	const uint8_t KEY_SELECT_UP = (1 << 2);
	const uint8_t KEY_START_DOWN = (1 << 3);
	const uint8_t SELECT_DPAD = (1 << 4);
	const uint8_t SELECT_BUTTONS = (1 << 5);

public:
	SM83* sm83;
	uint8_t P1 = 0xCF; // 0xFF00
	uint8_t DPad = 0xFF;
	uint8_t buttons = 0xFF;
	void keyDown(uint8_t key, uint8_t select);
	void keyUp(uint8_t key, uint8_t select);
	void step();
};

