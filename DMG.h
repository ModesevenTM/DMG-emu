#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include "Memory.h"
#include "MBC0.h"
#include "MBC1.h"
#include "SM83.h"

class DMG
{
private:
	uint16_t START_POINT = 0x100;
public:	
	SM83* sm83 = nullptr;
	Memory* memory = nullptr;
	uint8_t romBanks;
	uint8_t ramBanks;

	std::string title;

	DMG(std::string file);
	~DMG();

	void start();
	void printInfo();
};

