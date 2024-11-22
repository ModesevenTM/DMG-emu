#include "SM83.h"

SM83::SM83(Memory* memory) : memory(memory) {
	instructions.registers = &registers;
	instructions.memory = memory;
	instructions.interrupts = &interrupts;
	instructions.timer = &timer;

	interrupts.sm83 = this;
	timer.sm83 = this;
};