/***************************************************************************
                          Keyboard.cpp  -  description
                             -------------------
    begin                : Thu Feb 1 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Keyboard.h"

SDL_Event eEvent;
bool abKey[KEY_MAX];

Keyboard::Keyboard(){
	memset(abKey, false, KEY_MAX*sizeof(bool));
}

Keyboard::~Keyboard(){
}

void Keyboard::poll() {
	while(SDL_PollEvent(&eEvent)) {
		if (eEvent.type == SDL_KEYDOWN) {
			abKey[eEvent.key.keysym.sym] = true;
		}
		if (eEvent.type == SDL_KEYUP) {
			abKey[eEvent.key.keysym.sym] = false;
		}
	}
}

void Keyboard::clear() {
	memset(abKey, false, KEY_MAX*sizeof(bool));
}

SDLKey Keyboard::waitForKey() {
	while(true) {
		SDL_Event event;
		SDL_WaitEvent(&event);
		if (event.type == SDL_KEYDOWN && event.key.state == SDL_PRESSED) {
			break;
		}
	}
	while (true) {
		SDL_Event event;
		SDL_WaitEvent(&event);
		if (event.type == SDL_KEYUP && event.key.state == SDL_RELEASED) {
			return event.key.keysym.sym;
		}
	}
}

bool Keyboard::isKeyDown(int piKey) {
	return abKey[piKey];
}
