/***************************************************************************
                          BouBeh.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/


#include "CaveBehavior.h"
#include "Group.h"
#include "Pinball.h"
#include "Polygon.h"
#include "Shape3D.h"
#include "Score.h"

CaveBehavior::CaveBehavior() {
	m_iBall = PBL_NULL;
	m_iLockCounter = -1;
	m_bLock = false;
	p_Light = NULL;
	this->setType(PBL_TYPE_CAVEBEH);
}

CaveBehavior::~CaveBehavior() {
}

void CaveBehavior::onTick() {
	if (m_iLockCounter == 80) {
		SetLightOn(true);
	}
	if (m_iLockCounter == 20) {
		m_iBall = PBL_NULL;
		SetLightOn(false);
		UnsetProperty( PBL_TRAP );
		SetProperty( PBL_TRAP_BOUNCE );
	}
	if (m_iLockCounter == 0) {
		UnsetProperty( PBL_TRAP_BOUNCE );
		//		SendSignal( PBL_SIG_CAVE_OFF, 0, this->p_Parent, NULL );
		m_bLock = false;
		Score::getInstance()->playSample(1);
		Score::getInstance()->addScore(1200);
	}
	if (m_iLockCounter > -1) m_iLockCounter--;
}

void CaveBehavior::StdOnCollision() {
	OnCallerProperty( (PBL_BALL_1 | PBL_BALL_2 | PBL_BALL_3 | PBL_BALL_4) ) {
		if (m_bLock == true) return;
		m_bLock = true;
		m_iBall = GetCallerProperty() & (PBL_BALL_1 | PBL_BALL_2 | PBL_BALL_3 | PBL_BALL_4);
		m_iLockCounter = 80;
		SetProperty( PBL_TRAP );
		//SendSignal( PBL_SIG_CAVE_ON, 0 , this->p_Parent, NULL );
	}
}
