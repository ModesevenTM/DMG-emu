#include "PPU.h"
#include "SM83.h"

void PPU::compareLY_LYC()
{
	if (LY == LYC)
	{
		STAT |= 0x04;
		if (STAT & 0x40)
			sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_STAT);
	}
	else
		STAT &= 0xFB;
}

void PPU::step()
{
	int mode = STAT & 0x03;

	switch (mode)
	{
	case 2:
	{
		if (sm83->timer.frameClock >= MODE_2_CHECK)
		{
			STAT = (STAT & 0xFC) | 3;
		}
		break;
	}
	case 3:
	{
		if (sm83->timer.frameClock >= MODE_3_CHECK)
		{
			STAT = (STAT & 0xFC) | 0;
			if (STAT & 0x08)
				sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_STAT);
		}
		break;
	}
	case 0:
	{
		if (sm83->timer.frameClock >= ROW_TIME)
		{
			LY++;
			compareLY_LYC();
			if (LY == 144)
			{
				frameReady = true;
				STAT = (STAT & 0xFC) | 1;
				if (STAT & 0x10)
					sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_STAT);
				sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_VBLANK);
			}
			else {
				STAT = (STAT & 0xFC) | 2;
				if (STAT & 0x20)
					sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_STAT);
			}
		}
		break;
	}
	case 1:
	{
		if (sm83->timer.frameClock >= ROW_TIME)
		{
			LY++;
			compareLY_LYC();
			if (LY == 154)
			{
				LY = 0;
				STAT = (STAT & 0xFC) | 2;
				if (STAT & 0x20)
					sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_STAT);
			}
		}
		break;
	}
	}

	sm83->timer.frameClock %= ROW_TIME;
}