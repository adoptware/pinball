/***************************************************************************
                          BouBeh.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/


#include "BumperBehavior.h"
#include "Group.h"
#include "Pinball.h"
#include "Polygon.h"
#include "Shape3D.h"


BumperBehavior::BumperBehavior() {
	m_iLightCounter = -1;
	p_Light = NULL;
}

BumperBehavior::~BumperBehavior() {
}

void BumperBehavior::onTick() {
	if (m_iLightCounter > -1) m_iLightCounter--;
	if (m_iLightCounter == 0 ) SetLightOff();
}

void BumperBehavior::StdOnCollision() {
	OnCallerProperty( PBL_BALL_1 OR_CP PBL_BALL_2 OR_CP PBL_BALL_3 OR_CP PBL_BALL_4 ) {
		m_iLightCounter = 10;
		SetLightOn();
		SendSignal( PBL_SIG_BUMPER, 0, this->p_Parent, NULL );
	}
}
