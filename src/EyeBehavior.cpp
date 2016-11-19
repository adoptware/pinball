//#ident "$Id: EyeBehavior.cpp,v 1.15 2003/06/13 13:39:46 rzr Exp $"
/***************************************************************************
                          EyeBehavior.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
***************************************************************************/

#include <string>

#include "Private.h"
#include "EyeBehavior.h"
#include "Group.h"
#include "Pinball.h"
#include "Config.h"
#include "Score.h"
#include "Keyboard.h"
#include "SoundUtil.h"
#include "Table.h"
#include "BallGroup.h"

#define DIFF_FACTOR 0.05f

EyeBehavior::EyeBehavior() : Behavior() {
  m_bTilt = false;
  m_iNudgeTick = 0;
  m_iNudgeType = 0;
  m_iTiltTick = 0;
  m_fXNudge = 0.0f;
  m_fZNudge = 0.0f;
  m_iSound = -1;
  this->setType(PBL_TYPE_EYEBEH);
  m_bFButton = false;
}

EyeBehavior::~EyeBehavior() {
}

void EyeBehavior::StdOnSignal() {
  OnSignal(PBL_SIG_RESET_ALL) {
    m_iNudgeTick = 0;
    m_iNudgeType = 0;
    m_iTiltTick = 0;
    m_fXNudge = 0.0f;
    m_fZNudge = 0.0f;
    m_bTilt = false;
  }
}

