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
//#include "SoundUtil.h"
#include "TextureUtil.h"
#include "EmFont.h"
#include "Config.h"

extern int em_width_;
extern int em_height_;

Score* Score::p_Score = NULL;

Score::Score() {
	char filename[256];
	/*
	sprintf(filename, "%s/bump.wav", Config::getInstance()->getDataDir());
	m_aSample[0] = SoundUtil::getInstance()->loadSample(filename);
	sprintf(filename, "%s/fjong.wav", Config::getInstance()->getDataDir());
	m_aSample[1] = SoundUtil::getInstance()->loadSample(filename);
	sprintf(filename, "%s/lock.wav", Config::getInstance()->getDataDir());
	m_aSample[2] = SoundUtil::getInstance()->loadSample(filename);
	sprintf(filename, "%s/fart.wav", Config::getInstance()->getDataDir());
	m_aSample[3] = SoundUtil::getInstance()->loadSample(filename);
	sprintf(filename, "%s/flush.wav", Config::getInstance()->getDataDir());
	m_aSample[4] = SoundUtil::getInstance()->loadSample(filename);
	*/
	m_Font = EmFont::getInstance();
	sprintf(filename, "%s/font_34.png", Config::getInstance()->getDataDir());
	m_Font->loadFont(filename);

	this->clear();
}

Score::~Score(){
}

Score* Score::getInstance() {
	if (p_Score == NULL) {
		p_Score = new Score();
	}
	return p_Score;
}

void Score::addScore(int s) {
	m_iScore += (s*m_iFactor);
}

bool Score::isBallActive(int ball) {
	switch (ball) {
	case PBL_BALL_1: return m_aAliveBall[0] == PBL_ALIVE;
	case PBL_BALL_2: return m_aAliveBall[1] == PBL_ALIVE;
	case PBL_BALL_3: return m_aAliveBall[2] == PBL_ALIVE;
	default: return m_aAliveBall[3] == PBL_ALIVE;
	}
}

void Score::lockBall(int ball) {
	switch (ball) {
	case PBL_BALL_1: m_aAliveBall[0] = PBL_LOCKED; break;
	case PBL_BALL_2: m_aAliveBall[1] = PBL_LOCKED; break; 
	case PBL_BALL_3: m_aAliveBall[2] = PBL_LOCKED; break;
	default: m_aAliveBall[3] = PBL_LOCKED;
	}
}

void Score::unLockBall(int ball) {
	switch (ball) {
	case PBL_BALL_1 : if (m_aAliveBall[0] == PBL_LOCKED) m_aAliveBall[0] = PBL_ALIVE; break;	
	case PBL_BALL_2 : if (m_aAliveBall[1] == PBL_LOCKED) m_aAliveBall[1] = PBL_ALIVE; break;
	case PBL_BALL_3 : if (m_aAliveBall[2] == PBL_LOCKED) m_aAliveBall[2] = PBL_ALIVE; break;
	default : if (m_aAliveBall[3] == PBL_LOCKED) m_aAliveBall[3] = PBL_ALIVE; break;
	}
}

void Score::onTick() {
	if (m_bLaunch && m_iBall < 4 && Keyboard::isKeyDown(SDLK_RETURN)) {
		m_bLaunch = false;
		switch (m_iBall) {
		case 1 :
			if (m_aAliveBall[0] == PBL_DEAD) {
				SendSignal( PBL_SIG_BALL1_ON, 0, this->p_Parent, NULL );
				m_aAliveBall[0] = PBL_ALIVE;	
				m_Text1 = "";	
				m_Text2 = "";
				break;
			}	
		case 2 :
			if (m_aAliveBall[1] == PBL_DEAD) {
				SendSignal( PBL_SIG_BALL2_ON, 0, this->p_Parent, NULL );
				m_aAliveBall[1] = PBL_ALIVE;	
				m_Text1 = "";	
				m_Text2 = "";
				break;
			}
		case 3 :
			if (m_aAliveBall[2] == PBL_DEAD) {
				SendSignal( PBL_SIG_BALL3_ON, 0, this->p_Parent, NULL );
				m_aAliveBall[2] = PBL_ALIVE;
				m_Text1 = "";
				m_Text2 = "";
				break;
			}
		default:
			if (m_aAliveBall[0] == PBL_DEAD) {
				SendSignal( PBL_SIG_BALL1_ON, 0, this->p_Parent, NULL );
				m_aAliveBall[0] = PBL_ALIVE;
				m_Text1 = "";
				m_Text2 = "";
				break;
			}	
			if (m_aAliveBall[1] == PBL_DEAD) {
				SendSignal( PBL_SIG_BALL2_ON, 0, this->p_Parent, NULL );
				m_aAliveBall[1] = PBL_ALIVE;
				m_Text1 = "";
				m_Text2 = "";
				break;
			}
			if (m_aAliveBall[2] == PBL_DEAD) {
				SendSignal( PBL_SIG_BALL3_ON, 0, this->p_Parent, NULL );
				m_aAliveBall[2] = PBL_ALIVE;
				m_Text1 = "";
				m_Text2 = "";
				break;
			}
			throw string("All balls busy");
		}
		EM_COUT("Score::onTick() new ball", 1);
	}
}

