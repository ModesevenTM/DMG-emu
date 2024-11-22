#pragma once
#include <SDL.h>
#include "PPU.h"

class Renderer
{
public:
	const int SCREEN_WIDTH = 160;
	const int SCREEN_HEIGHT = 144;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
	PPU* ppu;

	Renderer(PPU* ppu);
	~Renderer();

	void render();
};

