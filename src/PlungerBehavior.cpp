/***************************************************************************
                          PlungerBehavior.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "PlungerBehavior.h"
#include "Pinball.h"
#include "Group.h"
#include "Keyboard.h"
#include "SoundUtil.h"
#include "Loader.h"

PlungerBehavior::PlungerBehavior() : Behavior() {
  m_iLaunchState = 0;
  m_fPower = 0.0f;
  m_iCounter = -1;
  m_iSound = -1;
  m_sigPlunger = Loader::getInstance()->getSignal("plunger");
  this->setType(PBL_TYPE_PLUNGERBEH);
}

PlungerBehavior::~PlungerBehavior() {
}

void PlungerBehavior::StdEmptyOnCollision() {
//  OnCallerProperty( PBL_BALL_1 OR_CP PBL_BALL_2 OR_CP PBL_BALL_3 OR_CP PBL_BALL_4 ) {
//     if (!m_bLaunch || m_iCounter > 0) {
//       return;
//     }
//     m_iCounter = 10;
//     SendSignal(m_sigPlunger, 0, this->getParent(), NULL);
//     SoundUtil::getInstance()->playSample(m_iSound, false);
//  }
}

void PlungerBehavior::onTick() {
  if (Keyboard::isKeyDown(SDLK_RETURN)) {
    // return pressed, pull plunger back
    m_iCounter = -1;
    m_iLaunchState = 1;
    m_fPower += 0.01f;
    EM_COUT("PlungerBehavior::onTick " << m_fPower, 1);
  } else {
    if (m_iLaunchState == 1) {
      // return released, fire ball
      m_iLaunchState = 2;
      m_iCounter = 10;
      SendSignal(m_sigPlunger, 0, this->getParent(), NULL);
      SoundUtil::getInstance()->playSample(m_iSound, false);
    } else if (m_iLaunchState == 2) {
      if (m_iCounter > -1) {
	// short delay after launching
				--m_iCounter;
      } else {
	// reset
				m_iLaunchState = 0;
				m_fPower = 0.0f;
      }
    }
  }
}
