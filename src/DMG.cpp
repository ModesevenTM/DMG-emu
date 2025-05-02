#include "DMG.h"

DMG::DMG(std::string file)
{
	uint8_t* rom;
	std::ifstream romFile(file, std::ios::binary);
	if (romFile.is_open())
	{
		romFile.seekg(0, std::ios::end);
		int size = romFile.tellg();
		romFile.seekg(0, std::ios::beg);
		rom = new uint8_t[size];
		romFile.read((char*)rom, size);
		romFile.close();
	}
	else
	{
		throw std::runtime_error("Could not open file");
	}

	title = std::string((char*)&rom[0x134], 16);

	// ROM banks
	if(rom[0x148] <= 8)
		romBanks = 2 << rom[0x148];
	else
	{
		switch (rom[0x148])  // unofficial values
		{
		case 0x52:
			romBanks = 72;
			break;
		case 0x53:
			romBanks = 80;
			break;
		case 0x54:
			romBanks = 96;
			break;
		}
	}

	// RAM banks
	switch (rom[0x149])
	{
	case 0:
	case 1: // unofficially 2 KiB, though never used
		ramBanks = 0;
		break;
	case 2:
		ramBanks = 1;
		break;
	case 3:
		ramBanks = 4;
		break;
	case 4:
		ramBanks = 16;
		break;
	case 5:
		ramBanks = 8;
		break;
	}

	// load savefile if the game supports it
	if (ramBanks)
	{
		exram = new uint8_t[ramBanks * 0x2000];

		std::ifstream saveFile(title, std::ios::binary);
		if (saveFile.is_open())
		{
			saveFile.read((char*)exram, (unsigned long)ramBanks * 0x2000);
			saveFile.close();
		}
	}

	// MBC type
	switch (rom[0x147])
	{
	case 0x00:	// MBC0
	case 0x08:	// MBC0 + RAM 
	case 0x09:	// MBC0 + RAM + BATTERY
		memory = new MBC0(rom);
		break;
	case 0x01:	// MBC1
	case 0x02:	// MBC1 + RAM
	case 0x03:	// MBC1 + RAM + BATTERY
		memory = new MBC1(rom, romBanks, ramBanks, exram);
		break;
	default:
		throw std::runtime_error("Unsupported MBC type");
		break;
	}	

	sm83 = new SM83(memory);
	renderer = new Renderer(&sm83->ppu);

	printInfo();
}

DMG::~DMG()
{
	delete renderer;
	delete sm83;
	if(memory) delete memory;
}

void DMG::start()
{
	while (!sm83->ended)
	{
		if(!sm83->interrupts.checkInterrupts())
			sm83->step();
		sm83->timer.update();
		sm83->ppu.step();
		if (sm83->ppu.frameReady)
		{
			sm83->ppu.frameReady = false;
			renderer->render();
		}
		sm83->joypad.step();
	}

	saveGame();
}

void DMG::printInfo()
{
	std::cout << "Title: " << title << std::endl;
	std::cout << "ROM banks: " << (int)romBanks << std::endl;
	std::cout << "RAM banks: " << (int)ramBanks << std::endl;
	std::cout << "MBC type: " << (int)memory->rom[0x147] << std::endl;
}

void DMG::saveGame()
{
	if (ramBanks && exram)
	{
		std::ofstream saveFile(title, std::ios::binary);
		if (saveFile.is_open())
		{
			saveFile.write((char*)exram, (unsigned long)ramBanks * 0x2000);
			saveFile.close();
		}
		else
		{
			std::cerr << "Could not save game" << std::endl;
		}
	}
}