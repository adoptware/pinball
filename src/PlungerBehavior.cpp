/***************************************************************************
                          PlungerBehavior.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com

    modifs
    2003-05-10  pnf      : Added double key press sends ball with medium
                            power.

 ***************************************************************************/

#include <string>

#include "Private.h"
#include "PlungerBehavior.h"
#include "Pinball.h"
#include "Group.h"
#include "Keyboard.h"
#include "SoundUtil.h"
#include "Loader.h"
#include "Config.h"
#include <iostream>

PlungerBehavior::PlungerBehavior() : Behavior() {
  m_bFirst = true;
  m_iLaunchState = 0;
  m_fPower = 0.0f;
  m_iCounter = -1;
  m_iSound = -1;
  m_sigPlunger = Loader::getInstance()->getSignal("plunger");
  this->setType(PBL_TYPE_PLUNGERBEH);

  m_bDoublePress = false;
}

PlungerBehavior::~PlungerBehavior() {
}

void PlungerBehavior::StdEmptyOnCollision() {
}

void PlungerBehavior::onTick() {
  EM_COUT("PlungerBehavior::onTick()", 0);

  if (m_bFirst) {
    this->getParent()->getTranslation(m_vtxTr.x, m_vtxTr.y, m_vtxTr.z);
    m_bFirst = false;
  }
  
  string launch("launch");
  
  if (Keyboard::isKeyDown(Config::getInstance()->getKey(launch))) {
    // return pressed, pull plunger back
    m_iCounter = -1;
    m_iLaunchState = 1;
    if (m_fPower < 1.0f) {
      m_fPower += 0.005f;
    }
    EM_COUT("PlungerBehavior::onTick " << m_fPower, 0);
  } else {
    if (m_iLaunchState == 1) {
      // return released, fire ball
      m_iLaunchState = 2;
      m_iCounter = 50;
      SendSignal(m_sigPlunger, 0, this->getParent(), NULL);
      SoundUtil::getInstance()->playSample(m_iSound, false);
      
      // If the key was pressed momentanily
      if (m_fPower <= 0.1) {
	// If the flag is already active give medium power to ball
        if (m_bDoublePress) {
          m_fPower = 1.0f;
          m_bDoublePress = false;
          m_iLaunchState = 2;
          m_iCounter = 50;
        } else {
          m_bDoublePress = true;
        }
      } else {
        m_bDoublePress = false;
      }
    } else if (m_iLaunchState == 2) {
      if (m_iCounter > -1)  {
        // short delay after launching
        --m_iCounter;
      } else {
        // reset
        m_iLaunchState = 0;
        m_fPower = 0.0f;
      }
    }
  }
  
  if (m_iLaunchState == 1) {
    this->getParent()->setTranslation(m_vtxTr.x, m_vtxTr.y, m_vtxTr.z + m_fPower*2.0f);
  } else {
    this->getParent()->setTranslation(m_vtxTr.x, m_vtxTr.y, m_vtxTr.z);
  }
}
