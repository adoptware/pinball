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

// locked view
#define TX0 -1.75f
#define TY0 40.0f
#define TZ0 40.0f
#define RX0 0.15f
#define RY0 0.0f
#define RZ0 0.0f

// top view
#define TX1 -1.75f 
#define TY1 40.0f
#define TZ1 10.0f
#define RX1 0.23f
#define RY1 0.0f
#define RZ1 0.0f

// pan & scan view
#define TX2 -1.75f 
#define TY2 40.0f
#define TZ2 30.0f
#define RX2 0.17f
#define RY2 0.0f
#define RZ2 0.0f

EyeBehavior::EyeBehavior() : Behavior() {
  m_bTilt = false;
  m_iNudgeTick = 0;
  m_iNudgeType = 0;
  m_iTiltTick = 0;
  m_fXNudge = 0.0f;
  m_fZNudge = 0.0f;
  m_iSound = -1;
  this->setType(PBL_TYPE_EYEBEH);
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

  // the nudge code is here
  if (m_iNudgeTick < 1) {
    string bottomnudge("bottomnudge");
    string leftnudge("leftnudge");
    string rightnudge("rightnudge");
    if (Keyboard::isKeyDown(Config::getInstance()->getKey(bottomnudge)) && !m_bTilt) {
      cerr << "nudge" << endl;
      m_fZNudge = 1.0f;
      m_iTiltTick += 100;
      m_iNudgeTick = 50;
      m_iNudgeType = PBL_SIG_BNUDGE;
      SendSignal(PBL_SIG_BNUDGE, 0, this->getParent(), NULL ); 
      SoundUtil::getInstance()->playSample(m_iSound, false);
    } else if (Keyboard::isKeyDown(Config::getInstance()->getKey(leftnudge)) && !m_bTilt) {
      m_fXNudge = -1.0f;
      m_iTiltTick += 100;
      m_iNudgeTick = 50;
      m_iNudgeType = PBL_SIG_LNUDGE;
      SendSignal(PBL_SIG_LNUDGE, 0, this->getParent(), NULL ); 
      SoundUtil::getInstance()->playSample(m_iSound, false);
    } else if (Keyboard::isKeyDown(Config::getInstance()->getKey(rightnudge)) && !m_bTilt) {
      m_fXNudge = 1.0f;
      m_iTiltTick += 100;
      m_iNudgeTick = 50;
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

  if (m_iTiltTick > 210) {
    // send tilt signal
    SendSignal( PBL_SIG_TILT, 0, this->getParent(), NULL ); 
    EM_COUT("EyeBehavior::onTick() TILT", 1);
    m_iTiltTick = 0;
  } else if (m_iTiltTick > 110) {
    // send warning signal
    m_bTilt = true;
    SendSignal( PBL_SIG_TILT_WARNING, 0, this->getParent(), NULL ); 
    EM_COUT("EyeBehavior::onTick() WARNING", 1);
  }

  if (m_iTiltTick > 0) {
    m_iTiltTick--;
  }

  float ex, ey, ez, sx, sy, sz;
	
  if (Config::getInstance()->getView() == 2) {
    // locked view
    this->getParent()->getTranslation(ex, ey, ez);
	  
    this->getParent()->setTransform(TX0+m_fXNudge, TY0, TZ0+m_fZNudge, RX0, RY0, RZ0);
  } else if (Config::getInstance()->getView() == 1) {
    // top view
    float bx=0, by=0, bz=0;
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
		
    this->getParent()->getTranslation(ex, ey, ez);
		
    sx = (TX1+bx*0.4+m_fXNudge*2) - ex;
    sy = (TY1+by*0.4) - ey;
    sz = (TZ1+bz*0.4+m_fZNudge*2) - ez;
		
    this->getParent()->addTranslation(sx*0.1f, sy*0.1, sz*0.1f);
    this->getParent()->setRotation(RX1, RY1, RZ1);
  } else { 
    // pan & scan view
    float bx=0, by=0, bz=0;
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
		
    this->getParent()->getTranslation(ex, ey, ez);
		
    sx = (TX2+bx*0.4+m_fXNudge*2) - ex;
    sy = (TY2+by*0.4) - ey;
    sz = (TZ2+bz*0.1+m_fZNudge*2) - ez;
		
    this->getParent()->addTranslation(sx*0.1f, sy*0.1, sz*0.1f);
    this->getParent()->setRotation(RX2, RY2, RZ2);
  }
}
