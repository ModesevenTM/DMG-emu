#include "Interrupts.h"
#include "SM83.h"

void Interrupts::EI()
{
	//EIqueued = true;
	IME = true;
}

void Interrupts::DI()
{
	EIqueued = false;
	IME = false;
}

void Interrupts::requestInterrupt(uint8_t interrupt)
{
	IF |= interrupt;
}

void Interrupts::serviceInterrupt(uint8_t interrupt)
{
	auto it = INT_ADDR.find(interrupt);
	if (it != INT_ADDR.end())
	{
		sm83->instructions.RST(it->second);
		IME = false;
		IF &= ~interrupt;
		sm83->timer.countCycles(5);
	}
	else
	{
		throw std::runtime_error("Interrupt not found");
	}
}

bool Interrupts::checkInterrupts()
{
	if(sm83->halted && IE & IF)
		sm83->halted = false;

	if (EIqueued)
	{
		EIqueued = false;
		IME = true;
		return false;
	}
	if (IME)
	{
		uint8_t interrupt = IE & IF;
		if (interrupt)
		{
			if (interrupt & MASK_INT_VBLANK)
				serviceInterrupt(MASK_INT_VBLANK);
			else if (interrupt & MASK_INT_STAT)
				serviceInterrupt(MASK_INT_STAT);
			else if (interrupt & MASK_INT_TIMER)
				serviceInterrupt(MASK_INT_TIMER);
			else if (interrupt & MASK_INT_SERIAL)
				serviceInterrupt(MASK_INT_SERIAL);
			else if (interrupt & MASK_INT_JOYPAD)
				serviceInterrupt(MASK_INT_JOYPAD);
			return true;
		}
	}
	return false;
}