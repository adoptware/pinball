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


CaveBehavior::CaveBehavior() {
	m_iLockCounter = -1;
	m_bLock = false;
	p_Light = NULL;
}

CaveBehavior::~CaveBehavior() {
}

void CaveBehavior::onTick() {
	if (m_iLockCounter == 80) {
		SetLightOn();
	}
	if (m_iLockCounter == 20) {
		SetLightOff();
		UnsetProperty( PBL_TRAP );
		SetProperty( PBL_TRAP_BOUNCE );
	}
	if (m_iLockCounter == 0) {
		UnsetProperty( PBL_TRAP_BOUNCE );
		SendSignal( PBL_SIG_CAVE_OUT, 0, this->p_Parent, NULL );
		m_bLock = false;
	}
	if (m_iLockCounter > -1) m_iLockCounter--;
}

void CaveBehavior::StdOnCollision() {
	if (m_bLock == true) return;
	m_bLock = true;
	m_iLockCounter = 80;
	SetProperty( PBL_TRAP );
	SendSignal( PBL_SIG_CAVE, 0 , this->p_Parent, NULL );
}
