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
	m_x = 0;
	m_y = 0;
	m_z = 0;
}

KeyRotBehavior::~KeyRotBehavior() {
}

void KeyRotBehavior::onTick() {
	if (Keyboard::isKeyDown(KEY_RIGHT)) m_y += 0.01f;	
	if (Keyboard::isKeyDown(KEY_LEFT)) m_y -= 0.01f;
	if (Keyboard::isKeyDown(KEY_UP)) m_x += 0.01f;
	if (Keyboard::isKeyDown(KEY_DOWN)) m_x -= 0.01f;
	if (Keyboard::isKeyDown(KEY_PGUP)) m_z += 0.01f;
	if (Keyboard::isKeyDown(KEY_PGDN)) m_z -= 0.01f;
	p_Parent->setRotation(m_x, m_y, m_z);
}