void Score::StdOnSignal() {
	EM_COUT(cerr << (int)em_signal, 1);

	OnSignal( PBL_SIG_RESET_ALL ) {
		this->clear();
	} else
	OnSignal( PBL_SIG_BALL1_OFF) {
		m_aAliveBall[0] = PBL_DEAD;
	}	else
  OnSignal( PBL_SIG_BALL2_OFF) {
		m_aAliveBall[1] = PBL_DEAD;
	} else
  OnSignal( PBL_SIG_BALL3_OFF) {
		m_aAliveBall[2] = PBL_DEAD;
	}	else
	OnSignal( PBL_SIG_BALL4_OFF) {
		m_aAliveBall[3] = PBL_DEAD;
	}	else
	OnSignal( PBL_SIG_BUMPER_ON ) {
		m_iScore += 450;
		m_iBumps++;
 	}	else 
 		OnSignal( PBL_SIG_LOCK_1_LOCK OR_SI PBL_SIG_LOCK_2_LOCK ) {
		m_bLaunch = true;
 		m_iScore += 7500;
	}	else
	OnSignal( PBL_SIG_CAVE_ON ) {
		m_iScore += 10000;
		if (m_aMission[1] == 1) {
			m_aMission[1] = -1;
			m_iScore += 50000;
		}
	}	else
	OnSignal( PBL_SIG_CAVE_OFF ) {
	} else 
		// L I N U X
	OnSignal( PBL_SIG_LINUX_L_ON ) {
		m_aLinux[0] = true;
	} else
	OnSignal( PBL_SIG_LINUX_I_ON ) {
		m_aLinux[1] = true;
	} else
	OnSignal( PBL_SIG_LINUX_N_ON ) {
		m_aLinux[2] = true;
	} else
	OnSignal( PBL_SIG_LINUX_U_ON ) {
		m_aLinux[3] = true;
	} else
	OnSignal( PBL_SIG_LINUX_X_ON ) {
		m_aLinux[4] = true;
	} else
	OnSignal( PBL_SIG_LINUX_L_OFF ) {
		m_aLinux[0] = false;
	} else
	OnSignal( PBL_SIG_LINUX_I_OFF ) {
		m_aLinux[1] = false;
	} else
	OnSignal( PBL_SIG_LINUX_N_OFF ) {
		m_aLinux[2] = false;
	} else
	OnSignal( PBL_SIG_LINUX_U_OFF ) {
		m_aLinux[3] = false;
	} else
	OnSignal( PBL_SIG_LINUX_X_OFF ) {
		m_aLinux[4] = false;
	} else 
	OnSignal( PBL_SIG_LINUX_I_ON ) {
		m_aLinux[1] = true;
	} else
		// T U X
	OnSignal( PBL_SIG_TUX_T_ON ) {
		m_aTux[0] = true;
	} else
	OnSignal( PBL_SIG_TUX_U_ON ) {
		m_aTux[1] = true;
	} else
	OnSignal( PBL_SIG_TUX_X_ON ) {
		m_aTux[2] = true;
	} else
	OnSignal( PBL_SIG_TUX_T_OFF ) {
		m_aTux[0] = false;
	} else
	OnSignal( PBL_SIG_TUX_U_OFF ) {
		m_aTux[1] = false;
	} else
	OnSignal( PBL_SIG_TUX_X_OFF ) {
		m_aTux[2] = false;
	} else
		// B O O T
	OnSignal( PBL_SIG_BOOT_B_ON ) {
		m_aBoot[0] = true;
	} else
	OnSignal( PBL_SIG_BOOT_O_ON ) {
		m_aBoot[1] = true;
	} else
	OnSignal( PBL_SIG_BOOT_2_ON ) {
		m_aBoot[2] = true;
	} else
	OnSignal( PBL_SIG_BOOT_T_ON ) {
		m_aBoot[3] = true;
	} else
	OnSignal( PBL_SIG_BOOT_B_OFF ) {
		m_aBoot[0] = false;
	} else
	OnSignal( PBL_SIG_BOOT_O_OFF ) {
		m_aBoot[1] = false;
	} else
	OnSignal( PBL_SIG_BOOT_2_OFF ) {
		m_aBoot[2] = false;
	} else
	OnSignal( PBL_SIG_BOOT_T_OFF ) {
		m_aBoot[3] = false;
	} else
		// Arms shifts BOOT
	OnSignal( PBL_SIG_RIGHTARM_ON ) {
		bool tmp = m_aBoot[0];
		m_aBoot[0] = m_aBoot[3];
		m_aBoot[3] = m_aBoot[2];
		m_aBoot[2] = m_aBoot[1];
		m_aBoot[1] = tmp;
		if (m_aBoot[0])	SendSignal(PBL_SIG_BOOT_B_ON, 0, this->p_Parent, NULL);
		else            SendSignal(PBL_SIG_BOOT_B_OFF, 0, this->p_Parent, NULL);
		if (m_aBoot[1])	SendSignal(PBL_SIG_BOOT_O_ON, 0, this->p_Parent, NULL);
		else            SendSignal(PBL_SIG_BOOT_O_OFF, 0, this->p_Parent, NULL);
		if (m_aBoot[2])	SendSignal(PBL_SIG_BOOT_2_ON, 0, this->p_Parent, NULL);
		else            SendSignal(PBL_SIG_BOOT_2_OFF, 0, this->p_Parent, NULL);
		if (m_aBoot[3])	SendSignal(PBL_SIG_BOOT_T_ON, 0, this->p_Parent, NULL);
		else            SendSignal(PBL_SIG_BOOT_T_OFF, 0, this->p_Parent, NULL);
	} else
	OnSignal( PBL_SIG_LEFTARM_ON ) {
		bool tmp = m_aBoot[0];
		m_aBoot[0] = m_aBoot[1];
		m_aBoot[1] = m_aBoot[2];
		m_aBoot[2] = m_aBoot[3];
		m_aBoot[3] = tmp;
		if (m_aBoot[0])	SendSignal(PBL_SIG_BOOT_B_ON, 0, this->p_Parent, NULL);
		else            SendSignal(PBL_SIG_BOOT_B_OFF, 0, this->p_Parent, NULL);
		if (m_aBoot[1])	SendSignal(PBL_SIG_BOOT_O_ON, 0, this->p_Parent, NULL);
		else            SendSignal(PBL_SIG_BOOT_O_OFF, 0, this->p_Parent, NULL);
		if (m_aBoot[2])	SendSignal(PBL_SIG_BOOT_2_ON, 0, this->p_Parent, NULL);
		else            SendSignal(PBL_SIG_BOOT_2_OFF, 0, this->p_Parent, NULL);
		if (m_aBoot[3])	SendSignal(PBL_SIG_BOOT_T_ON, 0, this->p_Parent, NULL);
		else            SendSignal(PBL_SIG_BOOT_T_OFF, 0, this->p_Parent, NULL);
	} else
		// Stuff
	OnSignal( PBL_SIG_JACKPOT ) {
		m_iScore += 150000;
	} else
	OnSignal( PBL_SIG_EXTRA_BALL ) {
		m_bExtraBall = true;
		m_iScore += 50000;
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

  OnSignal( PBL_SIG_TUX_T_ON OR_SI
						PBL_SIG_TUX_U_ON OR_SI
						PBL_SIG_TUX_X_ON ) {
		if (m_aTux[0] && m_aTux[1] && m_aTux[2]) {
			SendSignal(PBL_SIG_TUX_ALL, 0, this->p_Parent, NULL);
			m_aTux[0] = m_aTux[1] = m_aTux[2] = false;
		}
	}

  OnSignal( PBL_SIG_BOOT_B_ON OR_SI
						PBL_SIG_BOOT_O_ON OR_SI
						PBL_SIG_BOOT_2_ON OR_SI
						PBL_SIG_BOOT_T_ON ) {
		if (m_aBoot[0] && m_aBoot[1] && m_aBoot[2] && m_aBoot[3]) {
			SendSignal(PBL_SIG_BOOT_ALL, 0, this->p_Parent, NULL);
			m_aBoot[0] = m_aBoot[1] = m_aBoot[2] = m_aBoot[3] = false;
		}
	}

  OnSignal( PBL_SIG_LINUX_L_ON OR_SI
						PBL_SIG_LINUX_I_ON OR_SI
						PBL_SIG_LINUX_N_ON OR_SI
						PBL_SIG_LINUX_U_ON OR_SI
						PBL_SIG_LINUX_X_ON ) {
		if (m_aLinux[0] && m_aLinux[1] && m_aLinux[2] && m_aLinux[3] && m_aLinux[4]) {
			if (m_iFactor < 5) m_iFactor++;
			switch (m_iFactor) {
			case 1: SendSignal(PBL_SIG_FACTOR_1, 0, this->p_Parent, NULL); break;
			case 2: SendSignal(PBL_SIG_FACTOR_2, 0, this->p_Parent, NULL); break;
			case 3: SendSignal(PBL_SIG_FACTOR_3, 0, this->p_Parent, NULL); break;
			case 4: SendSignal(PBL_SIG_FACTOR_4, 0, this->p_Parent, NULL); break;
			case 5: SendSignal(PBL_SIG_FACTOR_5, 0, this->p_Parent, NULL); break;
			}
			m_aLinux[0] = m_aLinux[1] = m_aLinux[2] = m_aLinux[3] = m_aLinux[4] = false;
			SendSignal(PBL_SIG_LINUX_ALL, 0, this->p_Parent, NULL);
		}
	}


	OnSignal( PBL_SIG_BALL1_OFF OR_SI 
						PBL_SIG_BALL2_OFF OR_SI	
						PBL_SIG_BALL3_OFF OR_SI 
						PBL_SIG_BALL4_OFF ) {
		//if (m_iActiveBalls == 1) {
		//SendSignal( PBL_SIG_MULTIBALL_OFF, 0, this->p_Parent, NULL );
		//SendSignal( PBL_SIG_LOCK_ON, 0, this->p_Parent, NULL );
		//}
		int alive = 0;
		if (m_aAliveBall[0] == PBL_ALIVE) alive++;
		if (m_aAliveBall[1] == PBL_ALIVE)	alive++;
		if (m_aAliveBall[2] == PBL_ALIVE) alive++;
		if (m_aAliveBall[3] == PBL_ALIVE) alive++;
		int locked = 0;
		if (m_aAliveBall[0] == PBL_LOCKED) locked++;
		if (m_aAliveBall[1] == PBL_LOCKED) locked++;
		if (m_aAliveBall[2] == PBL_LOCKED) locked++;
		if (m_aAliveBall[3] == PBL_LOCKED) locked++;
		
		if (locked == 0 && alive == 1) {
			SendSignal(PBL_SIG_MULTIBALL_OFF, 0, this->p_Parent, NULL);
			cerr << "multiball off" << endl;
		}

		if (alive == 0) {
			if (m_iBall >= 3) {
				m_Text1 = "press r to start new game";
			} else {
				if (m_bExtraBall) {
					m_bExtraBall = false;
				} else {
					m_iBall++;
				}
				m_bLaunch = true;
				m_Text1 = "press enter to launch ball";
				SendSignal(PBL_SIG_BALL_ALL_OFF, 0, this->p_Parent, NULL);
			}
		}
	}	
}

extern int em_width_div2_;
extern int em_height_div2_;
extern volatile float g_fFps;

void Score::draw() {
	char buffer[256];
	sprintf(buffer, "SCORE %d BALL %d\n", m_iScore, m_iBall);
	m_Font->print(buffer, -1, 1);
	//#if EM_DEBUG
	sprintf(buffer, "FPS %f\n", g_fFps);
	m_Font->print(buffer, -1, 1-EM_FONTSIZE_Y);
	//#endif

	m_Font->print(m_Text1, -1, EM_FONTSIZE_Y/2);
	m_Font->print(m_Text2, -1, -EM_FONTSIZE_Y/2);
}

void Score::clear() {
	m_Text1="press enter to launch ball";
	m_Text2="left and right shift activates flippers";
	m_iBall = 1;
	m_iScore = 0;
	m_iFactor = 1;
	m_iBumps = 0;
	m_bExtraBall = false;
	m_bLaunch = true;
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
	m_aBoot[0] = false;
	m_aBoot[1] = false;
	m_aBoot[2] = false;
	m_aBoot[3] = false;
	m_aTux[0] = false;
	m_aTux[1] = false;
	m_aTux[2] = false;
	m_aAliveBall[0] = PBL_DEAD;
	m_aAliveBall[1] = PBL_DEAD;
	m_aAliveBall[2] = PBL_DEAD;
	m_aAliveBall[3] = PBL_DEAD;
}

