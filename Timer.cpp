#include "Timer.h"
#include "SM83.h"

void Timer::update()
{
	while (DIVClock >= 64)
	{
		DIVClock -= 64;
		DIV++;
	}

	if (TAC & 0x04)
	{
		int freq = 0;
		switch (TAC & 0x03)
		{
		case 0:
			freq = 256;
			break;
		case 1:
			freq = 4;
			break;
		case 2:
			freq = 16;
			break;
		case 3:
			freq = 64;
			break;
		}
		while (TIMAClock >= freq)
		{
			TIMAClock -= freq;
			if (TIMA == 0xFF)
			{
				TIMA = TMA;
				sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_TIMER);
			}
			else
				TIMA++;
		}
	}

}

void Timer::countCycles(int cycles)
{
	frameClock += cycles;
	DIVClock += cycles;
	if (TAC & 0x04)
		TIMAClock += cycles;
}