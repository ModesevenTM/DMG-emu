#include <iostream>
#include "DMG.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <ROM file>" << std::endl;
		return 1;
	}
	DMG emu(argv[1]);
	emu.start();
    return 0;
}