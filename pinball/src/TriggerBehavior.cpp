/***************************************************************************
                          TriggerBehavior.cpp  -  description
                             -------------------
    begin                : Thu Jan 25 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#include "TriggerBehavior.h"
#include "Pinball.h"
#include "Group.h"
#include "Light.h"

TriggerBehavior::TriggerBehavior(int aSignal, int uSignal, int signal, bool init) {
	m_iActiveSignal = aSignal;
	m_iUnActiveSignal = uSignal;
	m_iSignal = signal;
	m_iTick = 3;
	m_bActive = init;
	m_bInit = init;
	p_Light = NULL;
}

TriggerBehavior::~TriggerBehavior(){
}

void TriggerBehavior::onTick() {
	m_iTick++;
	if (!m_bActive && p_Light != NULL) p_Light->setOn(false);
	else if (p_Light != NULL) p_Light->setOn(true);
}

void TriggerBehavior::StdOnSignal() {
	OnSignal( PBL_SIG_RESET_ALL) {
		m_bActive = m_bInit;
		SetLightOn();
	} else
	OnSignal( m_iActiveSignal) {
		m_bActive = true;
		SetLightOn();
	} else
	OnSignal( m_iUnActiveSignal) {
		m_bActive = false;
		SetLightOff();
	}
}

void TriggerBehavior::StdOnCollision() {
	OnCallerProperty( (PBL_BALL_1 | PBL_BALL_2 | PBL_BALL_3 | PBL_BALL_4) ) {
		if (!m_bActive) return;
		if (m_iTick < 3) {
			m_iTick = 0;
			return;
		}
		m_iTick = 0;
		SendSignal(m_iSignal, 0, this->p_Parent, NULL);
	}
}
