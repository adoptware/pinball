/***************************************************************************
                          Score.cpp  -  description
                             -------------------
    begin                : Fri Jan 26 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <cstdio>
#include <iostream>
#include <cstring>
#include "Private.h"
#include "Score.h"
#include "Group.h"
#include "Pinball.h"
#include "Keyboard.h"
//#include "SoundUtil.h"
//#include "TextureUtil.h"
#include "EmFont.h"
#include "Config.h"

Score* Score::p_Score = NULL;

Score::Score() {
	m_Font = EmFont::getInstance();
	this->clear();
}

Score::~Score(){
	p_Score = NULL;
}

Score* Score::getInstance() {
 	if (p_Score == NULL) {
 		p_Score = new Score();
 	}
 	return p_Score;
}

void Score::addScore(int s, bool multi) {
	if (multi) {
		m_iScore += (s*m_iFactor);
	} else {
		m_iScore += s;
	}
}

void Score::setText1(const char * text) {
	strncpy(m_Text1, text, 63);
}

void Score::setText2(const char * text) {
	strncpy(m_Text2, text, 63);
}

void Score::setText3(const char * text) {
	strncpy(m_Text3, text, 63);
}

void Score::setText4(const char * text) {
	strncpy(m_Text4, text, 63);
}

void Score::clearText() {
	strcpy(m_Text1, "");
	strcpy(m_Text2, "");
	strcpy(m_Text3, "");
	strcpy(m_Text4, "");
}

void Score::activateBall(int ball) {
	switch (ball) {
	case PBL_BALL_1 : m_aBall[0] = PBL_ACTIVE; break;	
	case PBL_BALL_2 : m_aBall[1] = PBL_ACTIVE; break;
	case PBL_BALL_3 : m_aBall[2] = PBL_ACTIVE; break;
	case PBL_BALL_4 : m_aBall[3] = PBL_ACTIVE; break;
	}
}

void Score::unActivateBall(int ball) {
	switch (ball) {
	case PBL_BALL_1 : m_aBall[0] = PBL_DEAD; break;	
	case PBL_BALL_2 : m_aBall[1] = PBL_DEAD; break;
	case PBL_BALL_3 : m_aBall[2] = PBL_DEAD; break;
	case PBL_BALL_4 : m_aBall[3] = PBL_DEAD; break;
	}
}

bool Score::isBallActive(int ball) {
	switch (ball) {
 	case PBL_BALL_1: return m_aBall[0] == PBL_ACTIVE;
 	case PBL_BALL_2: return m_aBall[1] == PBL_ACTIVE;
 	case PBL_BALL_3: return m_aBall[2] == PBL_ACTIVE;
 	case PBL_BALL_4: return m_aBall[3] == PBL_ACTIVE;
 	}
	return false;
}

bool Score::isBallDead(int ball) {
	switch (ball) {
 	case PBL_BALL_1: return m_aBall[0] == PBL_DEAD;
 	case PBL_BALL_2: return m_aBall[1] == PBL_DEAD;
 	case PBL_BALL_3: return m_aBall[2] == PBL_DEAD;
 	case PBL_BALL_4: return m_aBall[3] == PBL_DEAD;
 	}
	return false;
}

/** The bounce behavior calls this method when the ball is locked */
void Score::lockBall(int ball) {
 	switch (ball) {
 	case PBL_BALL_1: m_aBall[0] = PBL_LOCKED; break;
 	case PBL_BALL_2: m_aBall[1] = PBL_LOCKED; break; 
 	case PBL_BALL_3: m_aBall[2] = PBL_LOCKED; break;
 	case PBL_BALL_4: m_aBall[2] = PBL_LOCKED; break;
 	}
}

void Score::unLockBall(int ball) {
	switch (ball) {
	case PBL_BALL_1 : if (m_aBall[0] == PBL_LOCKED) m_aBall[0] = PBL_ACTIVE; break;	
	case PBL_BALL_2 : if (m_aBall[1] == PBL_LOCKED) m_aBall[1] = PBL_ACTIVE; break;
	case PBL_BALL_3 : if (m_aBall[2] == PBL_LOCKED) m_aBall[2] = PBL_ACTIVE; break;
	case PBL_BALL_4 : if (m_aBall[3] == PBL_LOCKED) m_aBall[2] = PBL_ACTIVE; break;
	}
}

