#include "Joypad.h"
#include "SM83.h"

void Joypad::keyDown(uint8_t key, uint8_t select) {
	P1 &= ~(key | select);
	sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_JOYPAD);
}

void Joypad::keyUp(uint8_t key, uint8_t select) {
	P1 |= key | select;
	sm83->interrupts.requestInterrupt(sm83->interrupts.MASK_INT_JOYPAD);
}

//void Joypad::step() {
//	uint8_t keys = P1 & 0x30;
//	if (keys == 0x10) P1 |= KEY_A_RIGHT;
//	else if (keys == 0x20) P1 |= KEY_B_LEFT;
//	else if (keys == 0x30) P1 |= KEY_A_RIGHT | KEY_B_LEFT;
//
//	keys = P1 & 0xC0;
//	if (keys == 0x40) P1 |= KEY_SELECT_UP;
//	else if (keys == 0x80) P1 |= KEY_START_DOWN;
//	else if (keys == 0xC0) P1 |= KEY_SELECT_UP | KEY_START_DOWN;
//}

void Joypad::step()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
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
		case SDLK_z:
			keyDown(KEY_A_RIGHT, SELECT_BUTTONS);
			break;
		case SDLK_x:
			keyDown(KEY_B_LEFT, SELECT_BUTTONS);
			break;
		case SDLK_n:
			keyDown(KEY_SELECT_UP, SELECT_BUTTONS);
			break;
		case SDLK_m:
			keyDown(KEY_START_DOWN, SELECT_BUTTONS);
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym)
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
		case SDLK_z:
			keyUp(KEY_A_RIGHT, SELECT_BUTTONS);
			break;
		case SDLK_x:
			keyUp(KEY_B_LEFT, SELECT_BUTTONS);
			break;
		case SDLK_n:
			keyUp(KEY_SELECT_UP, SELECT_BUTTONS);
			break;
		case SDLK_m:
			keyUp(KEY_START_DOWN, SELECT_BUTTONS);
			break;
		}
		break;
	case SDL_QUIT:
		sm83->ended = true;
		break;
	}
}