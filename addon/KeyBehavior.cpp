/***************************************************************************
                          KeyBeh.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
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
	EM_COUT("KeyBehavior::onCollision()", 1);
}

void KeyBehavior::onTick() {
	EmAssert(this->getParent() != NULL, "KeyBehavior::onTick parent NULL");
	m_x = this->getParent()->m_mtxSrc.t[0];
	m_y = this->getParent()->m_mtxSrc.t[1];
	m_z = this->getParent()->m_mtxSrc.t[2];
	if (Keyboard::isKeyDown(SDLK_a)) m_x -= 0.2;
	if (Keyboard::isKeyDown(SDLK_d)) m_x += 0.2;
	if (Keyboard::isKeyDown(SDLK_w)) m_z -= 0.2;
	if (Keyboard::isKeyDown(SDLK_s)) m_z += 0.2;
	if (Keyboard::isKeyDown(SDLK_q)) m_y -= 0.2;
	if (Keyboard::isKeyDown(SDLK_e)) m_y += 0.2;
	this->getParent()->setTranslation(m_x, m_y, m_z);
}
