/***************************************************************************
                          BouBeh.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/


#include "LockBehavior.h"
#include "Group.h"
#include "Pinball.h"
#include "Polygon.h"
#include "Shape3D.h"
#include "Light.h"

LockBehavior::LockBehavior(int as, int us, int ls, bool lock) {
	m_iActiveSignal = as;
  m_iUnActiveSignal = us;
  m_iLockSignal = ls;
	m_bLock = false;
	m_bActive = lock;
}

LockBehavior::~LockBehavior() {
}

void LockBehavior::StdOnSignal() {
	OnSignal( PBL_SIG_RESET_ALL) {
		m_bActive = false;
		m_bLock = false;
		UnsetProperty( PBL_TRAP );
		SetLightOff();
	} else
	OnSignal( m_iUnActiveSignal ) {
		m_bActive = false;
		m_bLock = false;
		UnsetProperty( PBL_TRAP );
		SetLightOff();
	} else
	OnSignal( m_iActiveSignal ) {
		m_bActive = true;
		SetProperty( PBL_TRAP );
		SetLightOn();
	}
}

void LockBehavior::StdOnCollision() {
	OnCallerProperty( (PBL_BALL_1 | PBL_BALL_2 | PBL_BALL_3 | PBL_BALL_4) ) {
		if (m_bLock) return;
		if (!m_bActive) return;
		m_bLock = true;
		SetProperty( PBL_TRAP );
		SendSignal( m_iLockSignal, 0, this->p_Parent, NULL );
		SetLightOff();
	}
}
