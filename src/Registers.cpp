#include "Registers.h"

Registers::Registers()
{
	a = 0x01;
	f = 0xB0;
	b = 0x00;
	c = 0x13;
	d = 0x00;
	e = 0xD8;
	h = 0x01;
	l = 0x4D;
}

void Registers::setFlag(uint8_t flag, bool value)
{
	if (value)
		f |= flag;
	else
		f &= ~flag;
}

bool Registers::getFlag(uint8_t flag)
{
	return f & flag;
}