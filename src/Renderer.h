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
	const int64_t FRAME_TIME = 1000000000 / 60;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
	PPU* ppu;

	std::chrono::steady_clock::time_point timePoint = std::chrono::steady_clock::now();

	Renderer(PPU* ppu);
	~Renderer();

	void render();
};

