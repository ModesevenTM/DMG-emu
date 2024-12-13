#pragma once
#include <SDL.h>
#include <chrono>
#include <thread>
#include "PPU.h"

class Renderer
{
public:
	const int SCREEN_WIDTH = 160;
	const int SCREEN_HEIGHT = 144;
	const double FRAME_TIME = 1000.0 / 60.0;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
	PPU* ppu;

	std::chrono::high_resolution_clock::time_point frameStart;
	std::chrono::high_resolution_clock::time_point frameEnd;

	Renderer(PPU* ppu);
	~Renderer();

	void render();
};

