#include "Instructions.h"

void Instructions::decodeCB(uint8_t opcode)
{
	timer->countCycles(2 + (((opcode & 7) == 6) << ((opcode >> 6) != 1)));

	switch (opcode)
	{
	case 0x00: RLC(registers->b); break;
	case 0x01: RLC(registers->c); break;
	case 0x02: RLC(registers->d); break;
	case 0x03: RLC(registers->e); break;
	case 0x04: RLC(registers->h); break;
	case 0x05: RLC(registers->l); break;
	case 0x06:
	{
		uint8_t val = memory->read8(registers->hl);
		RLC(val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x07: RLC(registers->a); break;
	case 0x08: RRC(registers->b); break;
	case 0x09: RRC(registers->c); break;
	case 0x0A: RRC(registers->d); break;
	case 0x0B: RRC(registers->e); break;
	case 0x0C: RRC(registers->h); break;
	case 0x0D: RRC(registers->l); break;
	case 0x0E:
	{
		uint8_t val = memory->read8(registers->hl);
		RRC(val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x0F: RRC(registers->a); break;
	case 0x10: RL(registers->b); break;
	case 0x11: RL(registers->c); break;
	case 0x12: RL(registers->d); break;
	case 0x13: RL(registers->e); break;
	case 0x14: RL(registers->h); break;
	case 0x15: RL(registers->l); break;
	case 0x16:
	{
		uint8_t val = memory->read8(registers->hl);
		RL(val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x17: RL(registers->a); break;
	case 0x18: RR(registers->b); break;
	case 0x19: RR(registers->c); break;
	case 0x1A: RR(registers->d); break;
	case 0x1B: RR(registers->e); break;
	case 0x1C: RR(registers->h); break;
	case 0x1D: RR(registers->l); break;
	case 0x1E:
	{
		uint8_t val = memory->read8(registers->hl);
		RR(val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x1F: RR(registers->a); break;
	case 0x20: SLA(registers->b); break;
	case 0x21: SLA(registers->c); break;
	case 0x22: SLA(registers->d); break;
	case 0x23: SLA(registers->e); break;
	case 0x24: SLA(registers->h); break;
	case 0x25: SLA(registers->l); break;
	case 0x26:
	{
		uint8_t val = memory->read8(registers->hl);
		SLA(val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x27: SLA(registers->a); break;
	case 0x28: SRA(registers->b); break;
	case 0x29: SRA(registers->c); break;
	case 0x2A: SRA(registers->d); break;
	case 0x2B: SRA(registers->e); break;
	case 0x2C: SRA(registers->h); break;
	case 0x2D: SRA(registers->l); break;
	case 0x2E:
	{
		uint8_t val = memory->read8(registers->hl);
		SRA(val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x2F: SRA(registers->a); break;
	case 0x30: SWAP(registers->b); break;
	case 0x31: SWAP(registers->c); break;
	case 0x32: SWAP(registers->d); break;
	case 0x33: SWAP(registers->e); break;
	case 0x34: SWAP(registers->h); break;
	case 0x35: SWAP(registers->l); break;
	case 0x36:
	{
		uint8_t val = memory->read8(registers->hl);
		SWAP(val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x37: SWAP(registers->a); break;
	case 0x38: SRL(registers->b); break;
	case 0x39: SRL(registers->c); break;
	case 0x3A: SRL(registers->d); break;
	case 0x3B: SRL(registers->e); break;
	case 0x3C: SRL(registers->h); break;
	case 0x3D: SRL(registers->l); break;
	case 0x3E:
	{
		uint8_t val = memory->read8(registers->hl);
		SRL(val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x3F: SRL(registers->a); break;
	case 0x40: BIT(0, registers->b); break;
	case 0x41: BIT(0, registers->c); break;
	case 0x42: BIT(0, registers->d); break;
	case 0x43: BIT(0, registers->e); break;
	case 0x44: BIT(0, registers->h); break;
	case 0x45: BIT(0, registers->l); break;
	case 0x46: BIT(0, memory->read8(registers->hl)); break;
	case 0x47: BIT(0, registers->a); break;
	case 0x48: BIT(1, registers->b); break;
	case 0x49: BIT(1, registers->c); break;
	case 0x4A: BIT(1, registers->d); break;
	case 0x4B: BIT(1, registers->e); break;
	case 0x4C: BIT(1, registers->h); break;
	case 0x4D: BIT(1, registers->l); break;
	case 0x4E: BIT(1, memory->read8(registers->hl)); break;
	case 0x4F: BIT(1, registers->a); break;
	case 0x50: BIT(2, registers->b); break;
	case 0x51: BIT(2, registers->c); break;
	case 0x52: BIT(2, registers->d); break;
	case 0x53: BIT(2, registers->e); break;
	case 0x54: BIT(2, registers->h); break;
	case 0x55: BIT(2, registers->l); break;
	case 0x56: BIT(2, memory->read8(registers->hl)); break;
	case 0x57: BIT(2, registers->a); break;
	case 0x58: BIT(3, registers->b); break;
	case 0x59: BIT(3, registers->c); break;
	case 0x5A: BIT(3, registers->d); break;
	case 0x5B: BIT(3, registers->e); break;
	case 0x5C: BIT(3, registers->h); break;
	case 0x5D: BIT(3, registers->l); break;
	case 0x5E: BIT(3, memory->read8(registers->hl)); break;
	case 0x5F: BIT(3, registers->a); break;
	case 0x60: BIT(4, registers->b); break;
	case 0x61: BIT(4, registers->c); break;
	case 0x62: BIT(4, registers->d); break;
	case 0x63: BIT(4, registers->e); break;
	case 0x64: BIT(4, registers->h); break;
	case 0x65: BIT(4, registers->l); break;
	case 0x66: BIT(4, memory->read8(registers->hl)); break;
	case 0x67: BIT(4, registers->a); break;
	case 0x68: BIT(5, registers->b); break;
	case 0x69: BIT(5, registers->c); break;
	case 0x6A: BIT(5, registers->d); break;
	case 0x6B: BIT(5, registers->e); break;
	case 0x6C: BIT(5, registers->h); break;
	case 0x6D: BIT(5, registers->l); break;
	case 0x6E: BIT(5, memory->read8(registers->hl)); break;
	case 0x6F: BIT(5, registers->a); break;
	case 0x70: BIT(6, registers->b); break;
	case 0x71: BIT(6, registers->c); break;
	case 0x72: BIT(6, registers->d); break;
	case 0x73: BIT(6, registers->e); break;
	case 0x74: BIT(6, registers->h); break;
	case 0x75: BIT(6, registers->l); break;
	case 0x76: BIT(6, memory->read8(registers->hl)); break;
	case 0x77: BIT(6, registers->a); break;
	case 0x78: BIT(7, registers->b); break;
	case 0x79: BIT(7, registers->c); break;
	case 0x7A: BIT(7, registers->d); break;
	case 0x7B: BIT(7, registers->e); break;
	case 0x7C: BIT(7, registers->h); break;
	case 0x7D: BIT(7, registers->l); break;
	case 0x7E: BIT(7, memory->read8(registers->hl)); break;
	case 0x7F: BIT(7, registers->a); break;
	case 0x80: RES(0, registers->b); break;
	case 0x81: RES(0, registers->c); break;
	case 0x82: RES(0, registers->d); break;
	case 0x83: RES(0, registers->e); break;
	case 0x84: RES(0, registers->h); break;
	case 0x85: RES(0, registers->l); break;
	case 0x86:
	{
		uint8_t val = memory->read8(registers->hl);
		RES(0, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x87: RES(0, registers->a); break;
	case 0x88: RES(1, registers->b); break;
	case 0x89: RES(1, registers->c); break;
	case 0x8A: RES(1, registers->d); break;
	case 0x8B: RES(1, registers->e); break;
	case 0x8C: RES(1, registers->h); break;
	case 0x8D: RES(1, registers->l); break;
	case 0x8E:
	{
		uint8_t val = memory->read8(registers->hl);
		RES(1, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x8F: RES(1, registers->a); break;
	case 0x90: RES(2, registers->b); break;
	case 0x91: RES(2, registers->c); break;
	case 0x92: RES(2, registers->d); break;
	case 0x93: RES(2, registers->e); break;
	case 0x94: RES(2, registers->h); break;
	case 0x95: RES(2, registers->l); break;
	case 0x96:
	{
		uint8_t val = memory->read8(registers->hl);
		RES(2, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x97: RES(2, registers->a); break;
	case 0x98: RES(3, registers->b); break;
	case 0x99: RES(3, registers->c); break;
	case 0x9A: RES(3, registers->d); break;
	case 0x9B: RES(3, registers->e); break;
	case 0x9C: RES(3, registers->h); break;
	case 0x9D: RES(3, registers->l); break;
	case 0x9E:
	{
		uint8_t val = memory->read8(registers->hl);
		RES(3, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0x9F: RES(3, registers->a); break;
	case 0xA0: RES(4, registers->b); break;
	case 0xA1: RES(4, registers->c); break;
	case 0xA2: RES(4, registers->d); break;
	case 0xA3: RES(4, registers->e); break;
	case 0xA4: RES(4, registers->h); break;
	case 0xA5: RES(4, registers->l); break;
	case 0xA6:
	{
		uint8_t val = memory->read8(registers->hl);
		RES(4, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xA7: RES(4, registers->a); break;
	case 0xA8: RES(5, registers->b); break;
	case 0xA9: RES(5, registers->c); break;
	case 0xAA: RES(5, registers->d); break;
	case 0xAB: RES(5, registers->e); break;
	case 0xAC: RES(5, registers->h); break;
	case 0xAD: RES(5, registers->l); break;
	case 0xAE:
	{
		uint8_t val = memory->read8(registers->hl);
		RES(5, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xAF: RES(5, registers->a); break;
	case 0xB0: RES(6, registers->b); break;
	case 0xB1: RES(6, registers->c); break;
	case 0xB2: RES(6, registers->d); break;
	case 0xB3: RES(6, registers->e); break;
	case 0xB4: RES(6, registers->h); break;
	case 0xB5: RES(6, registers->l); break;
	case 0xB6:
	{
		uint8_t val = memory->read8(registers->hl);
		RES(6, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xB7: RES(6, registers->a); break;
	case 0xB8: RES(7, registers->b); break;
	case 0xB9: RES(7, registers->c); break;
	case 0xBA: RES(7, registers->d); break;
	case 0xBB: RES(7, registers->e); break;
	case 0xBC: RES(7, registers->h); break;
	case 0xBD: RES(7, registers->l); break;
	case 0xBE:
	{
		uint8_t val = memory->read8(registers->hl);
		RES(7, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xBF: RES(7, registers->a); break;
	case 0xC0: SET(0, registers->b); break;
	case 0xC1: SET(0, registers->c); break;
	case 0xC2: SET(0, registers->d); break;
	case 0xC3: SET(0, registers->e); break;
	case 0xC4: SET(0, registers->h); break;
	case 0xC5: SET(0, registers->l); break;
	case 0xC6:
	{
		uint8_t val = memory->read8(registers->hl);
		SET(0, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xC7: SET(0, registers->a); break;
	case 0xC8: SET(1, registers->b); break;
	case 0xC9: SET(1, registers->c); break;
	case 0xCA: SET(1, registers->d); break;
	case 0xCB: SET(1, registers->e); break;
	case 0xCC: SET(1, registers->h); break;
	case 0xCD: SET(1, registers->l); break;
	case 0xCE:
	{
		uint8_t val = memory->read8(registers->hl);
		SET(1, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xCF: SET(1, registers->a); break;
	case 0xD0: SET(2, registers->b); break;
	case 0xD1: SET(2, registers->c); break;
	case 0xD2: SET(2, registers->d); break;
	case 0xD3: SET(2, registers->e); break;
	case 0xD4: SET(2, registers->h); break;
	case 0xD5: SET(2, registers->l); break;
	case 0xD6:
	{
		uint8_t val = memory->read8(registers->hl);
		SET(2, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xD7: SET(2, registers->a); break;
	case 0xD8: SET(3, registers->b); break;
	case 0xD9: SET(3, registers->c); break;
	case 0xDA: SET(3, registers->d); break;
	case 0xDB: SET(3, registers->e); break;
	case 0xDC: SET(3, registers->h); break;
	case 0xDD: SET(3, registers->l); break;
	case 0xDE:
	{
		uint8_t val = memory->read8(registers->hl);
		SET(3, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xDF: SET(3, registers->a); break;
	case 0xE0: SET(4, registers->b); break;
	case 0xE1: SET(4, registers->c); break;
	case 0xE2: SET(4, registers->d); break;
	case 0xE3: SET(4, registers->e); break;
	case 0xE4: SET(4, registers->h); break;
	case 0xE5: SET(4, registers->l); break;
	case 0xE6:
	{
		uint8_t val = memory->read8(registers->hl);
		SET(4, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xE7: SET(4, registers->a); break;
	case 0xE8: SET(5, registers->b); break;
	case 0xE9: SET(5, registers->c); break;
	case 0xEA: SET(5, registers->d); break;
	case 0xEB: SET(5, registers->e); break;
	case 0xEC: SET(5, registers->h); break;
	case 0xED: SET(5, registers->l); break;
	case 0xEE:
	{
		uint8_t val = memory->read8(registers->hl);
		SET(5, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xEF: SET(5, registers->a); break;
	case 0xF0: SET(6, registers->b); break;
	case 0xF1: SET(6, registers->c); break;
	case 0xF2: SET(6, registers->d); break;
	case 0xF3: SET(6, registers->e); break;
	case 0xF4: SET(6, registers->h); break;
	case 0xF5: SET(6, registers->l); break;
	case 0xF6:
	{
		uint8_t val = memory->read8(registers->hl);
		SET(6, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xF7: SET(6, registers->a); break;
	case 0xF8: SET(7, registers->b); break;
	case 0xF9: SET(7, registers->c); break;
	case 0xFA: SET(7, registers->d); break;
	case 0xFB: SET(7, registers->e); break;
	case 0xFC: SET(7, registers->h); break;
	case 0xFD: SET(7, registers->l); break;
	case 0xFE:
	{
		uint8_t val = memory->read8(registers->hl);
		SET(7, val);
		memory->write8(registers->hl, val);
		break;
	}
	case 0xFF: SET(7, registers->a); break;
	}
}