bool Score::isBallLocked(int ball) {
	switch (ball) {
	case PBL_BALL_1 : return (m_aBall[0] == PBL_LOCKED);	
	case PBL_BALL_2 : return (m_aBall[1] == PBL_LOCKED);
	case PBL_BALL_3 : return (m_aBall[2] == PBL_LOCKED);
	case PBL_BALL_4 : return (m_aBall[3] == PBL_LOCKED);
	}
	return false;
}

int Score::active() {
 	int active = 0;
 	if (m_aBall[0] == PBL_ACTIVE) ++active;
	if (m_aBall[1] == PBL_ACTIVE)	++active;
 	if (m_aBall[2] == PBL_ACTIVE) ++active;
 	if (m_aBall[3] == PBL_ACTIVE) ++active;
 	return active;
}

int Score::locked() {
	int locked = 0;
	if (m_aBall[0] == PBL_LOCKED) ++locked;
	if (m_aBall[1] == PBL_LOCKED) ++locked;
	if (m_aBall[2] == PBL_LOCKED) ++locked;
	if (m_aBall[3] == PBL_LOCKED) ++locked;
	return locked;
}


void Score::onTick() {
	if (this->active() == 0 && this->getCurrentBall() < 4) {
		strcpy(m_Text1, "   press enter to launch ball");
	} else if (this->getCurrentBall() > 3) {
		strcpy(m_Text1, "   press r to start new game");
	} else {
		strcpy(m_Text1, "");
	}

// 	if (m_bLaunch && m_iBall < 4 && Keyboard::isKeyDown(SDLK_RETURN)) {
// 		m_bLaunch = false;
// 		switch (m_iBall) {
// 		case 1 :
// 			if (m_aAliveBall[0] == PBL_DEAD) {
// 				SendSignal( PBL_SIG_BALL1_ON, 0, this->getParent(), NULL );
// 				m_aAliveBall[0] = PBL_ALIVE;	
// 				m_Text1 = "";	
// 				m_Text2 = "";
// 				m_Text3 = "";
// 				m_Text4 = "";
// 				break;
// 			}	
// 		case 2 :
// 			if (m_aAliveBall[1] == PBL_DEAD) {
// 				SendSignal( PBL_SIG_BALL2_ON, 0, this->getParent(), NULL );
// 				m_aAliveBall[1] = PBL_ALIVE;	
// 				m_Text1 = "";	
// 				m_Text2 = "";
// 				m_Text3 = "";
// 				m_Text4 = "";
// 				break;
// 			}
// 		case 3 :
// 			if (m_aAliveBall[2] == PBL_DEAD) {
// 				SendSignal( PBL_SIG_BALL3_ON, 0, this->getParent(), NULL );
// 				m_aAliveBall[2] = PBL_ALIVE;
// 				m_Text1 = "";
// 				m_Text2 = "";
// 				m_Text3 = "";
// 				m_Text4 = "";
// 				break;
// 			}
// 		default:
// 			if (m_aAliveBall[0] == PBL_DEAD) {
// 				SendSignal( PBL_SIG_BALL1_ON, 0, this->getParent(), NULL );
// 				m_aAliveBall[0] = PBL_ALIVE;
// 				m_Text1 = "";
// 				m_Text2 = "";
// 				m_Text3 = "";
// 				m_Text4 = "";
// 				break;
// 			}	
// 			if (m_aAliveBall[1] == PBL_DEAD) {
// 				SendSignal( PBL_SIG_BALL2_ON, 0, this->getParent(), NULL );
// 				m_aAliveBall[1] = PBL_ALIVE;
// 				m_Text1 = "";
// 				m_Text2 = "";
// 				m_Text3 = "";
// 				m_Text4 = "";
// 				break;
// 			}
// 			if (m_aAliveBall[2] == PBL_DEAD) {
// 				SendSignal( PBL_SIG_BALL3_ON, 0, this->getParent(), NULL );
// 				m_aAliveBall[2] = PBL_ALIVE;
// 				m_Text1 = "";
// 				m_Text2 = "";
// 				m_Text3 = "";
// 				m_Text4 = "";
// 				break;
// 			}
// 			throw string("All balls busy");
// 		}
// 		EM_COUT("Score::onTick() new ball", 1);
// 	}
}

