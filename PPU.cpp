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
	switch (STAT & 0x03)
	{
	case 2: // OAM Scan
	{
		if (sm83->timer.frameClock >= MODE_2_CHECK)
		{
			oamScan();
			STAT = (STAT & 0xFC) | 3;
		}
		break;
	}
	case 3:	// Drawing
	{
		if (sm83->timer.frameClock >= MODE_3_CHECK)
		{
			renderScanline();
			if (LY == 143)
				frameReady = true;
			STAT = (STAT & 0xFC) | 0;
			if (STAT & 0x08)
				sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_STAT);
		}
		break;
	}
	case 0:	// HBlank
	{
		if (sm83->timer.frameClock >= ROW_TIME)
		{
			LY++;
			compareLY_LYC();
			if (LY == 144)
			{
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
	case 1:	// VBlank
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

void PPU::oamScan()
{
	while (!objQueue.empty())
		objQueue.pop();

	if (LCDC & 0x02)
	{
		for (int i = 0; i < 40; i++)
		{
			uint8_t y = sm83->memory->oam[i * 4];
			if (LY + 16 >= y && LY + 16 < y + (LCDC & 0x04 ? 16 : 8))
			{
				Object obj = { sm83->memory->oam[i * 4], sm83->memory->oam[i * 4 + 1], sm83->memory->oam[i * 4 + 2], sm83->memory->oam[i * 4 + 3] };
				objQueue.push(obj);
				if (objQueue.size() == 10)
					break;
			}
		}
	}
}

void PPU::renderScanline()
{
	renderScanlineBG();
	renderScanlineWindow();
	renderScanlineOBJ();
}

void PPU::renderScanlineBG()
{
	if (LCDC & 0x80)
	{
		uint16_t tileData, bgMap;
		bool signedData = false;

		if (LCDC & 0x10)
			tileData = 0x8000;
		else
		{
			tileData = 0x9000;
			signedData = true;
		}

		if (LCDC & 0x08)
			bgMap = 0x9C00;
		else
			bgMap = 0x9800;

		uint8_t y = LY + SCY;
		uint16_t tileRow = (y / 8) * 32;

		for (int x = 0; x < 160; x++)
		{
			uint8_t tileCol = (SCX + x) / 8;
			uint8_t tileNum = sm83->memory->read8(bgMap + tileRow + tileCol);
			uint16_t tileAddr = tileData + (signedData ? (int8_t)tileNum : tileNum) * 16;

			uint8_t line = y % 8;
			uint8_t data1 = sm83->memory->read8(tileAddr + line * 2);
			uint8_t data2 = sm83->memory->read8(tileAddr + line * 2 + 1);

			uint8_t colorBit = 7 - ((SCX + x) % 8);
			uint8_t colorNum = ((data2 >> colorBit) & 0x01) << 1;
			colorNum |= (data1 >> colorBit) & 0x01;

			Color color = COLORS[(BGP >> (colorNum * 2)) & 0x03];

			frameBuffer[(LY * 160 + x) * 4] = color.r;
			frameBuffer[(LY * 160 + x) * 4 + 1] = color.g;
			frameBuffer[(LY * 160 + x) * 4 + 2] = color.b;
			frameBuffer[(LY * 160 + x) * 4 + 3] = color.a;
		}
	}
}

void PPU::renderScanlineWindow()
{
	if (LCDC & 0x20)
	{
		uint16_t tileData, bgMap;
	 	bool signedData = false;
	
	 	if (LCDC & 0x10)
	 		tileData = 0x8000;
	 	else
	 	{
	 		tileData = 0x9000;
	 		signedData = true;
	 	}
	
	 	if (LCDC & 0x40)
	 		bgMap = 0x9C00;
	 	else
	 		bgMap = 0x9800;
	
	 	uint8_t y = LY + WY;
	 	uint16_t tileRow = (y / 8) * 32;
	
	 	for (int x = 0; x < 160; x++)
	 	{
	 		uint8_t tileCol = (WX - 7 + x) / 8;
	 		uint8_t tileNum = sm83->memory->read8(bgMap + tileRow + tileCol);
	 		uint16_t tileAddr = tileData + (signedData ? (int8_t)tileNum : tileNum) * 16;
	
	 		uint8_t line = y % 8;
	 		uint8_t data1 = sm83->memory->read8(tileAddr + line * 2);
	 		uint8_t data2 = sm83->memory->read8(tileAddr + line * 2 + 1);
	
	 		uint8_t colorBit = 7 - ((WX - 7 + x) % 8);
	 		uint8_t colorNum = ((data2 >> colorBit) & 0x01) << 1;
	 		colorNum |= (data1 >> colorBit) & 0x01;
	
	 		Color color = COLORS[(BGP >> (colorNum * 2)) & 0x03];
	
	 		frameBuffer[(LY * 160 + x) * 4] = color.r;
	 		frameBuffer[(LY * 160 + x) * 4 + 1] = color.g;
	 		frameBuffer[(LY * 160 + x) * 4 + 2] = color.b;
	 		frameBuffer[(LY * 160 + x) * 4 + 3] = color.a;
	 	}
	}
}

void PPU::renderScanlineOBJ()
{
	if (LCDC & 0x02)
	{
		while (!objQueue.empty())
		{
			Object obj = objQueue.front();
			objQueue.pop();
			if(obj.x == 0 || obj.x >= 168)
				continue;

			uint8_t height = LCDC & 0x04 ? 16 : 8;
			uint8_t y = LY - obj.y + 16;
			if (obj.flags & 0x40)
				y = height - y - 1;
			uint16_t tileAddr = 0x8000 + (height == 8 ? obj.tileNum : obj.tileNum & 0xFE | (y >= 8 ? 0x01 : 0)) * 16;

			for (int k = (obj.x < 8 ? 8 - obj.x : 0); k < (obj.x <= 160 ? 8 : 168 - obj.x); k++)
			{
				uint8_t colorBit = obj.flags & 0x20 ? k : 7 - k;
				uint8_t colorNum = ((sm83->memory->read8(tileAddr + (y % 8) * 2 + 1) >> colorBit) & 0x01) << 1;
				colorNum |= (sm83->memory->read8(tileAddr + (y % 8) * 2) >> colorBit) & 0x01;

				if (colorNum == 0)
					continue;

				Color color = COLORS[(obj.flags & 0x10) ? ((OBP1 >> (colorNum * 2)) & 0x03) : ((OBP0 >> (colorNum * 2)) & 0x03)];

				frameBuffer[(LY * 160 + (obj.x - 8) + k) * 4] = color.r;
				frameBuffer[(LY * 160 + (obj.x - 8) + k) * 4 + 1] = color.g;
				frameBuffer[(LY * 160 + (obj.x - 8) + k) * 4 + 2] = color.b;
				frameBuffer[(LY * 160 + (obj.x - 8) + k) * 4 + 3] = color.a;
			}
		}
	}
}