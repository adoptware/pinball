/***************************************************************************
                          ArmBehavior.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com


    ========================= Modifications =========================

        Apr. 6, 2017:
            - Replace name of "m_bOn" with "m_bActive". (c30zD)
            - Add events for game over and game start signals. (c30zD)
            - Modify function doArm to also use m_bActive. (c30zD)

 ***************************************************************************/

#include "Private.h"
#include "ArmBehavior.h"
#include "Group.h"
#include "Shape3D.h"
#include "Pinball.h"
#include "Keyboard.h"
#include "SoundUtil.h"
#include "Config.h"

ArmBehavior::ArmBehavior(bool right) : Behavior() {
  m_bTilt = false;
  m_bRight = right;
  m_iCount = 0;
  m_bActive = false;
  m_iSound = -1;
  m_bFirst = true;
  this->setType(PBL_TYPE_ARMBEH);
}

ArmBehavior::~ArmBehavior() {
}

void ArmBehavior::StdOnSignal() {
  OnSignal(PBL_SIG_RESET_ALL) {
    m_bTilt = false;
    m_iCount = 0;
    this->getParent()->setUserProperty(PBL_UNACTIVE_ARM);
    this->getParent()->unsetUserProperty(PBL_ACTIVE_ARM);
    this->getParent()->setRotation(0.0f, m_vtxRot.y, 0.0f);
    m_bActive = false;
  }
  ElseOnSignal(PBL_SIG_TILT) {
    m_bTilt = true;
  }
  ElseOnSignal(PBL_SIG_GAME_START) {
    m_bActive = true;
  }
  ElseOnSignal(PBL_SIG_GAME_OVER) {
    m_bActive = false;
  }
}

void ArmBehavior::doArm(EMKey key) {
  if (Keyboard::isKeyDown(key) && !m_bTilt && m_bActive) {
    if (m_iCount < 10) {
      m_iCount++;
      this->getParent()->setUserProperty(PBL_ACTIVE_ARM);
      this->getParent()->unsetUserProperty(PBL_UNACTIVE_ARM);
      if (m_iCount == 2) {
	if (m_bRight) {
	  SendSignal(PBL_SIG_RIGHTARM_ON, 0, this->getParent(), NULL);
	} else {
	  SendSignal(PBL_SIG_LEFTARM_ON, 0, this->getParent(), NULL);
	}
	SoundUtil::getInstance()->playSample(m_iSound, false);
      }
    } else {
      this->getParent()->setUserProperty(PBL_UNACTIVE_ARM);
      this->getParent()->unsetUserProperty(PBL_ACTIVE_ARM);
    }
  } else {
    this->getParent()->setUserProperty(PBL_UNACTIVE_ARM);
    this->getParent()->unsetUserProperty(PBL_ACTIVE_ARM);
    if (m_iCount > 0) m_iCount--;
  }
}

void ArmBehavior::onTick() {
  EM_COUT("ArmBehavior::onTick", 0);
  EmAssert(this->getParent() != NULL, "ArmBehavior::onTick");
  // get translation on first tick
  if (m_bFirst) {
    this->getParent()->getRotation(m_vtxRot.x, m_vtxRot.y, m_vtxRot.z);
    m_bFirst = false;
  }

  if (m_bRight) {
    string leftflip("rightflip");
    this->doArm(Config::getInstance()->getKey(leftflip));
    this->getParent()->setRotation(0.0f, m_vtxRot.y + 0.01f*m_iCount, 0.0f);
  } else {
    string rightflip("leftflip");
    this->doArm(Config::getInstance()->getKey(rightflip));
    this->getParent()->setRotation(0.0f, m_vtxRot.y - 0.01f*m_iCount, 0.0f);
  }
}
