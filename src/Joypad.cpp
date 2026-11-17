#include "Joypad.h"
#include "SM83.h"

void Joypad::keyDown(uint8_t key, uint8_t select) {
	select == SELECT_DPAD ? DPad &= ~key : buttons &= ~key;
	sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_JOYPAD);
}

void Joypad::keyUp(uint8_t key, uint8_t select) {
	select == SELECT_DPAD ? DPad |= key : buttons |= key;
	sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_JOYPAD);
}

void Joypad::step()
{
	SDL_Event event;
	if (!SDL_PollEvent(&event))
		return;

	switch (event.type)
	{
	case SDL_EVENT_KEY_DOWN:
		switch (event.key.key)
		{
		case SDLK_RIGHT:
			keyDown(KEY_A_RIGHT, SELECT_DPAD);
			break;
		case SDLK_LEFT:
			keyDown(KEY_B_LEFT, SELECT_DPAD);
			break;
		case SDLK_UP:
			keyDown(KEY_SELECT_UP, SELECT_DPAD);
			break;
		case SDLK_DOWN:
			keyDown(KEY_START_DOWN, SELECT_DPAD);
			break;
		case SDLK_Z:
			keyDown(KEY_A_RIGHT, SELECT_BUTTONS);
			break;
		case SDLK_X:
			keyDown(KEY_B_LEFT, SELECT_BUTTONS);
			break;
		case SDLK_N:
			keyDown(KEY_SELECT_UP, SELECT_BUTTONS);
			break;
		case SDLK_M:
			keyDown(KEY_START_DOWN, SELECT_BUTTONS);
			break;
		}
		break;
	case SDL_EVENT_KEY_UP:
		switch (event.key.key)
		{
		case SDLK_RIGHT:
			keyUp(KEY_A_RIGHT, SELECT_DPAD);
			break;
		case SDLK_LEFT:
			keyUp(KEY_B_LEFT, SELECT_DPAD);
			break;
		case SDLK_UP:
			keyUp(KEY_SELECT_UP, SELECT_DPAD);
			break;
		case SDLK_DOWN:
			keyUp(KEY_START_DOWN, SELECT_DPAD);
			break;
		case SDLK_Z:
			keyUp(KEY_A_RIGHT, SELECT_BUTTONS);
			break;
		case SDLK_X:
			keyUp(KEY_B_LEFT, SELECT_BUTTONS);
			break;
		case SDLK_N:
			keyUp(KEY_SELECT_UP, SELECT_BUTTONS);
			break;
		case SDLK_M:
			keyUp(KEY_START_DOWN, SELECT_BUTTONS);
			break;
		}
		break;
	case SDL_EVENT_QUIT:
		sm83->ended = true;
		break;
	}
}