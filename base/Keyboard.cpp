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
  bool pressed;
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
      pressed = (event.type == SDL_MOUSEBUTTONDOWN);
      switch (event.button.button) {
      case SDL_BUTTON_RIGHT:
	changeKey("rightflip", pressed);
	break;
      case SDL_BUTTON_LEFT:
	changeKey("leftflip", pressed);
	break;
      case SDL_BUTTON_MIDDLE:
	if (isKeyDown(Config::getInstance()->getKey("rightflip"))
	    && isKeyDown(Config::getInstance()->getKey("leftflip"))) {
	  changeKey("bottomnudge", pressed);
	} else if (isKeyDown(Config::getInstance()->getKey("rightflip"))) {
	  changeKey("rightnudge", pressed);
	} else if (isKeyDown(Config::getInstance()->getKey("leftflip"))) {
	  changeKey("leftnudge", pressed);
	} else {
	  changeKey("launch", pressed);
	}
	break;
      }
      break;

    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
      pressed = (event.type == SDL_JOYBUTTONDOWN);

      switch (event.cbutton.button) {
      case 0x04: // Left Shoulder
      case 0x0A: // Left Thumb
	changeKey("leftflip", pressed);
	break;
      case 0x05: // Right Shoulder
      case 0x0B: // Right Thumb
	changeKey("rightflip", pressed);
	break;
      case 0x03: // X
      case 0x06: // Left Trigger
	changeKey("leftnudge", pressed);
	break;
      case 0x01: // B
      case 0x07: // Right Trigger
	changeKey("rightnudge", pressed);
	break;
      case 0x00: // Y
      case 0x08: // Reset
	changeKey("bottomnudge", pressed);
	break;
      case 0x02: // A
      case 0x09: // Start
      case 0x0C: // Home
	changeKey("launch", pressed);
	break;
      }
    default:
      if (false) {
	std::cout
	  << "log: Event not handled (0x" << hex<<event.type
	  << ")" << std::endl;
      }
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


void Keyboard::changeKey(char const *name, bool pressed)
{
#if EM_USE_SDL
  SDL_Event event;
  event.type = (pressed) ? SDL_KEYDOWN : SDL_KEYUP;
  event.key.keysym.sym = Config::getInstance()->getKey(name);
  if (event.key.keysym.sym !=0) {
    SDL_PushEvent(&event);
  }
#else
# warning "TODO"
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
