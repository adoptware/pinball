/***************************************************************************
                          KeyRBeh.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "KeyRotBehavior.h"
#include "Group.h"
#include "Keyboard.h"

KeyRotBehavior::KeyRotBehavior() {
}

KeyRotBehavior::~KeyRotBehavior() {
}

void KeyRotBehavior::onTick() {
	float x = 0, y = 0, z = 0;
	if (Keyboard::isKeyDown(SDLK_RIGHT)) y = 0.01f;	
	if (Keyboard::isKeyDown(SDLK_LEFT)) y = -0.01f;
	if (Keyboard::isKeyDown(SDLK_UP)) x = 0.01f;
	if (Keyboard::isKeyDown(SDLK_DOWN)) x = -0.01f;
	if (Keyboard::isKeyDown(SDLK_PAGEUP)) z = 0.01f;
	if (Keyboard::isKeyDown(SDLK_PAGEDOWN)) z = -0.01f;
	p_Parent->addRotation(x, y, z);
}
