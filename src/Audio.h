#include <cstdint>

class Audio
{
public:
	// registers
	uint8_t NR1[5];		// Channel 1
	uint8_t NR2[4];		// Channel 2
	uint8_t NR3[5];		// Channel 3
	uint8_t NR4[4];		// Channel 4
	uint8_t NR5[3];		// Control registers
	uint8_t WAV[16];	// Wave Pattern RAM

	Audio() {};
};