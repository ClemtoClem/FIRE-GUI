#include "input.h"
#include "logger.h"

SDL_INPUT *input = NULL;

bool Input_init(bool usedScancode)
{
	bool success = false;
	if (input == NULL) {
		input = calloc(1, sizeof(SDL_INPUT));
		if (input != NULL) {
			input->usedScancode = usedScancode;
			
			SDL_GetGlobalMouseState(&input->mouse.x, &input->mouse.y);
			SDL_GetGlobalMouseState(&input->mouse.lastX, &input->mouse.lastY);

			success = true;
		}
	} else {
		success = true;
	}
	return success;
}

void Input_free()
{
	if (input != NULL) {
		free(input);
		input = NULL;
	}
}

SDL_INPUT *Input_get()
{
	return input;
}

void Input_resetFrame()
{
	if (input != NULL && input->hasEvent == true) {
		memset(input->pressedKeys, 0, NB_KEYS_EVENT);
		memset(input->releasedKeys, 0, NB_KEYS_EVENT);
		memset(input->mouse.pressed, 0, NB_MOUSE_EVENT);
		memset(input->mouse.released, 0, NB_MOUSE_EVENT);
		memset(input->userEvent, 0, NB_USER_EVENT);
		if (input->mouse.wheelX != 0) input->mouse.wheelX = 0;
		if (input->mouse.wheelY != 0) input->mouse.wheelY = 0;
		if (input->mouse.wheelEvent) input->mouse.wheelEvent = false;
		if (input->mouse.moveX != 0) input->mouse.moveX = 0;
		if (input->mouse.moveY != 0) input->mouse.moveY = 0;
		if (input->mouse.moveEvent) input->mouse.moveEvent = false;
		if (input->quitEvent) input->quitEvent = false;
		if (input->hasEvent) input->hasEvent = false;
		if (input->window.gainFocused) input->window.gainFocused = false;
		if (input->window.loseFocused) input->window.loseFocused = false;
		if (input->window.resized) input->window.resized = false;
		if (input->window.maximized) input->window.maximized = false;
		if (input->window.minimized) input->window.minimized = false;
		if (input->window.moved) input->window.moved = false;
	}
}

void Input_pushEvent(Uint32 type, Uint32 code)
{
	if (input != NULL) {
		// On génère un évènement
		SDL_Event evenement;
		evenement.type = type;
		evenement.user.code = code;
		SDL_PushEvent(&evenement);
		input->hasEvent = true;
	}
}

void Input_pushUserEvent(Uint32 code)
{
	if (input != NULL) {
		// On génère un évènement
		SDL_Event evenement;
		evenement.type = SDL_USEREVENT;
		evenement.user.code = code;
		SDL_PushEvent(&evenement);
		input->hasEvent = true;
	}
}

