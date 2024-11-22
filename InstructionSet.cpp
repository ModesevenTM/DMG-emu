#include "Instructions.h"
#include "SM83.h"

void Instructions::INC(uint8_t& reg)
{
	reg++;
	registers->setFlag(registers->AF_Z, reg == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, (reg & 0x0F) == 0);
}

void Instructions::DEC(uint8_t& reg)
{
	reg--;
	registers->setFlag(registers->AF_Z, reg == 0);
	registers->setFlag(registers->AF_N, true);
	registers->setFlag(registers->AF_H, (reg & 0x0F) == 0x0F);
}

void Instructions::RLC(uint8_t& reg)
{
	registers->setFlag(registers->AF_C, (reg & 0x80) != 0);
	reg = (reg << 1) | (reg >> 7);
	registers->setFlag(registers->AF_Z, reg == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, false);
}

void Instructions::ADD(uint16_t& reg, uint16_t val)
{
	uint32_t result = reg + val;
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, (reg & 0x0FFF) + (val & 0x0FFF) > 0x0FFF);
	registers->setFlag(registers->AF_C, result > 0xFFFF);
	reg = result & 0xFFFF;
}

void Instructions::RRC(uint8_t& reg)
{
	registers->setFlag(registers->AF_C, (reg & 0x01) != 0);
	reg = (reg >> 1) | (reg << 7);
	registers->setFlag(registers->AF_Z, reg == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, false);
}

void Instructions::RL(uint8_t& reg)
{
	bool carry = registers->getFlag(registers->AF_C);
	registers->setFlag(registers->AF_C, (reg & 0x80) != 0);
	reg = (reg << 1) | (uint8_t)carry;
	registers->setFlag(registers->AF_Z, reg == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, false);
}

bool Instructions::JR(bool cond)
{
	int8_t offset = memory->read8(registers->pc);
	registers->pc++;
	if (cond)
		registers->pc += offset;
	return cond;
}

void Instructions::RR(uint8_t& reg)
{
	bool carry = registers->getFlag(registers->AF_C);
	registers->setFlag(registers->AF_C, (reg & 0x01) != 0);
	reg = (reg >> 1) | (carry << 7);
	registers->setFlag(registers->AF_Z, reg == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, false);
}

void Instructions::DAA()
{
	uint8_t correction = 0;
	if (registers->getFlag(registers->AF_H) || (!registers->getFlag(registers->AF_N) && (registers->a & 0x0F) > 9))
		correction |= 0x06;
	if (registers->getFlag(registers->AF_C) || (!registers->getFlag(registers->AF_N) && registers->a > 0x99))
		correction |= 0x60;
	if (registers->getFlag(registers->AF_N))
		registers->a -= correction;
	else
		registers->a += correction;
	registers->setFlag(registers->AF_C, correction >= 0x60);
	registers->setFlag(registers->AF_H, false);
	registers->setFlag(registers->AF_Z, registers->a == 0);
}

void Instructions::HALT()
{
	memory->sm83->halted = true;
}

void Instructions::ADD(uint8_t val)
{
	uint16_t result = registers->a + val;
	registers->setFlag(registers->AF_Z, (result & 0xFF) == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, (registers->a & 0x0F) + (val & 0x0F) > 0x0F);
	registers->setFlag(registers->AF_C, result > 0xFF);
	registers->a = result & 0xFF;
}

void Instructions::ADC(uint8_t val)
{
	uint16_t result = registers->a + val + registers->getFlag(registers->AF_C);
	registers->setFlag(registers->AF_Z, (result & 0xFF) == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, (registers->a & 0x0F) + (val & 0x0F) + registers->getFlag(registers->AF_C) > 0x0F);
	registers->setFlag(registers->AF_C, result > 0xFF);
	registers->a = result & 0xFF;
}

void Instructions::SUB(uint8_t val)
{
	uint16_t result = registers->a - val;
	registers->setFlag(registers->AF_Z, (result & 0xFF) == 0);
	registers->setFlag(registers->AF_N, true);
	registers->setFlag(registers->AF_H, (registers->a & 0x0F) < (val & 0x0F));
	registers->setFlag(registers->AF_C, result > 0xFF);
	registers->a = result & 0xFF;
}

void Instructions::SBC(uint8_t val)
{
	uint16_t result = registers->a - val - registers->getFlag(registers->AF_C);
	registers->setFlag(registers->AF_Z, (result & 0xFF) == 0);
	registers->setFlag(registers->AF_N, true);
	registers->setFlag(registers->AF_H, (registers->a & 0x0F) < (val & 0x0F) + registers->getFlag(registers->AF_C));
	registers->setFlag(registers->AF_C, result > 0xFF);
	registers->a = result & 0xFF;
}

