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
#include "Score.h"

LockBehavior::LockBehavior(int as, int us, int ls, bool lock) : Behavior() {
	m_iActiveSignal = as;
  m_iUnActiveSignal = us;
  m_iLockSignal = ls;
	m_iBall = PBL_NULL;
	m_bActive = lock;
	this->setType(PBL_TYPE_LOCKBEH);
}

LockBehavior::~LockBehavior() {
}

void LockBehavior::StdOnSignal() {
	OnSignal( PBL_SIG_RESET_ALL) {
		m_bActive = false;
		m_iBall = PBL_NULL;
		UnsetProperty( PBL_TRAP );
		SetLightOn(false);
	} else
	OnSignal( m_iUnActiveSignal ) {
		m_bActive = false;
		m_iBall = PBL_NULL;
		UnsetProperty( PBL_TRAP );
		SetLightOn(false);
	} else
	OnSignal( m_iActiveSignal ) {
		m_bActive = true;
		m_iBall = PBL_NULL;
		SetProperty( PBL_TRAP );
		SetLightOn(true);
	}
}

void LockBehavior::StdOnCollision() {
	if (m_iBall != PBL_NULL) return;              
	if (!m_bActive) return;           

#define CALLER_BALL(ball)             \
	OnCallerProperty( ball ) {          \
		m_iBall = ball;                   \
    /*Score::getInstance()->lockBall( ball );*/           \
		SetProperty( PBL_TRAP );                              \
		SendSignal( m_iLockSignal, 0, this->p_Parent, NULL ); \
		SetLightOn(false);                                    \
	}	

  CALLER_BALL( PBL_BALL_1 )
		else
  CALLER_BALL( PBL_BALL_2 )
		else
  CALLER_BALL( PBL_BALL_3 )
		else
  CALLER_BALL( PBL_BALL_4 )

#undef CALLER_BALL
}