void Input_event()
{
	if (input != NULL) {
		Input_resetFrame(input);
		SDL_Scancode touch;
		while(SDL_PollEvent(&input->event)) {
			switch(input->event.type) {
				case SDL_QUIT: /* évènement quitter la fenêtre */
					input->quitEvent = true;
					input->hasEvent = true;
					break;
				case SDL_KEYDOWN: /* évènement presser un bouton */
					if (input->usedScancode) touch = input->event.key.keysym.scancode;
					else touch = SDL_GetScancodeFromKey(input->event.key.keysym.sym);
					
					if (!input->heldKeys[touch]) {
						input->pressedKeys[touch] = true;
						input->releasedKeys[touch] = false;
						input->heldKeys[touch] = true;
						input->hasEvent = true;
					}
					break;
				case SDL_KEYUP: /* évènement relâcher un bouton */
					if (input->usedScancode) touch = input->event.key.keysym.scancode;
					else touch = SDL_GetScancodeFromKey(input->event.key.keysym.sym);
					
					input->pressedKeys[touch] = false;
					input->releasedKeys[touch] = true;
					input->heldKeys[touch] = false;
					input->hasEvent = true;
					break;
				case SDL_MOUSEMOTION: /* évènement déplacer la souris */
					input->mouse.lastX = input->mouse.x;
					input->mouse.lastY = input->mouse.y;
					input->mouse.x = input->event.button.x;
					input->mouse.y = input->event.button.y;
					input->mouse.moveX = (input->mouse.x - input->mouse.lastX)/2;
					input->mouse.moveY = (input->mouse.y - input->mouse.lastY)/2;
					if (input->mouse.moveX != 0 || input->mouse.moveY != 0) {
						input->mouse.moveEvent = true;
						input->hasEvent = true;
					}
					break;
				case SDL_MOUSEBUTTONDOWN: /* évènement presser un bouton de la souris */
					input->mouse.pressed[input->event.button.button] = true;
					input->mouse.released[input->event.button.button] = false;
					input->mouse.held[input->event.button.button] = true;
					input->hasEvent = true;
					break;
				case SDL_MOUSEBUTTONUP: /* évènement relâcher un bouton de la souris */
					input->mouse.pressed[input->event.button.button] = false;
					input->mouse.released[input->event.button.button] = true;
					input->mouse.held[input->event.button.button] = false;
					input->hasEvent = true;
					break;
				case SDL_MOUSEWHEEL:
					input->mouse.wheelX = input->event.wheel.x;
					if (input->event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
						input->mouse.wheelY = -input->event.wheel.y;
					else 
						input->mouse.wheelY = input->event.wheel.y;
					input->mouse.wheelEvent = true;
					input->hasEvent = true;
					break;
				case SDL_WINDOWEVENT:
					switch (input->event.window.event) {
						case SDL_WINDOWEVENT_FOCUS_GAINED:
					 		input->window.gainFocused = true;
					 		input->window.focused = true;
							break;
						case SDL_WINDOWEVENT_FOCUS_LOST:
							input->window.loseFocused = true;
					 		input->window.focused = false;
							break;
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							input->window.resized = true;
							input->window.width  = input->event.window.data1;
							input->window.height = input->event.window.data2;
							break;
						case SDL_WINDOWEVENT_MAXIMIZED:
							input->window.maximized = true;
							break;
						case SDL_WINDOWEVENT_MINIMIZED:
							input->window.minimized = true;
							break;
						case SDL_WINDOWEVENT_MOVED:
							input->window.moved = true;
							break;
					}
					break;
				case SDL_USEREVENT:
					input->userEvent[input->event.user.code] = true;
					break;
				default:  /* autre évènements inutiles */
					input->hasEvent = false;
					break;
			}
		}

	}
}

bool Input_quitEvent()
{
	if (input != NULL) {
		return input->quitEvent;
	}
	return false;
}

bool Input_WindowGainFocused()
{
	
	if (input != NULL) {
		return input->window.gainFocused;
	}
	return false;
}

bool _Input_wasKeyPressed(int firstKey, ...)
{
	bool pressed = false;
	if (input != NULL) {
		int key = firstKey;
		SDL_Scancode touch;
		va_list args;
		va_start(args, firstKey);
		while (key != -1) {
			if (input->usedScancode) touch = key;
			else touch = SDL_GetScancodeFromKey(key);
			pressed = pressed || input->pressedKeys[touch];
			key = va_arg(args, int);
		}
		va_end(args);
	}
	return pressed;
}

bool _Input_wasKeyReleased(int firstKey, ...)
{
	bool released = false;
	if (input != NULL) {
		int key = firstKey;
		SDL_Scancode touch;
		va_list args;
		va_start(args, firstKey);
		while (key != -1) {
			if (input->usedScancode) touch = key;
			else touch = SDL_GetScancodeFromKey(key);
			released = released || input->releasedKeys[touch];
			key = va_arg(args, int);
		}
		va_end(args);
	}
	return released;
}

bool _Input_wasKeyHeld(int firstKey, ...)
{
	bool held = false;
	if (input != NULL) {
		int key = firstKey;
		SDL_Scancode touch;
		va_list args;
		va_start(args, firstKey);
		while (key != -1) {
			if (input->usedScancode) touch = key;
			else touch = SDL_GetScancodeFromKey(key);
			held = held || input->heldKeys[touch];
			key = va_arg(args, int);
		}
		va_end(args);
	}
	return held;
}

bool _Input_wasMousePressed(Uint8 firstButton, ...)
{
	bool pressed = false;
	if (input != NULL) {
		int button = firstButton;
		va_list args;
		va_start(args, firstButton);
		while (button != -1) {
			pressed = pressed || input->mouse.pressed[button];
			button = va_arg(args, int);
		}
		va_end(args);
	}
	return pressed;
}

bool _Input_wasMouseReleased(Uint8 firstButton, ...)
{
	bool released = false;
	if (input != NULL) {
		int button = firstButton;
		va_list args;
		va_start(args, firstButton);
		while (button != -1) {
			released = released || input->mouse.released[button];
			button = va_arg(args, int);
		}
		va_end(args);
	}
	return released;
}

bool _Input_wasMouseHeld(Uint8 firstButton, ...)
{
	bool held = false;
	if (input != NULL) {
		int button = firstButton;
		va_list args;
		va_start(args, firstButton);
		while (button != -1) {
			held = held || input->mouse.held[button];
			button = va_arg(args, int);
		}
		va_end(args);
	}
	return held;
}

void Input_getMousePosition(SDL_Point *pos)
{
	if (input != NULL) {
		pos->x = input->mouse.x;
		pos->y = input->mouse.y;
	}
}

bool Input_wasMotionEvent()
{
	if (input != NULL) {
		return input->mouse.moveEvent;
	}
	return false;
}

bool Input_wasWheelEvent(Sint32 *wheelX, Sint32 *wheelY)
{
	if (input != NULL) {
		if (wheelX != NULL) *wheelX = input->mouse.wheelX;
		if (wheelY != NULL) *wheelY = input->mouse.wheelY;
		return input->mouse.wheelEvent;
	}
	return false;
}

bool _Input_wasUserEvent(int fisrtCode, ...)
{
	int event = false;
	if (input != NULL) {
		int code = fisrtCode;
		va_list args;
		va_start(args, fisrtCode);
		while (code != -1) {
			event = event || input->userEvent[code];
			code = va_arg(args, int);
		}
		va_end(args);
	}
	return event;
}

int Input_getMouseMoveX()
{
	int x = 0;
	if (input != NULL) x = input->mouse.moveX;
	return x;
}

int Input_getMouseMoveY()
{
	int y = 0;
	if (input != NULL) y = input->mouse.moveY;
	return y;
}