void Instructions::AND(uint8_t val)
{
	registers->a &= val;
	registers->setFlag(registers->AF_Z, registers->a == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, true);
	registers->setFlag(registers->AF_C, false);
}

void Instructions::XOR(uint8_t val)
{
	registers->a ^= val;
	registers->setFlag(registers->AF_Z, registers->a == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, false);
	registers->setFlag(registers->AF_C, false);
}

void Instructions::OR(uint8_t val)
{
	registers->a |= val;
	registers->setFlag(registers->AF_Z, registers->a == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, false);
	registers->setFlag(registers->AF_C, false);
}

void Instructions::CP(uint8_t val)
{
	uint16_t result = registers->a - val;
	registers->setFlag(registers->AF_Z, (result & 0xFF) == 0);
	registers->setFlag(registers->AF_N, true);
	registers->setFlag(registers->AF_H, (registers->a & 0x0F) < (val & 0x0F));
	registers->setFlag(registers->AF_C, result > 0xFF);
}

bool Instructions::RET(bool cond)
{
	if (cond)
	{
		registers->pc = memory->read16(registers->sp);
		registers->sp += 2;
	}
	return cond;
}

void Instructions::POP(uint16_t& reg)
{
	reg = memory->read16(registers->sp);
	registers->sp += 2;
}

bool Instructions::JP(bool cond)
{
	if (cond)
		registers->pc = memory->read16(registers->pc);
	else
		registers->pc += 2;
	return cond;
}

bool Instructions::CALL(bool cond)
{
	if (cond)
	{
		registers->sp -= 2;
		memory->write16(registers->sp, registers->pc + 2);
		registers->pc = memory->read16(registers->pc);
	}
	else
		registers->pc += 2;
	return cond;
}

void Instructions::PUSH(uint16_t reg)
{
	registers->sp -= 2;
	memory->write16(registers->sp, reg);
}

void Instructions::RST(uint16_t addr)
{
	registers->sp -= 2;
	memory->write16(registers->sp, registers->pc);
	registers->pc = addr;
}

void Instructions::ADDSP(int8_t val)
{
	uint16_t result = registers->sp + val;
	registers->setFlag(registers->AF_Z, false);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, (registers->sp & 0x0F) + (val & 0x0F) > 0x0F);
	registers->setFlag(registers->AF_C, (registers->sp & 0xFF) + (val & 0xFF) > 0xFF);
	registers->sp = result;
}

void Instructions::LDHL(int8_t val)
{
	uint16_t result = registers->sp + val;
	registers->setFlag(registers->AF_Z, false);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, (registers->hl & 0x0F) + (val & 0x0F) > 0x0F);
	registers->setFlag(registers->AF_C, (registers->hl & 0xFF) + (val & 0xFF) > 0xFF);
	registers->hl = result;
}

// 0xCB instruction set
void Instructions::SLA(uint8_t& reg)
{
	registers->setFlag(registers->AF_C, (reg & 0x80) != 0);
	reg <<= 1;
	registers->setFlag(registers->AF_Z, reg == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, false);
}

void Instructions::SRA(uint8_t& reg)
{
	registers->setFlag(registers->AF_C, (reg & 0x01) != 0);
	reg = (reg & 0x80) | (reg >> 1);
	registers->setFlag(registers->AF_Z, reg == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, false);
}

void Instructions::SWAP(uint8_t& reg)
{
	reg = (reg << 4) | (reg >> 4);
	registers->setFlag(registers->AF_Z, reg == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, false);
	registers->setFlag(registers->AF_C, false);
}

void Instructions::SRL(uint8_t& reg)
{
	registers->setFlag(registers->AF_C, (reg & 0x01) != 0);
	reg >>= 1;
	registers->setFlag(registers->AF_Z, reg == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, false);
}

void Instructions::BIT(uint8_t reg, uint8_t bit)
{
	registers->setFlag(registers->AF_Z, (reg & (1 << bit)) == 0);
	registers->setFlag(registers->AF_N, false);
	registers->setFlag(registers->AF_H, true);
}

void Instructions::RES(uint8_t bit, uint8_t& reg)
{
	reg &= ~(1 << bit);
}

void Instructions::SET(uint8_t bit, uint8_t& reg)
{
	reg |= 1 << bit;
}