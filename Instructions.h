#pragma once
#include <cstdint>
#include "Registers.h"
#include "Memory.h"
#include "Interrupts.h"
#include "Timer.h"

class Instructions
{
public:
	Registers* registers;
	Memory* memory;
	Interrupts* interrupts;
	Timer* timer;

	const int INSTR_CYCLES[1 << 8] = {
		1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1,
		1, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1,
		2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1,
		2, 3, 2, 2, 3, 3, 3, 1, 2, 2, 2, 2, 1, 1, 2, 1,
		1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
		1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
		1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
		2, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1,
		1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
		1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
		1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
		1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
		2, 3, 3, 4, 3, 4, 2, 4, 2, 4, 3, 0, 3, 6, 2, 4,
		2, 3, 3, 0, 3, 4, 2, 4, 2, 4, 3, 0, 3, 0, 2, 4,
		3, 3, 2, 0, 0, 4, 2, 4, 4, 1, 4, 0, 0, 0, 2, 4,
		3, 3, 2, 1, 0, 4, 2, 4, 3, 2, 4, 1, 0, 0, 2, 4
	};

	const int JR_COND_TRUE = 1;
	const int RET_COND_TRUE = 3;
	const int JP_COND_TRUE = 1;
	const int CALL_COND_TRUE = 3;
	
	void decode(uint8_t opcode);
	void decodeCB(uint8_t opcode);

	// instruction set
	void INC(uint8_t& reg);
	void DEC(uint8_t& reg);
	void RLC(uint8_t& reg);
	void ADD(uint16_t& reg, uint16_t val);
	void RRC(uint8_t& reg);
	void RL(uint8_t& reg);
	bool JR(bool cond = true);
	void RR(uint8_t& reg);
	void DAA();
	void ADD(uint8_t val);
	void ADC(uint8_t val);
	void SUB(uint8_t val);
	void SBC(uint8_t val);
	void AND(uint8_t val);
	void XOR(uint8_t val);
	void OR(uint8_t val);
	void CP(uint8_t val);
	bool RET(bool cond = true);
	void POP(uint16_t& reg);
	bool JP(bool cond = true);
	bool CALL(bool cond = true);
	void PUSH(uint16_t reg);
	void RST(uint16_t addr);
	void ADDSP(int8_t val);
	void LDHL(int8_t val);

	// 0xCB instruction set
	void SLA(uint8_t& reg);
	void SRA(uint8_t& reg);
	void SWAP(uint8_t& reg);
	void SRL(uint8_t& reg);
	void BIT(uint8_t bit, uint8_t reg);
	void RES(uint8_t bit, uint8_t& reg);
	void SET(uint8_t bit, uint8_t& reg);
};