#include "Renderer.h"

Renderer::Renderer(PPU* ppu) : ppu(ppu)
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("DMG-emu", SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, 0);
	renderer = SDL_CreateRenderer(window, NULL);
	SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	SDL_SetWindowResizable(window, true);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}

Renderer::~Renderer()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Renderer::render()
{
	timePoint += std::chrono::nanoseconds{FRAME_TIME};
	std::this_thread::sleep_until(timePoint);

	SDL_SetRenderTarget(renderer, texture);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	SDL_UpdateTexture(texture, NULL, ppu->frameBuffer, SCREEN_WIDTH * 4);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

}