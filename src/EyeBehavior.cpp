/***************************************************************************
                          EyeBehavior.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "EyeBehavior.h"
#include "Group.h"
#include "Pinball.h"
#include "Config.h"
#include "Score.h"
#include "Keyboard.h"
#include "SoundUtil.h"

#define TX1 -1.5f 
#define TY1 30
#define TZ1 6
#define RX1 0.23f
#define RY1 0
#define RZ1 0

#define TX2 -1.5f 
#define TY2 35
#define TZ2 10
#define RX2 0.22f
#define RY2 0
#define RZ2 0

EyeBehavior::EyeBehavior(Group * g1, Group * g2, Group * g3) : Behavior() {
	m_iNudgeTick = 0;
	m_iNudgeType = 0;
	m_iTiltTick = 0;
	m_fXNudge = 0.0f;
	m_fZNudge = 0.0f;
	m_iSound = -1;
	p_gBall1 = g1;
	p_gBall2 = g2;
	p_gBall3 = g3;
	p_Score = Score::getInstance();
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
	}
}

void EyeBehavior::onTick() {
	EmAssert(p_gBall1 != NULL, "Ball1 group NULL");
	EmAssert(p_gBall2 != NULL, "Ball2 group NULL");
	EmAssert(p_gBall3 != NULL, "Ball3 group NULL");
	EmAssert(this->getParent() != NULL, "Parent group NULL");

	// the nudge code is here
	if (m_iNudgeTick < 1) {
		if (Keyboard::isKeyDown(SDLK_SPACE)) {
			cerr << "nudge" << endl;
			m_fZNudge = 1.0f;
			m_iTiltTick += 100;
			m_iNudgeTick = 50;
			m_iNudgeType = PBL_SIG_BNUDGE;
			SendSignal(PBL_SIG_BNUDGE, 0, this->getParent(), NULL ); 
			SoundUtil::getInstance()->playSample(m_iSound, false);
		} else if (Keyboard::isKeyDown(SDLK_LCTRL)) {
			m_fXNudge = -1.0f;
			m_iTiltTick += 100;
			m_iNudgeTick = 50;
			m_iNudgeType = PBL_SIG_LNUDGE;
			SendSignal(PBL_SIG_LNUDGE, 0, this->getParent(), NULL ); 
			SoundUtil::getInstance()->playSample(m_iSound, false);
		} else if (Keyboard::isKeyDown(SDLK_RCTRL)) {
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

	if (m_iTiltTick > 110) {
		// send tilt signal
		SendSignal( PBL_SIG_TILT, 0, this->getParent(), NULL ); 
		cerr << "TILT" << endl;
		m_iTiltTick = 0;
	}
	if (m_iTiltTick > 0) {
		m_iTiltTick--;
	}

	float ex, ey, ez, sx, sy, sz;
	
	if (Config::getInstance()->getView() == 2) {
		// classic view
		this->getParent()->getTranslation(ex, ey, ez);
		
// 		sx = (TX+m_fXNudge*2) - ex;
// 		sy = (TY) - ey;
// 		sz = (TZ+m_fZNudge*2) - ez;
		
// 		this->getParent()->addTranslation(sx*0.1f, sy*0.1, sz*0.1f);
// 		this->getParent()->setRotation(RX, RY, RZ);

		this->getParent()->setTransform(0+m_fXNudge, 35, 12+m_fZNudge, 0.21f, 0, 0);
	} else if (Config::getInstance()->getView() == 1) {
		// dynamic view
		float bx=0, by=0, bz=0;
		int balls = 0;
		
		if (p_Score->isBallActive(PBL_BALL_1)) {
			float x, y, z;
			p_gBall1->getTranslation(x, y, z);
			bx = x; by = y; bz = z;
			balls++;
		}
		if (p_Score->isBallActive(PBL_BALL_2)) {
			float x, y, z;
			p_gBall2->getTranslation(x, y, z);
			if (balls > 0) {
				bx += x; 
				by = EM_MAX(by, y); 
				bz = EM_MAX(bz, z);
			} else {
				bx = x; by = y; bz = z;
			}
			balls++;
		}
		if (p_Score->isBallActive(PBL_BALL_3)) {
			float x, y, z;
			p_gBall3->getTranslation(x, y, z);
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
		// top view
		float bx=0, by=0, bz=0;
		int balls = 0;
		
		if (p_Score->isBallActive(PBL_BALL_1)) {
			float x, y, z;
			p_gBall1->getTranslation(x, y, z);
			bx = x; by = y; bz = z;
			balls++;
		}
		if (p_Score->isBallActive(PBL_BALL_2)) {
			float x, y, z;
			p_gBall2->getTranslation(x, y, z);
			if (balls > 0) {
				bx += x; 
				by = EM_MAX(by, y); 
				bz = EM_MAX(bz, z);
			} else {
				bx = x; by = y; bz = z;
			}
			balls++;
		}
		if (p_Score->isBallActive(PBL_BALL_3)) {
			float x, y, z;
			p_gBall3->getTranslation(x, y, z);
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
		sy = (TY2+by*0.1) - ey;
		sz = (TZ2+bz*0.4+m_fZNudge*2) - ez;
		
		this->getParent()->addTranslation(sx*0.1f, sy*0.1, sz*0.1f);
		this->getParent()->setRotation(RX2, RY2, RZ2);
	}
}
