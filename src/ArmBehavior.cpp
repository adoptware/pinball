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
#include "SoundUtil.h"

ArmBehavior::ArmBehavior(bool right) : Behavior() {
	m_bRight = right;
	m_iCount = 0;
	m_bOn = false;
	m_iSound = -1;
	this->setType(PBL_TYPE_ARMBEH);
}

ArmBehavior::~ArmBehavior() {
}

void ArmBehavior::onTick() {
#define DO_ARM(key) \
		if (Keyboard::isKeyDown(key)) {                    \
			if (m_iCount < 10) {                             \
				m_iCount++;                                    \
				p_Parent->setUserProperty(PBL_ACTIVE_ARM);     \
				p_Parent->unsetUserProperty(PBL_UNACTIVE_ARM); \
        if (m_iCount == 5) {                           \
          if (m_bRight) {                              \
						SendSignal(PBL_SIG_RIGHTARM_ON, 0, this->p_Parent, NULL); \
					} else {                                                    \
						SendSignal(PBL_SIG_LEFTARM_ON, 0, this->p_Parent, NULL);  \
					}                                                           \
          SoundUtil::getInstance()->play(m_iSound, false);            \
        }                                              \
			} else {                                         \
				p_Parent->setUserProperty(PBL_UNACTIVE_ARM);   \
				p_Parent->unsetUserProperty(PBL_ACTIVE_ARM);   \
			}                                                \
		} else {                                           \
			p_Parent->setUserProperty(PBL_UNACTIVE_ARM);     \
			p_Parent->unsetUserProperty(PBL_ACTIVE_ARM);     \
			if (m_iCount > 0) m_iCount--;                    \
		}

	if (m_bRight) {
		DO_ARM(SDLK_RSHIFT);
		p_Parent->setRotation(0.0f, -0.05f + 0.01f*m_iCount, 0);
	} else {
		DO_ARM(SDLK_LSHIFT);
		p_Parent->setRotation(0.0f, 0.05f - 0.01f*m_iCount, 0);
	}
#undef DO_ARM
}
