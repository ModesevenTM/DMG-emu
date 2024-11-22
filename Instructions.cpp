#include "Instructions.h"

void Instructions::decode(uint8_t opcode)
{
	// Legend:
	// nx: x-bit immediate value
	// ax: x-bit address
	// ex: x-bit signed immediate value
	// HL+ - increments HL after load
	// HL- - decrements HL after load

	timer->countCycles(INSTR_CYCLES[opcode]);

	switch (opcode)
	{
	case 0x00: // NOP
		break;
	case 0x01: // LD BC, n16
	{
		registers->bc = memory->read16(registers->pc);
		registers->pc += 2;
		break;
	}
	case 0x02: // LD [BC], A
	{
		memory->write8(registers->bc, registers->a);
		break;
	}
	case 0x03: // INC BC
	{
		registers->bc++;
		break;
	}
	case 0x04: // INC B
	{
		INC(registers->b);
		break;
	}
	case 0x05: // DEC B
	{
		DEC(registers->b);
		break;
	}
	case 0x06: // LD B, n8
	{
		registers->b = memory->read8(registers->pc);
		registers->pc++;
		break;
	}
	case 0x07: // RLCA
	{
		RLC(registers->a);
		registers->setFlag(registers->AF_Z, false);
		break;
	}
	case 0x08: // LD [a16], SP
	{
		uint16_t addr = memory->read16(registers->pc);
		registers->pc += 2;
		memory->write16(addr, registers->sp);
		break;
	}
	case 0x09: // ADD HL, BC
	{
		ADD(registers->hl, registers->bc);
		break;
	}
	case 0x0A: // LD A, [BC]
	{
		registers->a = memory->read8(registers->bc);
		break;
	}
	case 0x0B: // DEC BC
	{
		registers->bc--;
		break;
	}
	case 0x0C: // INC C
	{
		INC(registers->c);
		break;
	}
	case 0x0D: // DEC C
	{
		DEC(registers->c);
		break;
	}
	case 0x0E: // LD C, n8
	{
		registers->c = memory->read8(registers->pc);
		registers->pc++;
		break;
	}
	case 0x0F: // RRCA
	{
		RRC(registers->a);
		registers->setFlag(registers->AF_Z, false);
		break;
	}
	case 0x10: // STOP
	{
		// TODO: Implement STOP
		// 2-byte instruction, second byte not always ignored
		break;
	}
	case 0x11: // LD DE, n16
	{
		registers->de = memory->read16(registers->pc);
		registers->pc += 2;
		break;
	}
	case 0x12: // LD [DE], A
	{
		memory->write8(registers->de, registers->a);
		break;
	}
	case 0x13: // INC DE
	{
		registers->de++;
		break;
	}
	case 0x14: // INC D
	{
		INC(registers->d);
		break;
	}
	case 0x15: // DEC D
	{
		DEC(registers->d);
		break;
	}
	case 0x16: // LD D, n8
	{
		registers->d = memory->read8(registers->pc);
		registers->pc++;
		break;
	}
	case 0x17: // RLA
	{
		RL(registers->a);
		registers->setFlag(registers->AF_Z, false);
		break;
	}
	case 0x18: // JR e8
	{
		/*int8_t offset = memory->read8(registers->pc);
		registers->pc += offset + 1;*/
		JR();
		break;
	}
	case 0x19: // ADD HL, DE
	{
		ADD(registers->hl, registers->de);
		break;
	}
	case 0x1A: // LD A, [DE]
	{
		registers->a = memory->read8(registers->de);
		break;
	}
	case 0x1B: // DEC DE
	{
		registers->de--;
		break;
	}
	case 0x1C: // INC E
	{
		INC(registers->e);
		break;
	}
	case 0x1D: // DEC E
	{
		DEC(registers->e);
		break;
	}
	case 0x1E: // LD E, n8
	{
		registers->e = memory->read8(registers->pc);
		registers->pc++;
		break;
	}
	case 0x1F: // RRA
	{
		RR(registers->a);
		registers->setFlag(registers->AF_Z, false);
		break;
	}
	case 0x20: // JR NZ, e8
	{
		/*int8_t offset = memory->read8(registers->pc);
		registers->pc++;
		if (!registers->getFlag(registers->AF_Z))
			registers->pc += offset;*/
		if(JR(!registers->getFlag(registers->AF_Z)))
			timer->countCycles(JR_COND_TRUE);
		break;
	}
	case 0x21: // LD HL, n16
	{
		registers->hl = memory->read16(registers->pc);
		registers->pc += 2;
		break;
	}
	case 0x22: // LD [HL+], A
	{
		memory->write8(registers->hl, registers->a);
		registers->hl++;
		break;
	}
	case 0x23: // INC HL
	{
		registers->hl++;
		break;
	}
	case 0x24: // INC H
	{
		INC(registers->h);
		break;
	}
	case 0x25: // DEC H
	{
		DEC(registers->h);
		break;
	}
	case 0x26: // LD H, n8
	{
		registers->h = memory->read8(registers->pc);
		registers->pc++;
		break;
	}
	case 0x27: // DAA
	{
		DAA();
		break;
	}
	case 0x28: // JR Z, e8
	{
		/*int8_t offset = memory->read8(registers->pc);
		registers->pc++;
		if (registers->getFlag(registers->AF_Z))
			registers->pc += offset;*/
		if(JR(registers->getFlag(registers->AF_Z)))
			timer->countCycles(JR_COND_TRUE);
		break;
	}
	case 0x29: // ADD HL, HL
	{
		ADD(registers->hl, registers->hl);
		break;
	}
	case 0x2A: // LD A, [HL+]
	{
		registers->a = memory->read8(registers->hl);
		registers->hl++;
		break;
	}
	case 0x2B: // DEC HL
	{
		registers->hl--;
		break;
	}
	case 0x2C: // INC L
	{
		INC(registers->l);
		break;
	}
	case 0x2D: // DEC L
	{
		DEC(registers->l);
		break;
	}
	case 0x2E: // LD L, n8
	{
		registers->l = memory->read8(registers->pc);
		registers->pc++;
		break;
	}
	case 0x2F: // CPL
	{
		registers->a = ~registers->a;
		registers->setFlag(registers->AF_N, true);
		registers->setFlag(registers->AF_H, true);
		break;
	}
	case 0x30: // JR NC, e8
	{
		/*int8_t offset = memory->read8(registers->pc);
		registers->pc++;
		if (!registers->getFlag(registers->AF_C))
			registers->pc += offset;*/
		if(JR(!registers->getFlag(registers->AF_C)))
			timer->countCycles(JR_COND_TRUE);
		break;
	}
	case 0x31: // LD SP, n16
	{
		registers->sp = memory->read16(registers->pc);
		registers->pc += 2;
		break;
	}
	case 0x32: // LD [HL-], A
	{
		memory->write8(registers->hl, registers->a);
		registers->hl--;
		break;
	}
	case 0x33: // INC SP
	{
		registers->sp++;
		break;
	}
	case 0x34: // INC [HL]
	{
		uint8_t val = memory->read8(registers->hl);
		INC(val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x35: // DEC [HL]
	{
		uint8_t val = memory->read8(registers->hl);
		DEC(val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x36: // LD [HL], n8
	{
		memory->write8(registers->hl, memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0x37: // SCF
	{
		registers->setFlag(registers->AF_N, false);
		registers->setFlag(registers->AF_H, false);
		registers->setFlag(registers->AF_C, true);
		break;
	}
	case 0x38: // JR C, e8
	{
		/*int8_t offset = memory->read8(registers->pc);
		registers->pc++;
		if (registers->getFlag(registers->AF_C))
			registers->pc += offset;*/
		if(JR(registers->getFlag(registers->AF_C)))
			timer->countCycles(JR_COND_TRUE);
		break;
	}
	case 0x39: // ADD HL, SP
	{
		ADD(registers->hl, registers->sp);
		break;
	}
	case 0x3A: // LD A, [HL-]
	{
		registers->a = memory->read8(registers->hl);
		registers->hl--;
		break;
	}
	case 0x3B: // DEC SP
	{
		registers->sp--;
		break;
	}
	case 0x3C: // INC A
	{
		INC(registers->a);
		break;
	}
	case 0x3D: // DEC A
	{
		DEC(registers->a);
		break;
	}
	case 0x3E: // LD A, n8
	{
		registers->a = memory->read8(registers->pc);
		registers->pc++;
		break;
	}
	case 0x3F: // CCF
	{
		registers->setFlag(registers->AF_N, false);
		registers->setFlag(registers->AF_H, false);
		registers->setFlag(registers->AF_C, !registers->getFlag(registers->AF_C));
		break;
	}
	case 0x40: // LD B, B
	{
		registers->b = registers->b;
		break;
	}
	case 0x41: // LD B, C
	{
		registers->b = registers->c;
		break;
	}
	case 0x42: // LD B, D
	{
		registers->b = registers->d;
		break;
	}
	case 0x43: // LD B, E
	{
		registers->b = registers->e;
		break;
	}
	case 0x44: // LD B, H
	{
		registers->b = registers->h;
		break;
	}
	case 0x45: // LD B, L
	{
		registers->b = registers->l;
		break;
	}
	case 0x46: // LD B, [HL]
	{
		registers->b = memory->read8(registers->hl);
		break;
	}
	case 0x47: // LD B, A
	{
		registers->b = registers->a;
		break;
	}
	case 0x48: // LD C, B
	{
		registers->c = registers->b;
		break;
	}
	case 0x49: // LD C, C
	{
		registers->c = registers->c;
		break;
	}
	case 0x4A: // LD C, D
	{
		registers->c = registers->d;
		break;
	}
	case 0x4B: // LD C, E
	{
		registers->c = registers->e;
		break;
	}
	case 0x4C: // LD C, H
	{
		registers->c = registers->h;
		break;
	}
	case 0x4D: // LD C, L
	{
		registers->c = registers->l;
		break;
	}
	case 0x4E: // LD C, [HL]
	{
		registers->c = memory->read8(registers->hl);
		break;
	}
	case 0x4F: // LD C, A
	{
		registers->c = registers->a;
		break;
	}
	case 0x50: // LD D, B
	{
		registers->d = registers->b;
		break;
	}
	case 0x51: // LD D, C
	{
		registers->d = registers->c;
		break;
	}
	case 0x52: // LD D, D
	{
		registers->d = registers->d;
		break;
	}
	case 0x53: // LD D, E
	{
		registers->d = registers->e;
		break;
	}
	case 0x54: // LD D, H
	{
		registers->d = registers->h;
		break;
	}
	case 0x55: // LD D, L
	{
		registers->d = registers->l;
		break;
	}
	case 0x56: // LD D, [HL]
	{
		registers->d = memory->read8(registers->hl);
		break;
	}
	case 0x57: // LD D, A
	{
		registers->d = registers->a;
		break;
	}
	case 0x58: // LD E, B
	{
		registers->e = registers->b;
		break;
	}
	case 0x59: // LD E, C
	{
		registers->e = registers->c;
		break;
	}
	case 0x5A: // LD E, D
	{
		registers->e = registers->d;
		break;
	}
	case 0x5B: // LD E, E
	{
		registers->e = registers->e;
		break;
	}
	case 0x5C: // LD E, H
	{
		registers->e = registers->h;
		break;
	}
	case 0x5D: // LD E, L
	{
		registers->e = registers->l;
		break;
	}
	case 0x5E: // LD E, [HL]
	{
		registers->e = memory->read8(registers->hl);
		break;
	}
	case 0x5F: // LD E, A
	{
		registers->e = registers->a;
		break;
	}
	case 0x60: // LD H, B
	{
		registers->h = registers->b;
		break;
	}
	case 0x61: // LD H, C
	{
		registers->h = registers->c;
		break;
	}
	case 0x62: // LD H, D
	{
		registers->h = registers->d;
		break;
	}
	case 0x63: // LD H, E
	{
		registers->h = registers->e;
		break;
	}
	case 0x64: // LD H, H
	{
		registers->h = registers->h;
		break;
	}
	case 0x65: // LD H, L
	{
		registers->h = registers->l;
		break;
	}
	case 0x66: // LD H, [HL]
	{
		registers->h = memory->read8(registers->hl);
		break;
	}
	case 0x67: // LD H, A
	{
		registers->h = registers->a;
		break;
	}
	case 0x68: // LD L, B
	{
		registers->l = registers->b;
		break;
	}
	case 0x69: // LD L, C
	{
		registers->l = registers->c;
		break;
	}
	case 0x6A: // LD L, D
	{
		registers->l = registers->d;
		break;
	}
	case 0x6B: // LD L, E
	{
		registers->l = registers->e;
		break;
	}
	case 0x6C: // LD L, H
	{
		registers->l = registers->h;
		break;
	}
	case 0x6D: // LD L, L
	{
		registers->l = registers->l;
		break;
	}
	case 0x6E: // LD L, [HL]
	{
		registers->l = memory->read8(registers->hl);
		break;
	}
	case 0x6F: // LD L, A
	{
		registers->l = registers->a;
		break;
	}
	case 0x70: // LD [HL], B
	{
		memory->write8(registers->hl, registers->b);
		break;
	}
	case 0x71: // LD [HL], C
	{
		memory->write8(registers->hl, registers->c);
		break;
	}
	case 0x72: // LD [HL], D
	{
		memory->write8(registers->hl, registers->d);
		break;
	}
	case 0x73: // LD [HL], E
	{
		memory->write8(registers->hl, registers->e);
		break;
	}
	case 0x74: // LD [HL], H
	{
		memory->write8(registers->hl, registers->h);
		break;
	}
	case 0x75: // LD [HL], L
	{
		memory->write8(registers->hl, registers->l);
		break;
	}
	case 0x76: // HALT
	{
		// TODO: Implement HALT
		// may replicate the "HALT bug" later
		HALT();
		break;
	}
	case 0x77: // LD [HL], A
	{
		memory->write8(registers->hl, registers->a);
		break;
	}
	case 0x78: // LD A, B
	{
		registers->a = registers->b;
		break;
	}
	case 0x79: // LD A, C
	{
		registers->a = registers->c;
		break;
	}
	case 0x7A: // LD A, D
	{
		registers->a = registers->d;
		break;
	}
	case 0x7B: // LD A, E
	{
		registers->a = registers->e;
		break;
	}
	case 0x7C: // LD A, H
	{
		registers->a = registers->h;
		break;
	}
	case 0x7D: // LD A, L
	{
		registers->a = registers->l;
		break;
	}
	case 0x7E: // LD A, [HL]
	{
		registers->a = memory->read8(registers->hl);
		break;
	}
	case 0x7F: // LD A, A
	{
		registers->a = registers->a;
		break;
	}
	case 0x80: // ADD A, B
	{
		ADD(registers->b);
		break;
	}
	case 0x81: // ADD A, C
	{
		ADD(registers->c);
		break;
	}
	case 0x82: // ADD A, D
	{
		ADD(registers->d);
		break;
	}
	case 0x83: // ADD A, E
	{
		ADD(registers->e);
		break;
	}
	case 0x84: // ADD A, H
	{
		ADD(registers->h);
		break;
	}
	case 0x85: // ADD A, L
	{
		ADD(registers->l);
		break;
	}
	case 0x86: // ADD A, [HL]
	{
		ADD(memory->read8(registers->hl));
		break;
	}
	case 0x87: // ADD A, A
	{
		ADD(registers->a);
		break;
	}
	case 0x88: // ADC A, B
	{
		ADC(registers->b);
		break;
	}
	case 0x89: // ADC A, C
	{
		ADC(registers->c);
		break;
	}
	case 0x8A: // ADC A, D
	{
		ADC(registers->d);
		break;
	}
	case 0x8B: // ADC A, E
	{
		ADC(registers->e);
		break;
	}
	case 0x8C: // ADC A, H
	{
		ADC(registers->h);
		break;
	}
	case 0x8D: // ADC A, L
	{
		ADC(registers->l);
		break;
	}
	case 0x8E: // ADC A, [HL]
	{
		ADC(memory->read8(registers->hl));
		break;
	}
	case 0x8F: // ADC A, A
	{
		ADC(registers->a);
		break;
	}
	case 0x90: // SUB A, B
	{
		SUB(registers->b);
		break;
	}
	case 0x91: // SUB A, C
	{
		SUB(registers->c);
		break;
	}
	case 0x92: // SUB A, D
	{
		SUB(registers->d);
		break;
	}
	case 0x93: // SUB A, E
	{
		SUB(registers->e);
		break;
	}
	case 0x94: // SUB A, H
	{
		SUB(registers->h);
		break;
	}
	case 0x95: // SUB A, L
	{
		SUB(registers->l);
		break;
	}
	case 0x96: // SUB A, [HL]
	{
		SUB(memory->read8(registers->hl));
		break;
	}
	case 0x97: // SUB A, A
	{
		SUB(registers->a);
		break;
	}
	case 0x98: // SBC A, B
	{
		SBC(registers->b);
		break;
	}
	case 0x99: // SBC A, C
	{
		SBC(registers->c);
		break;
	}
	case 0x9A: // SBC A, D
	{
		SBC(registers->d);
		break;
	}
	case 0x9B: // SBC A, E
	{
		SBC(registers->e);
		break;
	}
	case 0x9C: // SBC A, H
	{
		SBC(registers->h);
		break;
	}
	case 0x9D: // SBC A, L
	{
		SBC(registers->l);
		break;
	}
	case 0x9E: // SBC A, [HL]
	{
		SBC(memory->read8(registers->hl));
		break;
	}
	case 0x9F: // SBC A, A
	{
		SBC(registers->a);
		break;
	}
	case 0xA0: // AND A, B
	{
		AND(registers->b);
		break;
	}
	case 0xA1: // AND A, C
	{
		AND(registers->c);
		break;
	}
	case 0xA2: // AND A, D
	{
		AND(registers->d);
		break;
	}
	case 0xA3: // AND A, E
	{
		AND(registers->e);
		break;
	}
	case 0xA4: // AND A, H
	{
		AND(registers->h);
		break;
	}
	case 0xA5: // AND A, L
	{
		AND(registers->l);
		break;
	}
	case 0xA6: // AND A, [HL]
	{
		AND(memory->read8(registers->hl));
		break;
	}
	case 0xA7: // AND A, A
	{
		AND(registers->a);
		break;
	}
	case 0xA8: // XOR A, B
	{
		XOR(registers->b);
		break;
	}
	case 0xA9: // XOR A, C
	{
		XOR(registers->c);
		break;
	}
	case 0xAA: // XOR A, D
	{
		XOR(registers->d);
		break;
	}
	case 0xAB: // XOR A, E
	{
		XOR(registers->e);
		break;
	}
	case 0xAC: // XOR A, H
	{
		XOR(registers->h);
		break;
	}
	case 0xAD: // XOR A, L
	{
		XOR(registers->l);
		break;
	}
	case 0xAE: // XOR A, [HL]
	{
		XOR(memory->read8(registers->hl));
		break;
	}
	case 0xAF: // XOR A, A
	{
		XOR(registers->a);
		break;
	}
	case 0xB0: // OR A, B
	{
		OR(registers->b);
		break;
	}
	case 0xB1: // OR A, C
	{
		OR(registers->c);
		break;
	}
	case 0xB2: // OR A, D
	{
		OR(registers->d);
		break;
	}
	case 0xB3: // OR A, E
	{
		OR(registers->e);
		break;
	}
	case 0xB4: // OR A, H
	{
		OR(registers->h);
		break;
	}
	case 0xB5: // OR A, L
	{
		OR(registers->l);
		break;
	}
	case 0xB6: // OR A, [HL]
	{
		OR(memory->read8(registers->hl));
		break;
	}
	case 0xB7: // OR A, A
	{
		OR(registers->a);
		break;
	}
	case 0xB8: // CP A, B
	{
		CP(registers->b);
		break;
	}
	case 0xB9: // CP A, C
	{
		CP(registers->c);
		break;
	}
	case 0xBA: // CP A, D
	{
		CP(registers->d);
		break;
	}
	case 0xBB: // CP A, E
	{
		CP(registers->e);
		break;
	}
	case 0xBC: // CP A, H
	{
		CP(registers->h);
		break;
	}
	case 0xBD: // CP A, L
	{
		CP(registers->l);
		break;
	}
	case 0xBE: // CP A, [HL]
	{
		CP(memory->read8(registers->hl));
		break;
	}
	case 0xBF: // CP A, A
	{
		CP(registers->a);
		break;
	}
	case 0xC0: // RET NZ
	{
		if (RET(!registers->getFlag(registers->AF_Z)))
			timer->countCycles(RET_COND_TRUE);
		break;
	}
	case 0xC1: // POP BC
	{
		POP(registers->bc);
		break;
	}
	case 0xC2: // JP NZ, a16
	{
		if (JP(!registers->getFlag(registers->AF_Z)))
			timer->countCycles(JP_COND_TRUE);
		break;
	}
	case 0xC3: // JP a16
	{
		JP();
		break;
	}
	case 0xC4: // CALL NZ, a16
	{
		if (CALL(!registers->getFlag(registers->AF_Z)))
			timer->countCycles(CALL_COND_TRUE);
		break;
	}
	case 0xC5: // PUSH BC
	{
		PUSH(registers->bc);
		break;
	}
	case 0xC6: // ADD A, n8
	{
		ADD(memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xC7: // RST $00
	{
		RST(0x00);
		break;
	}
	case 0xC8: // RET Z
	{
		if (RET(registers->getFlag(registers->AF_Z)))
			timer->countCycles(RET_COND_TRUE);
		break;
	}
	case 0xC9: // RET
	{
		RET();
		break;
	}
	case 0xCA: // JP Z, a16
	{
		if (JP(registers->getFlag(registers->AF_Z)))
			timer->countCycles(JP_COND_TRUE);
		break;
	}
	case 0xCB: // $CB prefix
	{
		decodeCB(memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xCC: // CALL Z, a16
	{
		if (CALL(registers->getFlag(registers->AF_Z)))
			timer->countCycles(CALL_COND_TRUE);
		break;
	}
	case 0xCD: // CALL a16
	{
		CALL();
		break;
	}
	case 0xCE: // ADC A, n8
	{
		ADC(memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xCF: // RST $08
	{
		RST(0x08);
		break;
	}
	case 0xD0: // RET NC
	{
		if (RET(!registers->getFlag(registers->AF_C)))
			timer->countCycles(RET_COND_TRUE);
		break;
	}
	case 0xD1: // POP DE
	{
		POP(registers->de);
		break;
	}
	case 0xD2: // JP NC, a16
	{
		if (JP(!registers->getFlag(registers->AF_C)))
			timer->countCycles(JP_COND_TRUE);
		break;
	}
	case 0xD4: // CALL NC, a16
	{
		if (CALL(!registers->getFlag(registers->AF_C)))
			timer->countCycles(CALL_COND_TRUE);
		break;
	}
	case 0xD5: // PUSH DE
	{
		PUSH(registers->de);
		break;
	}
	case 0xD6: // SUB A, n8
	{
		SUB(memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xD7: // RST $10
	{
		RST(0x10);
		break;
	}
	case 0xD8: // RET C
	{
		if(RET(registers->getFlag(registers->AF_C)))
			timer->countCycles(RET_COND_TRUE);
		break;
	}
	case 0xD9: // RETI
	{
		interrupts->EI();
		RET();
		break;
	}
	case 0xDA: // JP C, a16
	{
		if (JP(registers->getFlag(registers->AF_C)))
			timer->countCycles(JP_COND_TRUE);
		break;
	}
	case 0xDC: // CALL C, a16
	{
		if (CALL(registers->getFlag(registers->AF_C)))
			timer->countCycles(CALL_COND_TRUE);
		break;
	}
	case 0xDE: // SBC A, n8
	{
		SBC(memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xDF: // RST $18
	{
		RST(0x18);
		break;
	}
	case 0xE0: // LDH [a8], A
	{
		memory->write8(0xFF00 + memory->read8(registers->pc), registers->a);
		registers->pc++;
		break;
	}
	case 0xE1: // POP HL
	{
		POP(registers->hl);
		break;
	}
	case 0xE2: // LD [C], A
	{
		memory->write8(0xFF00 + registers->c, registers->a);
		break;
	}
	case 0xE5: // PUSH HL
	{
		PUSH(registers->hl);
		break;
	}
	case 0xE6: // AND A, n8
	{
		AND(memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xE7: // RST $20
	{
		RST(0x20);
		break;
	}
	case 0xE8: // ADD SP, e8
	{
		ADDSP((int8_t)memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xE9: // JP HL
	{
		registers->pc = registers->hl;
		break;
	}
	case 0xEA: // LD [a16], A
	{
		memory->write8(memory->read16(registers->pc), registers->a);
		registers->pc += 2;
		break;
	}
	case 0xEE: // XOR A, n8
	{
		XOR(memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xEF: // RST $28
	{
		RST(0x28);
		break;
	}
	case 0xF0: // LDH A, [a8]
	{
		registers->a = memory->read8(0xFF00 + memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xF1: // POP AF
	{
		POP(registers->af);
		registers->f &= 0xF0; // lower 4 bits of F are always 0
		break;
	}
	case 0xF2: // LD A, [C]
	{
		registers->a = memory->read8(0xFF00 + registers->c);
		break;
	}
	case 0xF3: // DI
	{
		interrupts->DI();
		break;
	}
	case 0xF5: // PUSH AF
	{
		PUSH(registers->af);
		break;
	}
	case 0xF6: // OR A, n8
	{
		OR(memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xF7: // RST $30
	{
		RST(0x30);
		break;
	}
	case 0xF8: // LD HL, SP+e8
	{
		LDHL((int8_t)memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xF9: // LD SP, HL
	{
		registers->sp = registers->hl;
		break;
	}
	case 0xFA: // LD A, [a16]
	{
		registers->a = memory->read8(memory->read16(registers->pc));
		registers->pc += 2;
		break;
	}
	case 0xFB: // EI
	{
		interrupts->EI();
		break;
	}
	case 0xFE: // CP A, n8
	{
		CP(memory->read8(registers->pc));
		registers->pc++;
		break;
	}
	case 0xFF: // RST $38
	{
		RST(0x38);
		break;
	}
	}
}