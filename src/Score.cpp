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
#include "Config.h"

extern int em_width_;
extern int em_height_;

Score::Score() {
	char filename[256];
	sprintf(filename, "%s/bump.wav", Config::getInstance()->getDataDir());
	m_aSample[0] = SoundUtil::loadSample(filename);
	sprintf(filename, "%s/fjong.wav", Config::getInstance()->getDataDir());
	m_aSample[1] = SoundUtil::loadSample(filename);
	sprintf(filename, "%s/lock.wav", Config::getInstance()->getDataDir());
	m_aSample[2] = SoundUtil::loadSample(filename);
	sprintf(filename, "%s/fart.wav", Config::getInstance()->getDataDir());
	m_aSample[3] = SoundUtil::loadSample(filename);
	sprintf(filename, "%s/flush.wav", Config::getInstance()->getDataDir());
	m_aSample[4] = SoundUtil::loadSample(filename);

	m_Font = EmFont::getInstance();
	sprintf(filename, "%s/font_16.png", Config::getInstance()->getDataDir());
	m_Font->loadFont(filename);

	this->clear();
}

Score::~Score(){
}

void Score::onTick() {
	if (m_iActiveBalls == 0 && m_iBallsLeft > 0 && Keyboard::isKeyDown(SDLK_RETURN)) {
		SoundUtil::play(m_aSample[1], false);
		m_iActiveBalls = 1;
		if (!m_baAliveBall[0]) {
			SendSignal( PBL_SIG_BALL1_ON, 0, this->p_Parent, NULL );
			m_baAliveBall[0] = true;
			m_Text1="";
			m_Text2="";
		}	
		else if (!m_baAliveBall[1]) {
			SendSignal( PBL_SIG_BALL2_ON, 0, this->p_Parent, NULL );
			m_baAliveBall[1] = true;
			m_Text1="";
			m_Text2="";
		}
		else	if (!m_baAliveBall[2]) {
			SendSignal( PBL_SIG_BALL3_ON, 0, this->p_Parent, NULL );
			m_baAliveBall[2] = true;
			m_Text1="";
			m_Text2="";
		}
		else if (!m_baAliveBall[3]) {
			SendSignal( PBL_SIG_BALL4_ON, 0, this->p_Parent, NULL );
			m_baAliveBall[3] = true;
			m_Text1="";
			m_Text2="";
		}
		EM_COUT("Score::onTick() new ball", 1);
	}
	if (m_iBallsLeft == 0) {
		m_Text1 = "press r to start new game";
	}
}

