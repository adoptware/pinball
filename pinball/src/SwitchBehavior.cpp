/***************************************************************************
                          SwitchBehavior.cpp  -  description
                             -------------------
    begin                : Thu Feb 8 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#include "SwitchBehavior.h"
#include "Light.h"
#include "Pinball.h"

SwitchBehavior::SwitchBehavior(int aSignal, int uSignal, bool init) {
	m_iActiveSignal = aSignal;
	m_iUnActiveSignal = uSignal;
	m_bInit = init;
	p_Light = NULL;
}

SwitchBehavior::~SwitchBehavior(){
}

void SwitchBehavior::StdOnSignal() {
	OnSignal( PBL_SIG_RESET_ALL ) {
		if (m_bInit && p_Light != NULL) p_Light->setOn(true);
		else if (p_Light != NULL) p_Light->setOn(false);
	}
	OnSignal( m_iActiveSignal ) {
		if (p_Light != NULL) p_Light->setOn(true);
	}
	OnSignal( m_iUnActiveSignal ) {
		if (p_Light != NULL) p_Light->setOn(false);
	}
}
