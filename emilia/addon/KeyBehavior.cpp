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
	if (Keyboard::isKeyDown(KEY_A)) m_x -= 0.1;
	if (Keyboard::isKeyDown(KEY_D)) m_x += 0.1;
	if (Keyboard::isKeyDown(KEY_W)) m_z -= 0.1;
	if (Keyboard::isKeyDown(KEY_S)) m_z += 0.1;
	if (Keyboard::isKeyDown(KEY_Q)) m_y -= 0.1;
	if (Keyboard::isKeyDown(KEY_E)) m_y += 0.1;
	p_Parent->setTranslation(m_x, m_y, m_z);
}
