/***************************************************************************
                          KeyBeh.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "KeyBehavior.h"
#include "Group.h"
#include "Shape3D.h"
#include "Keyboard.h"

KeyBehavior::KeyBehavior() : Behavior() {
	m_x = 0;
	m_y = 0;
	m_z = 0;
}

KeyBehavior::~KeyBehavior() {
}

void KeyBehavior::onCollision(const Vertex3D & vtxWall, const Vertex3D & vtxOwn, Group* g) {
	cout << "KeyBehavior::onCollision()" << endl;
}

void KeyBehavior::onTick() {
	m_x = p_Parent->m_mtxSrc.t[0];
	m_y = p_Parent->m_mtxSrc.t[1];
	m_z = p_Parent->m_mtxSrc.t[2];
	if (Keyboard::isKeyDown(SDLK_a)) m_x -= 0.1;
	if (Keyboard::isKeyDown(SDLK_d)) m_x += 0.1;
	if (Keyboard::isKeyDown(SDLK_w)) m_z -= 0.1;
	if (Keyboard::isKeyDown(SDLK_s)) m_z += 0.1;
	if (Keyboard::isKeyDown(SDLK_q)) m_y -= 0.1;
	if (Keyboard::isKeyDown(SDLK_e)) m_y += 0.1;
	p_Parent->setTranslation(m_x, m_y, m_z);
}
