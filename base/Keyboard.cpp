//#ident "$Id: Keyboard.cpp,v 1.4 2003/07/16 20:02:04 rzr Exp $"
/***************************************************************************
                          Keyboard.cpp  -  description
                             -------------------
    begin                : Thu Feb 1 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <cstring>
#include "Private.h"
#include "Keyboard.h"

#if EM_USE_SDL
bool Keyboard::m_abKey[KEY_MAX];
#endif

Keyboard::Keyboard(){
	this->clear();
}

Keyboard::~Keyboard(){
}

void Keyboard::poll() {
#if EM_USE_SDL
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      m_abKey[event.key.keysym.sym] = true;
    }
    if (event.type == SDL_KEYUP) {
      m_abKey[event.key.keysym.sym] = false;
    }
  }
#endif
#if EM_USE_ALLEGRO
  poll_keyboard();
#endif
}

void Keyboard::clear() {
#if EM_USE_SDL
  memset(m_abKey, false, KEY_MAX*sizeof(bool));
#endif
#if EM_USE_ALLEGRO
  clear_keybuf();
#endif
}

EMKey Keyboard::waitForKey() {
#if EM_USE_SDL
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
#endif
#if EM_USE_ALLEGRO
  // TODO
  return (readkey() >> 8);
#endif
}

bool Keyboard::isKeyDown(int piKey) {
  if (piKey < 0 || piKey >= KEY_MAX) return false;
#if EM_USE_SDL
  return m_abKey[piKey];
#endif
#if EM_USE_ALLEGRO
  return key[piKey];
#endif
}
