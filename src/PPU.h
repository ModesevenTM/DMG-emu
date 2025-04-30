#pragma once
#include <cstdint>
#include <queue>

class SM83;

class PPU
{
private:
	union Color 
	{
		struct {
			uint8_t r, g, b, a;
		};
		uint8_t col[4];
	};

	struct Object
	{
		uint8_t& y;
		uint8_t& x;
		uint8_t& tileNum;
		uint8_t& flags;
	};
public:
	const int MODE_2_CHECK = 20;
	const int MODE_3_CHECK = 63;
	const int ROW_TIME = 114;
	const int FRAME_TIME = 17556;

	const Color COLORS[4] = {
		{ 255, 255, 255, 255 },
		{ 192, 192, 192, 255 },
		{ 96, 96, 96, 255 },
		{ 0, 0, 0, 255 }
	};

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

	bool frameReady = false;

	std::queue<Object> objQueue;
	uint8_t frameBuffer[160 * 144 * 4] = {};
	uint8_t colorVals[160] = {};

	SM83* sm83;

	void compareLY_LYC();
	void step();
	void oamScan();
	void renderScanline();
	void renderBlankScanline();
	void renderScanlineBG();
	void renderScanlineWindow();
	void renderScanlineOBJ();
};

