#include "Memory.h"
#include "SM83.h"

Memory::Memory(uint8_t* rom)
{
	this->rom = rom;
}

Memory::~Memory()
{
	delete[] rom;
}

uint8_t Memory::read8(uint16_t add)
{
	if (add < 0x8000)
		return rom[add];			// ROM Bank 0-1
	else if(add < 0xA000)
		return vram[add - 0x8000];	// VRAM
	// else if (add < 0xC000) -> controlled by MBC
	else if (add >= 0xC000 && add < 0xE000)
		return wram[add - 0xC000];	// RAM
	else if (add < 0xFE00)
		return wram[add - 0xE000];	// Echo RAM, shouldn't be used
	else if (add < 0xFEA0)
		return oam[add - 0xFE00];	// Object Attribute Memory
	else if (add < 0xFF00)
		return 0xFF;				// not usable
	else if (add < 0xFF80)
	{
		switch (add)
		{
		case 0xFF00: return sm83->joypad.P1;	// Joypad
		case 0xFF01: return 0xFF;	// Serial Transfer Data - not implemented
		case 0xFF02: return 0xFF;	// Serial Transfer Control - not implemented
		case 0xFF04: return sm83->timer.DIV;	// Divider Register
		case 0xFF05: return sm83->timer.TIMA;	// Timer Counter
		case 0xFF06: return sm83->timer.TMA;	// Timer Modulo
		case 0xFF07: return sm83->timer.TAC;	// Timer Control
		case 0xFF0F: return sm83->interrupts.IF;	// Interrupt Flag
		//case 0xFF10 ... 0xFF26: return 0xFF;	// Sound - not implemented
		case 0xFF40: return sm83->ppu.LCDC;	// LCDC
		case 0xFF41: return sm83->ppu.STAT;	// STAT
		case 0xFF42: return sm83->ppu.SCY;	// SCY
		case 0xFF43: return sm83->ppu.SCX;	// SCX
		case 0xFF44: return sm83->ppu.LY;	// LY
		case 0xFF45: return sm83->ppu.LYC;	// LYC
		case 0xFF46: return OAMDMA;	// DMA
		case 0xFF47: return sm83->ppu.BGP;	// BGP
		case 0xFF48: return sm83->ppu.OBP0;	// OBP0
		case 0xFF49: return sm83->ppu.OBP1;	// OBP1
		case 0xFF4A: return sm83->ppu.WY;	// WY
		case 0xFF4B: return sm83->ppu.WX;	// WX
		case 0xFF50: return BOOT;	// Boot ROM
		default: return 0xFF;
		}
	}
	else if (add < 0xFFFF)
		return hram[add - 0xFF80];	// High RAM
	else if (add == 0xFFFF)
		return sm83->interrupts.IE;	// Interrupt Enable Register
	return 0xFF;
}

void Memory::write8(uint16_t add, uint8_t val)
{
	if (add < 0x8000) // ROM, not writable
		return;
	else if (add < 0xA000)
		vram[add - 0x8000] = val;	// VRAM
	else if (add < 0xC000) // External RAM, managed by MBC if present
		return;
	else if (add < 0xE000)
		wram[add - 0xC000] = val;	// RAM
	else if (add < 0xFE00)
		wram[add - 0xE000] = val;	// Echo RAM, shouldn't be used
	else if (add < 0xFEA0)
		oam[add - 0xFE00] = val;	// Object Attribute Memory
	else if (add < 0xFF00)
		return;						// not usable
	else if (add < 0xFF80)
	{
		switch (add)
		{
		case 0xFF00: sm83->joypad.P1 = val & 0xF0; break;	// Joypad
		//case 0xFF01: break;	// Serial Transfer Data - not implemented
		//case 0xFF02: break;	// Serial Transfer Control - not implemented
		case 0xFF04: sm83->timer.DIV = 0; break;	// Divider Register
		case 0xFF05: sm83->timer.TIMA = val; break;	// Timer Counter - TODO: shouldn't be written like this
		case 0xFF06: sm83->timer.TMA = val;	break; // Timer Modulo - TODO: same as above
		case 0xFF07: sm83->timer.TAC = val & 0x07; break;	// Timer Control
		case 0xFF0F: sm83->interrupts.IF = val & 0x1F; break;	// Interrupt Flag
		//case 0xFF10 ... 0xFF26: break;	// Sound - not implemented
		case 0xFF40: {		// LCDC
			sm83->ppu.LCDC = val;
			if (!(val & 0x80))	// LCD off
			{
				sm83->ppu.LY = 0;
				sm83->ppu.STAT &= 0xFC;
			}
			break;
		} 
		case 0xFF41: sm83->ppu.STAT = val & 0x7F; break;	// STAT
		case 0xFF42: sm83->ppu.SCY = val; break;	// SCY
		case 0xFF43: sm83->ppu.SCX = val; break;	// SCX
		//case 0xFF44: break;	// LY - read only
		case 0xFF45: sm83->ppu.LYC = val; break;	// LYC
		case 0xFF46:		// DMA
		{
			OAMDMA = val;
			oamDMA(val);
			break;
		};
		case 0xFF47: sm83->ppu.BGP = val; break;	// BGP
		case 0xFF48: sm83->ppu.OBP0 = val; break;	// OBP0
		case 0xFF49: sm83->ppu.OBP1 = val; break;	// OBP1
		case 0xFF4A: sm83->ppu.WY = val; break;	// WY
		case 0xFF4B: sm83->ppu.WX = val; break;	// WX
		case 0xFF50: BOOT = 0x01;	// Boot ROM
		default: break;
		}
	}
	else if (add >= 0xFF80 && add < 0xFFFF)
		hram[add - 0xFF80] = val;	// High RAM
	else
		sm83->interrupts.IE = val & 0x1F;	// Interrupt Enable

}

uint16_t Memory::read16(uint16_t add)
{
    return read8(add + 1) << 8 | read8(add);
}

void Memory::write16(uint16_t add, uint16_t val)
{
	write8(add, val & 0xFF);
	write8(add + 1, val >> 8);
}

void Memory::oamDMA(uint8_t src)
{
	for (int i = 0; i < 0xA0; i++)
		oam[i] = read8((src << 8) + i);
}