void Score::StdOnSignal() {
	OnSignal( PBL_SIG_RESET_ALL ) {
		this->clear();
	} else
  OnSignal( PBL_SIG_BALL1_OFF) {
		m_iActiveBalls--;
		m_baAliveBall[0] = false;
	}	else
  OnSignal( PBL_SIG_BALL2_OFF) {
		m_iActiveBalls--;
		m_baAliveBall[1] = false;
	} else
  OnSignal( PBL_SIG_BALL3_OFF) {
		m_iActiveBalls--;
		m_baAliveBall[2] = false;
	}	else
	OnSignal( PBL_SIG_BALL4_OFF) {
		m_iActiveBalls--;
		m_baAliveBall[3] = false;
	}	

	OnSignal( PBL_SIG_BALL1_OFF OR_SI 
						PBL_SIG_BALL2_OFF OR_SI	
						PBL_SIG_BALL3_OFF OR_SI 
						PBL_SIG_BALL4_OFF ) {
		if (m_iActiveBalls == 1) {
			//SendSignal( PBL_SIG_MULTIBALL_OFF, 0, this->p_Parent, NULL );
			//SendSignal( PBL_SIG_LOCK_ON, 0, this->p_Parent, NULL );
		}
		if (m_iActiveBalls == 0) {
			if (!m_bExtraBall) {
				m_iBallsLeft--;
			} else {
				m_bExtraBall = false;
			}
				if (m_iBallsLeft == 0) {
					SoundUtil::play(m_aSample[4], false);
				} else {
					SoundUtil::play(m_aSample[3], false);
				}
		}
	}	

// 	OnSignal( PBL_SIG_JACKPOT ) {
// 		m_iScore += 100000;
// 		SoundUtil::play(m_aSample[4], false);
// 	}	else
	OnSignal( PBL_SIG_BUMPER_ON ) {
		m_iScore += 450;
		m_iBumps++;
		SoundUtil::play(m_aSample[0], false);
// 	}	else 
// 		OnSignal( PBL_SIG_LOCK_1 OR_SI PBL_SIG_LOCK_2 ) {
// 		m_iScore += 7500;
// 		m_iActiveBalls--;
// 		SoundUtil::play(m_aSample[2], false);
	}	else
	OnSignal( PBL_SIG_CAVE_ON ) {
		m_iScore += 10000;
		if (m_aMission[1] == 1) {
			m_aMission[1] = -1;
			m_iScore += 50000;
			SoundUtil::play(m_aSample[1], false);
		}
	}	else
	OnSignal( PBL_SIG_CAVE_OFF ) {
		SoundUtil::play(m_aSample[2], false);
	}
// 	}	else
// 	OnSignal( PBL_SIG_LEFT_LOOP ) {
// 		if (m_aMission[0] > 0) {
// 			m_aMission[0] = m_aMission[0] | 2;
// 		}
// 		SoundUtil::play(m_aSample[1], false);
// 	}	else
// 	OnSignal( PBL_SIG_RIGHT_LOOP	) {
// 		if (m_aMission[0] > 0) {
// 			m_aMission[0] = m_aMission[0] | 4;
// 		}
// 		SoundUtil::play(m_aSample[1], false);
// 	}	else
// 	OnSignal( PBL_SIG_RELEASE_LOCK ) {
// 		m_iScore += 8000;
// 		m_iActiveBalls += 2;
// 		SoundUtil::play(m_aSample[4], false);
// 	}	else
// 	OnSignal( PBL_SIG_ACTIVATE_LOCK ) {
// 		m_iScore += 4000;
// 		SoundUtil::play(m_aSample[6], false);
// 	}	else
// 	OnSignal( PBL_SIG_TARGET_1 ) {
// 		m_aTarget[0] = true;
// 		SoundUtil::play(m_aSample[6], false);
// 		if (m_aTarget[1]) {
// 			SendSignal( PBL_SIG_ACTIVATE_EXTRABALL, 0, this->p_Parent, NULL );
// 		}
// 	}	else
// 	OnSignal( PBL_SIG_TARGET_2 ) {
// 		m_aTarget[1] = true;
// 		SoundUtil::play(m_aSample[6], false);
// 		if (m_aTarget[0]) {
// 			SendSignal( PBL_SIG_ACTIVATE_EXTRABALL, 0, this->p_Parent, NULL );
// 		}
// 	}	else
// 	OnSignal( PBL_SIG_EXTRABALL ) {
// 		m_bExtraBall = true;
// 		SendSignal( PBL_SIG_NEW_TARGET, 0, this->p_Parent, NULL );
// 		m_aTarget[0] = false;
// 		m_aTarget[1] = false;
// 	}
		
// 	if (m_aMission[0] == 7) {
// 		m_aMission[0] = -1;
// 		m_iScore += 50000;
// 		SoundUtil::play(m_aSample[4], false);
// 	}

// 	if (m_iBumps == 5) {
// 		m_aMission[0] = 1;
// 		SendSignal( PBL_SIG_ACTIVATE_LEFT_LOOP, 0, this->p_Parent, NULL );
// 		SendSignal( PBL_SIG_ACTIVATE_RIGHT_LOOP, 0, this->p_Parent, NULL );
// 		SendSignal( PBL_SIG_MISSION_1, 0, this->p_Parent, NULL );
// 	}

// 	if (m_iBumps == 10 ) {
// 		m_aMission[1] = 1;
// 		SendSignal( PBL_SIG_MISSION_2, 0, this->p_Parent, NULL );
// 	}
}

extern int em_width_div2_;
extern int em_height_div2_;

void Score::draw() {
	char buffer[256];
	sprintf(buffer, "Score %d\n", m_iScore);
	m_Font->print(buffer, 10, em_height_ - 20);
	m_Font->print(m_Text1, 
								em_width_div2_ - strlen(m_Text1)*EmFont::getInstance()->getSize()/2, 
								em_height_div2_ + EmFont::getInstance()->getSize()/2+2);
	m_Font->print(m_Text2, 
								em_width_div2_ - strlen(m_Text2)*EmFont::getInstance()->getSize()/2, 
								em_height_div2_ - EmFont::getInstance()->getSize()/2-2);
}

void Score::clear() {
	m_Text1="press enter to launch ball";
	m_Text2="left and right shift activates flippers";
	m_iActiveBalls = 0;
	m_iBallsLeft = 3;
	m_iScore = 0;
	m_iBumps = 0;
	m_bExtraBall = false;
	for (int a=0; a<8; a++) {
		m_aMission[a] = 0;
	}
	m_aTarget[0] = false;
	m_aTarget[1] = false;
	m_aLinux[0] = false;
	m_aLinux[1] = false;
	m_aLinux[2] = false;
	m_aLinux[3] = false;
	m_aLinux[4] = false;
	m_baAliveBall[0] = false;
	m_baAliveBall[1] = false;
	m_baAliveBall[2] = false;
	m_baAliveBall[3] = false;
}