void EyeBehavior::onTick() {
  EM_COUT("EyeBehavior::onTick()", 0);
  Table * table = Table::getInstance();
  EmAssert(table->getBall(0) != NULL, "Ball1 group NULL");
  EmAssert(table->getBall(1) != NULL, "Ball2 group NULL");
  EmAssert(table->getBall(2) != NULL, "Ball3 group NULL");
  EmAssert(this->getParent() != NULL, "Parent group NULL");

  // shortcuts for changing view F1, F2, F2, F4 // now F5 F6 F7 F8
  // I changed to F5 F6 F7 F8 to use the Alt+F4 quit shortcut //!rzr
  if (Keyboard::isKeyDown(SDLK_F5)) {
    Config::getInstance()->setView(0);
  } else if (Keyboard::isKeyDown(SDLK_F6)) {
    Config::getInstance()->setView(1);
  } else if (Keyboard::isKeyDown(SDLK_F7)) {
    Config::getInstance()->setView(2);
  } else if (Keyboard::isKeyDown(SDLK_F8)) {
    Config::getInstance()->setView(3);
  } else if (Keyboard::isKeyDown(SDLK_F9)) {
    Config::getInstance()->setView(4);
  } else if (Keyboard::isKeyDown(SDLK_F10)) { //rotated screen 
    Config::getInstance()->setView(5);
  }

  // the nudge code is here ----------------
  if (m_iNudgeTick < 1) {
    string bottomnudge("bottomnudge");
    string leftnudge("leftnudge");
    string rightnudge("rightnudge");
    if (Keyboard::isKeyDown(Config::getInstance()->getKey(bottomnudge)) && !m_bTilt) {
      m_fZNudge = 2.0f;
      m_iTiltTick += 125;
      m_iNudgeTick = 75;
      m_iNudgeType = PBL_SIG_BNUDGE;
      SendSignal(PBL_SIG_BNUDGE, 0, this->getParent(), NULL ); 
      SoundUtil::getInstance()->playSample(m_iSound, false);
    } else if (Keyboard::isKeyDown(Config::getInstance()->getKey(leftnudge)) && !m_bTilt) {
      m_fXNudge = -2.0f;
      m_iTiltTick += 125;
      m_iNudgeTick = 75;
      m_iNudgeType = PBL_SIG_LNUDGE;
      SendSignal(PBL_SIG_LNUDGE, 0, this->getParent(), NULL ); 
      SoundUtil::getInstance()->playSample(m_iSound, false);
    } else if (Keyboard::isKeyDown(Config::getInstance()->getKey(rightnudge)) && !m_bTilt) {
      m_fXNudge = 2.0f;
      m_iTiltTick += 125;
      m_iNudgeTick = 75;
      m_iNudgeType = PBL_SIG_RNUDGE;
      SendSignal(PBL_SIG_RNUDGE, 0, this->getParent(), NULL ); 
      SoundUtil::getInstance()->playSample(m_iSound, false);
    }
  } else if (m_iNudgeTick == 40) {
    // after you nudged the table moves back to it's original position which
    // makes the ball go back in the other direction
    switch (m_iNudgeType) {
    case PBL_SIG_BNUDGE: 
      SendSignal(PBL_SIG_TNUDGE, 0, this->getParent(), NULL ); 
      break;
    case PBL_SIG_LNUDGE: 
      SendSignal(PBL_SIG_RNUDGE, 0, this->getParent(), NULL ); 
      break;
    case PBL_SIG_RNUDGE: 
      SendSignal(PBL_SIG_LNUDGE, 0, this->getParent(), NULL ); 
      break;
    }
    m_fXNudge = 0.0f;
    m_fZNudge = 0.0f;
    m_iNudgeType = 0;
  }  
  if (m_iNudgeTick > 0) {
    m_iNudgeTick--;
  }

  if (m_iTiltTick > 200) {
    // send tilt signal
    m_bTilt = true;
    SendSignal( PBL_SIG_TILT, 0, this->getParent(), NULL ); 
    EM_COUT("EyeBehavior::onTick() TILT", 1);
    m_iTiltTick = 0;
  } else if (m_iTiltTick > 150) {
    // send warning signal
    SendSignal( PBL_SIG_TILT_WARNING, 0, this->getParent(), NULL ); 
    EM_COUT("EyeBehavior::onTick() WARNING", 1);
  }
  if (m_iTiltTick > 0) {
    m_iTiltTick--;
  }
	     
  // camera movement code ---------------------
  // current x, y, z - diff x, y, z
  float cx, cy, cz, dx, dy, dz, crx, cry, crz, drx, dry, drz;
  float bx=0, by=0, bz=0;
  // get median of all balls
  int balls = 0;
  if (table->isBallActive(0)) {
    float x, y, z;
    table->getBall(0)->getTranslation(x, y, z);
    bx = x; by = y; bz = z;
    balls++;
  }
  if (table->isBallActive(1)) {
    float x, y, z;
    table->getBall(1)->getTranslation(x, y, z);
    if (balls > 0) {
      bx += x; 
      by = EM_MAX(by, y); 
      bz = EM_MAX(bz, z);
    } else {
      bx = x; by = y; bz = z;
    }
    balls++;
  }
  if (table->isBallActive(2)) {
    float x, y, z;
    table->getBall(2)->getTranslation(x, y, z);
    if (balls > 0) {
      bx += x; 
      by = EM_MAX(by, y); 
      bz = EM_MAX(bz, z);
    } else {
      bx = x; by = y; bz = z;
    }
    balls++;
  }
  if (balls > 0) {
    bx /= balls;
  }
  this->getParent()->getTranslation(cx, cy, cz);
  this->getParent()->getRotation(crx, cry, crz);

  // the views
  switch (Config::getInstance()->getView()) {
  case 1: // soft pan & scan
    // get diff between position we want to move to and current pos
    dx = (TX1+bx*0.075+m_fXNudge) - cx;
    dy = (TY1+by*0.3) - cy;
    dz = (TZ1+bz*0.15+m_fZNudge) - cz;
    drx = (RX1-bz*0.00015f) - crx;
    dry = RY1 - cry;
    drz = RZ1 - crz;
    this->getParent()->addTranslation(dx*DIFF_FACTOR, dy*DIFF_FACTOR, dz*DIFF_FACTOR);
    this->getParent()->addRotation(drx*DIFF_FACTOR, dry*DIFF_FACTOR, drz*DIFF_FACTOR);
    break;
  case 2: // normal pan & scan
    // get diff between position we want to move to and current pos
    dx = (TX2+bx*0.08+m_fXNudge) - cx;
    dy = (TY2+by*0.3) - cy;
    dz = (TZ2+bz*0.3+m_fZNudge) - cz;
    drx = (RX2-bz*0.0003f) - crx;
    dry = RY2 - cry;
    drz = RZ2 - crz;
    this->getParent()->addTranslation(dx*DIFF_FACTOR, dy*DIFF_FACTOR, dz*DIFF_FACTOR);
    this->getParent()->addRotation(drx*DIFF_FACTOR, dry*DIFF_FACTOR, drz*DIFF_FACTOR);
    break;
  case 3: // top view
    // get diff between position we want to move to and current pos
    dx = (TX3+bx*0.15+m_fXNudge) - cx;
    dy = (TY3+by*0.4) - cy;
    dz = (TZ3+bz*0.6+m_fZNudge) - cz;
    this->getParent()->addTranslation(dx*DIFF_FACTOR, dy*DIFF_FACTOR, dz*DIFF_FACTOR);
    this->getParent()->setRotation(RX3, RY3, RZ3);
    break;
  case 4: // top-down - for virtual pinball tables
    // get diff between position we want to move to and current pos
    dx = (TX4+m_fXNudge) - cx; //Left to Right
    dy = (TY4) - cy; //Zoom
    dz = (TZ4+m_fZNudge) - cz;  //Top  to Bottom
    this->getParent()->addTranslation(dx*DIFF_FACTOR, dy*DIFF_FACTOR, dz*DIFF_FACTOR);
    this->getParent()->setRotation(RX4, RY4, RZ4);
    //this->getParent()->setScale(0.4f,1.0f,1.0f);
    break;
  case 5: // top-down-rotated - for virtual pinball tables
    // get diff between position we want to move to and current pos
    dx = (TX5+m_fXNudge) - cx; //Left to Right
    dy = (TY5) - cy; //Zoom
    dz = (TZ5+m_fZNudge) - cz;  //Top  to Bottom
    this->getParent()->addTranslation(dx*DIFF_FACTOR, dy*DIFF_FACTOR, dz*DIFF_FACTOR);
    this->getParent()->setRotation(RX5, RY5, RZ5);
    break;
  default:
  case 0: // classic view
    dx = (TX0+m_fXNudge) - cx;
    dy = (TY0) - cy;
    dz = (TZ0+m_fZNudge) - cz;
    this->getParent()->addTranslation(dx*DIFF_FACTOR, dy*DIFF_FACTOR, dz*DIFF_FACTOR);
    this->getParent()->setRotation(RX0, RY0, RZ0);
    break;
  }
}
//EOF: $Id: EyeBehavior.cpp,v 1.15 2003/06/13 13:39:46 rzr Exp $
