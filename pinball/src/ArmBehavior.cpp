/***************************************************************************
                          KeyBeh.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/


#include "ArmBehavior.h"
#include "Group.h"
#include "Shape3D.h"
#include "Pinball.h"
#include "Keyboard.h"

ArmBehavior::ArmBehavior(bool right) : Behavior() {
	m_bRight = right;
	m_iCount = 0;
	m_bOn = false;
}

ArmBehavior::~ArmBehavior() {
}

void ArmBehavior::onTick() {
	if (m_bRight) {
		if (Keyboard::isKeyDown(KEY_RSHIFT)) {
			p_Parent->setUserProperty(PBL_ACTIVE_ARM);
			p_Parent->unsetUserProperty(PBL_UNACTIVE_ARM);
			if (m_iCount < 6) {
				m_iCount++;
			} else {
				p_Parent->setUserProperty(PBL_UNACTIVE_ARM);
				p_Parent->unsetUserProperty(PBL_ACTIVE_ARM);
			}
		} else {
			p_Parent->setUserProperty(PBL_UNACTIVE_ARM);
			p_Parent->unsetUserProperty(PBL_ACTIVE_ARM);
			if (m_iCount > 0) m_iCount--;
		}
		p_Parent->setRotation(0.0f, -0.06f + 0.02f*m_iCount, 0.5f);
	} else {
		if (Keyboard::isKeyDown(KEY_LSHIFT)) {
			p_Parent->setUserProperty(PBL_ACTIVE_ARM);
			p_Parent->unsetUserProperty(PBL_UNACTIVE_ARM);
			if (m_iCount < 6) {
				m_iCount++;
			} else {
				p_Parent->setUserProperty(PBL_UNACTIVE_ARM);
				p_Parent->unsetUserProperty(PBL_ACTIVE_ARM);
			}
		} else {
			p_Parent->setUserProperty(PBL_UNACTIVE_ARM);
			p_Parent->unsetUserProperty(PBL_ACTIVE_ARM);
			if (m_iCount > 0) m_iCount--;
		}
		p_Parent->setRotation(0.0f, 0.06f - 0.02f*m_iCount, 0);
	}
}
