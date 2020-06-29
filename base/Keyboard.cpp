/***************************************************************************
                          Keyboard.cpp  -  description
                             -------------------
    begin                : Thu Feb 1 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <cassert>
#include <cstring>
#include "Private.h"

#include <cassert>
#include <cstring>
#include <iostream>

#include "Keyboard.h"
#include "Config.h"

#if EM_USE_SDL
std::map<EMKey, bool> Keyboard::m_abKey;
#endif

Keyboard::Keyboard(){
  this->clear();
}

Keyboard::~Keyboard(){
}

void Keyboard::poll()
{
#if EM_USE_SDL
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch(event.type) {      
    case SDL_KEYDOWN:
      m_abKey[event.key.keysym.sym] = true;
      break;
    case SDL_KEYUP:
      m_abKey[event.key.keysym.sym] = false;
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      SDL_Event newEvent;
      newEvent.type = (event.type == SDL_MOUSEBUTTONDOWN)
	? SDL_KEYDOWN : SDL_KEYUP;
      switch (event.button.button) {
      case SDL_BUTTON_RIGHT:
	if (! isKeyDown(Config::getInstance()->getKey("launch"))) {
	  newEvent.key.keysym.sym
	    = Config::getInstance()->getKey("rightflip");
	} else {
	  newEvent.key.keysym.sym
	    = Config::getInstance()->getKey("rightnudge");
	}	  
	break;
      case SDL_BUTTON_LEFT:
	if (! isKeyDown(Config::getInstance()->getKey("launch"))) {
	  newEvent.key.keysym.sym
	    = Config::getInstance()->getKey("leftflip");
	} else {
	  newEvent.key.keysym.sym
	    = Config::getInstance()->getKey("leftnudge");
	}
	break;
      case SDL_BUTTON_MIDDLE:
	if (isKeyDown(Config::getInstance()->getKey("rightflip"))
	    && isKeyDown(Config::getInstance()->getKey("leftflip"))) {
	  newEvent.key.keysym.sym
	    = Config::getInstance()->getKey("bottomnudge");
	} else if (isKeyDown(Config::getInstance()->getKey("rightflip"))) {
	  newEvent.key.keysym.sym
	    = Config::getInstance()->getKey("rightnudge");
	} else if (isKeyDown(Config::getInstance()->getKey("leftflip"))) {
	  newEvent.key.keysym.sym
	    = Config::getInstance()->getKey("leftnudge");
	} else {
	  newEvent.key.keysym.sym
	    = Config::getInstance()->getKey("launch");
	}
	      
	break;
      }
      if (newEvent.key.keysym.sym !=0)
	SDL_PushEvent(&newEvent);
      return;
      break;
    }
  }
#endif
#if EM_USE_ALLEGRO
  poll_keyboard();
#endif
}

void Keyboard::clear() {
#if EM_USE_SDL
  m_abKey.clear();
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

bool Keyboard::isKeyDown(EMKey piKey)
{
 if (piKey < 0) return false;
#if EM_USE_SDL
  return m_abKey[piKey];
#endif
#if EM_USE_ALLEGRO
  return key[piKey];
#endif
}

void Keyboard::generate(EMKey sym)
{
#if EM_USE_SDL
  SDL_Event event = {};
  event.type = SDL_KEYDOWN;
  event.key.state = SDL_PRESSED;
  event.key.keysym.sym = sym;
  SDL_PushEvent(&event);
  event.type = SDL_KEYDOWN;
  event.key.state = SDL_RELEASED;
  event.key.keysym.sym = sym;
  SDL_PushEvent(&event);
#else
# warning "TODO"
#endif
}
