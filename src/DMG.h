#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "Memory.h"
#include "MBC0.h"
#include "MBC1.h"
#include "MBC2.h"
#include "MBC3.h"
#include "SM83.h"
#include "PPU.h"
#include "Renderer.h"

class DMG
{
private:
	uint16_t START_POINT = 0x100;
public:	
	SM83* sm83 = nullptr;
	Memory* memory = nullptr;
	PPU* ppu = nullptr;
	Renderer* renderer = nullptr;
	uint16_t romBanks;
	uint8_t ramBanks;
	uint8_t mbc;
	uint8_t* exram = nullptr;

	std::string title;

	DMG(std::string file);
	~DMG();

	void start();
	void printInfo();
	void saveGame();
	void printMBCType();
};

