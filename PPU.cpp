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