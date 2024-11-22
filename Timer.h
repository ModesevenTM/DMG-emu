#pragma once
#include <cstdint>

class SM83;

class Timer
{
public:
	SM83* sm83;

	int DIVClock = 0;
	int TIMAClock = 0;

	uint8_t DIV = 0xAB; // 0xFF04 - TODO: MIGHT BE WRONG
	uint8_t TIMA = 0;	// 0xFF05
	uint8_t TMA = 0;	// 0xFF06
	uint8_t TAC = 0xF8;	// 0xFF07

	void update();
	void countCycles(int cycles);
};