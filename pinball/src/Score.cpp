/***************************************************************************
                          Score.cpp  -  description
                             -------------------
    begin                : Fri Jan 26 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Score.h"
#include "Group.h"
#include "Pinball.h"
#include "Keyboard.h"
#include "SoundUtil.h"
#include "TextureUtil.h"
#include "EmFont.h"

extern int em_width_;
extern int em_height_;

Score::Score(){
	m_iActiveBalls = 0;
	m_iBallsLeft = 3;
	m_iScore = 0;
	m_iBumps = 0;
	m_bExtraBall = false;
	for (int a=0; a<8; a++) {
		m_aMission[a] = 0;
	}
	m_aTargets[0] = false;
	m_aTargets[1] = false;
	m_baAliveBalls[0] = false;
	m_baAliveBalls[1] = false;
	m_baAliveBalls[2] = false;
	m_baAliveBalls[3] = false;

	m_aSample[0] = SoundUtil::loadSample("data/boing.wav");
	m_aSample[1] = SoundUtil::loadSample("data/fjong.wav");
	m_aSample[2] = SoundUtil::loadSample("data/lock.wav");
	m_aSample[3] = SoundUtil::loadSample("data/balldead.wav");
	m_aSample[4] = SoundUtil::loadSample("data/crash.wav");
	m_aSample[5] = SoundUtil::loadSample("data/caveout.wav");
	m_aSample[6] = SoundUtil::loadSample("data/activatelock.wav");

	m_Font = EmFont::getInstance();
	m_Font->loadFont("data/font_16.pcx");
}

Score::~Score(){
}

void Score::onTick() {
	if (m_iActiveBalls == 0 && m_iBallsLeft > 0 && Keyboard::isKeyDown(KEY_ENTER)) {
		SoundUtil::play(m_aSample[1], false);
		m_iActiveBalls = 1;
		if (!m_baAliveBalls[0]) {
			SendSignal( PBL_SIG_ACTIVATE_BALL_1 );
			m_baAliveBalls[0] = true;
		}	
		else if (!m_baAliveBalls[1]) {
			SendSignal( PBL_SIG_ACTIVATE_BALL_2 );
			m_baAliveBalls[1] = true;
		}
		else	if (!m_baAliveBalls[2]) {
			SendSignal( PBL_SIG_ACTIVATE_BALL_3 );
			m_baAliveBalls[2] = true;
		}
		else if (!m_baAliveBalls[3]) {
			SendSignal( PBL_SIG_ACTIVATE_BALL_4 );
			m_baAliveBalls[3] = true;
		}
		EM_COUT("Score::onTick() new ball", 1);
	}
}

void Score::StdOnSignal() {
	OnSignal( PBL_SIG_RESET_ALL ) {
		this->empty();
	} else
  OnSignal( PBL_SIG_BALL_1_DEAD) {
		m_iActiveBalls--;
		m_baAliveBalls[0] = false;
	}	else
  OnSignal( PBL_SIG_BALL_2_DEAD) {
		m_iActiveBalls--;
		m_baAliveBalls[1] = false;
	} else
  OnSignal( PBL_SIG_BALL_3_DEAD) {
		m_iActiveBalls--;
		m_baAliveBalls[2] = false;
	}	else
	OnSignal( PBL_SIG_BALL_4_DEAD) {
		m_iActiveBalls--;
		m_baAliveBalls[3] = false;
	}	

	OnSignal( PBL_SIG_BALL_1_DEAD OR_SI PBL_SIG_BALL_2_DEAD OR_SI	
			PBL_SIG_BALL_3_DEAD OR_SI PBL_SIG_BALL_4_DEAD ) {
		if (m_iActiveBalls == 1) {
			SendSignal( PBL_SIG_MULTIBALL_DEAD );
			SendSignal( PBL_SIG_NEW_LOCK );
		}
		if (m_iActiveBalls == 0) {
			if (!m_bExtraBall) {
				m_iBallsLeft--;
				SoundUtil::play(m_aSample[3], false);
			} else {
				m_bExtraBall = false;
			}
		}
	}	

	OnSignal( PBL_SIG_JACKPOT ) {
		m_iScore += 100000;
		SoundUtil::play(m_aSample[4], false);
	}	else
	OnSignal( PBL_SIG_BUMPER ) {
		m_iScore += 450;
		m_iBumps++;
		SoundUtil::play(m_aSample[0], false);
	}	else
	OnSignal( PBL_SIG_LOCK_1 OR_SI PBL_SIG_LOCK_2 ) {
		m_iScore += 7500;
		m_iActiveBalls--;
		SoundUtil::play(m_aSample[2], false);
	}	else
	OnSignal( PBL_SIG_CAVE ) {
		m_iScore += 10000;
		if (m_aMission[1] == 1) {
			m_aMission[1] = -1;
			m_iScore += 50000;
			SoundUtil::play(m_aSample[4], false);
		}
	}	else
	OnSignal( PBL_SIG_CAVE_OUT ) {
		SoundUtil::play(m_aSample[5], false);
	}	else
	OnSignal( PBL_SIG_LEFT_LOOP ) {
		if (m_aMission[0] > 0) {
			m_aMission[0] = m_aMission[0] | 2;
		}
		SoundUtil::play(m_aSample[1], false);
	}	else
	OnSignal( PBL_SIG_RIGHT_LOOP	) {
		if (m_aMission[0] > 0) {
			m_aMission[0] = m_aMission[0] | 4;
		}
		SoundUtil::play(m_aSample[1], false);
	}	else
	OnSignal( PBL_SIG_RELEASE_LOCK ) {
		m_iScore += 8000;
		m_iActiveBalls += 2;
		SoundUtil::play(m_aSample[4], false);
	}	else
	OnSignal( PBL_SIG_ACTIVATE_LOCK ) {
		m_iScore += 4000;
		SoundUtil::play(m_aSample[6], false);
	}	else
	OnSignal( PBL_SIG_TARGET_1 ) {
		m_aTargets[0] = true;
		SoundUtil::play(m_aSample[6], false);
		if (m_aTargets[1]) {
			SendSignal( PBL_SIG_ACTIVATE_EXTRABALL );
		}
	}	else
	OnSignal( PBL_SIG_TARGET_2 ) {
		m_aTargets[1] = true;
		SoundUtil::play(m_aSample[6], false);
		if (m_aTargets[0]) {
			SendSignal( PBL_SIG_ACTIVATE_EXTRABALL );
		}
	}	else
	OnSignal( PBL_SIG_EXTRABALL ) {
		m_bExtraBall = true;
		SendSignal( PBL_SIG_NEW_TARGET );
		m_aTargets[0] = false;
		m_aTargets[1] = false;
	}
		
	if (m_aMission[0] == 7) {
		m_aMission[0] = -1;
		m_iScore += 50000;
		SoundUtil::play(m_aSample[4], false);
	}

	if (m_iBumps == 5) {
		m_aMission[0] = 1;
		SendSignal( PBL_SIG_ACTIVATE_LEFT_LOOP );
		SendSignal( PBL_SIG_ACTIVATE_RIGHT_LOOP );
		SendSignal( PBL_SIG_MISSION_1 );
	}

	if (m_iBumps == 10 ) {
		m_aMission[1] = 1;
		SendSignal( PBL_SIG_MISSION_2 );
	}
}

void Score::draw() {
	char buffer[256];
	sprintf(buffer, "Score %d\n", m_iScore);
	m_Font->print(buffer, 10, em_height_ - 20);
}

void Score::empty() {
	m_iActiveBalls = 0;
	m_iBallsLeft = 3;
	m_iScore = 0;
	m_iBumps = 0;
	m_bExtraBall = false;
	m_aMission[0] = 0;
	m_aMission[1] = 0;
	m_aMission[2] = 0;
	m_aTargets[0] = false;
	m_aTargets[1] = false;
	m_baAliveBalls[0] = false;
	m_baAliveBalls[1] = false;
	m_baAliveBalls[2] = false;
	m_baAliveBalls[3] = false;
}

