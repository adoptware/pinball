/***************************************************************************
                          ArmBehavior.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
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
	m_bRot = true;
	this->setType(PBL_TYPE_ARMBEH);
}

ArmBehavior::~ArmBehavior() {
}

void ArmBehavior::onTick() {
	EmAssert(this->getParent() != NULL, "ArmBehavior::onTick");
	// get translation on first tick
	if (m_bRot) {
		this->getParent()->getRotation(m_vtxRot.x, m_vtxRot.y, m_vtxRot.z);
		m_bRot = false;
	}
	
#define DO_ARM(key) \
		if (Keyboard::isKeyDown(key)) {                    \
			if (m_iCount < 10) {                             \
				m_iCount++;                                    \
				this->getParent()->setUserProperty(PBL_ACTIVE_ARM);     \
				this->getParent()->unsetUserProperty(PBL_UNACTIVE_ARM); \
        if (m_iCount == 2) {                           \
          if (m_bRight) {                              \
						SendSignal(PBL_SIG_RIGHTARM_ON, 0, this->getParent(), NULL); \
					} else {                                                    \
						SendSignal(PBL_SIG_LEFTARM_ON, 0, this->getParent(), NULL);  \
					}                                                           \
          SoundUtil::getInstance()->playSample(m_iSound, false);      \
        }                                              \
			} else {                                         \
				this->getParent()->setUserProperty(PBL_UNACTIVE_ARM);   \
				this->getParent()->unsetUserProperty(PBL_ACTIVE_ARM);   \
			}                                                \
		} else {                                           \
			this->getParent()->setUserProperty(PBL_UNACTIVE_ARM);     \
			this->getParent()->unsetUserProperty(PBL_ACTIVE_ARM);     \
			if (m_iCount > 0) m_iCount--;                    \
		}

	if (m_bRight) {
		DO_ARM(SDLK_RSHIFT);
		this->getParent()->setRotation(0.0f, m_vtxRot.y + 0.01f*m_iCount, 0);
	} else {
		DO_ARM(SDLK_LSHIFT);
		this->getParent()->setRotation(0.0f, m_vtxRot.y - 0.01f*m_iCount, 0);
	}
#undef DO_ARM
}
