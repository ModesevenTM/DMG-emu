#include "SM83.h"

SM83::SM83(Memory* memory) : memory(memory) {
	instructions.registers = &registers;
	instructions.memory = memory;
	instructions.interrupts = &interrupts;
	instructions.timer = &timer;

	interrupts.sm83 = this;
	timer.sm83 = this;
	ppu.sm83 = this;
	memory->sm83 = this;
	joypad.sm83 = this;
};

void SM83::step() {
	if (halted) timer.countCycles(1);
	else {
		uint8_t opcode = memory->read8(registers.pc);
		registers.pc++;
		instructions.decode(opcode);
	}
}