void Score::StdOnSignal() {
	EM_COUT((int)em_signal, 1);

	OnSignal( PBL_SIG_RESET_ALL ) {
		this->clear();
	} 
	//else
// 	OnSignal( PBL_SIG_BALL1_OFF) {
// 		m_aAliveBall[0] = PBL_DEAD;
// 	}	else
//   OnSignal( PBL_SIG_BALL2_OFF) {
// 		m_aAliveBall[1] = PBL_DEAD;
// 	} else
//   OnSignal( PBL_SIG_BALL3_OFF) {
// 		m_aAliveBall[2] = PBL_DEAD;
// 	}	else
// 	OnSignal( PBL_SIG_BALL4_OFF) {
// 		m_aAliveBall[3] = PBL_DEAD;
// 	}	else
// 	OnSignal( PBL_SIG_BUMPER_ON ) {
// 		//m_iScore += 450;
// 		m_iBumps++;
//  	}	else 
//  		OnSignal( PBL_SIG_LOCK_1 OR_SI PBL_SIG_LOCK_2 OR_SI PBL_SIG_LOCK_3 OR_SI PBL_SIG_LOCK_4) {
// 		if (this->alive() == 0) {
// 			m_Text1 = "   press enter to launch ball";
// 			m_bLaunch = true;
// 		}
//  		//m_iScore += 7500;
// 	}	else
// 	OnSignal( PBL_SIG_EXTRABALL ) {
// 		m_bExtraBall = true;
// 		//m_iScore += 50000;
// 	}

// 	OnSignal( PBL_SIG_BALL1_OFF OR_SI 
// 						PBL_SIG_BALL2_OFF OR_SI	
// 						PBL_SIG_BALL3_OFF OR_SI 
// 						PBL_SIG_BALL4_OFF ) {
// 		//if (m_iActiveBalls == 1) {
// 		//SendSignal( PBL_SIG_MULTIBALL_OFF, 0, this->getParent(), NULL );
// 		//SendSignal( PBL_SIG_LOCK_ON, 0, this->getParent(), NULL );
// 		//}
// 		EM_COUT("ball dead, locked " << this->locked() <<" alive "<< this->alive(), 1);
// 		if (this->locked() == 0 && this->alive() == 1) {
// 			SendSignal(PBL_SIG_MULTIBALL_OFF, 0, this->getParent(), NULL);
// 			EM_COUT("multiball off", 1);
// 		}

// 		if (this->alive() == 0) {
// 			if (m_bExtraBall) {
// 				m_bExtraBall = false;
// 			} else {
// 				m_iBall++;
// 			}
// 			if (m_iBall > 3) {
// 				m_Text1 = "   press r to start new game";
// 			} else {
// 				m_bLaunch = true;
// 				m_Text1 = "   press enter to launch ball";
// 				SendSignal(PBL_SIG_ALLBALLS_OFF, 0, this->getParent(), NULL);
// 			}
// 		}
// 	}	
}

extern volatile float g_fFps;

#if EM_DEBUG
extern volatile float em_poly_m;
#endif

void Score::draw() {
	char buffer[256];
	if (this->getCurrentBall() < 4) {
		sprintf(buffer, "SCORE %d BALL %d\n", m_iScore, m_iBall);
	} else {
		sprintf(buffer, "SCORE %d", m_iScore);
	}
	m_Font->printRow(buffer, 0);
	if (Config::getInstance()->getShowFPS()) {
#if EM_DEBUG
		sprintf(buffer, "FPS %.1f %.1f\n", g_fFps, em_poly_m);
#else
		sprintf(buffer, "FPS %.1f\n", g_fFps);
#endif
		m_Font->printRow(buffer, 1);
	}
	m_Font->printRowCenter(m_Text1, 6);
	m_Font->printRowCenter(m_Text2, 7);
	m_Font->printRowCenter(m_Text3, 8);
	m_Font->printRowCenter(m_Text4, 9);
}

void Score::clear() {
	this->clearText();
	m_iBall = 1;
	m_aBall[0] = PBL_DEAD;
	m_aBall[1] = PBL_DEAD;
	m_aBall[2] = PBL_DEAD;
	m_aBall[3] = PBL_DEAD;
	m_iScore = 0;
	m_iFactor = 1;
}

