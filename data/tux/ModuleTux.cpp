/***************************************************************************
                          Score.cpp  -  description
                             -------------------
    begin                : Fri Jan 26 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "Behavior.h"
#include "Group.h"
#include "Pinball.h"
#include "Loader.h"
#include "StateMachine.h"
#include "Score.h"
#include "Keyboard.h"

class TuxBehavior : public Behavior {
public:
	TuxBehavior() : Behavior() {
		Loader * loader = Loader::getInstance();
		// init signals
		m_sigReleaseLock = loader->getSignal("releaselock");
		m_sigRightLoop = loader->getSignal("rightloop");
		m_sigLinuxAll = loader->getSignal("linux_all");
		m_sigLinux[0] = loader->getSignal("linuxl");
		m_sigLinux[1] = loader->getSignal("linuxi");
		m_sigLinux[2] = loader->getSignal("linuxn");
		m_sigLinux[3] = loader->getSignal("linuxu");
		m_sigLinux[4] = loader->getSignal("linuxx");
		m_sigLinuxOn[0] = loader->getSignal("linuxl_on");
		m_sigLinuxOn[1] = loader->getSignal("linuxi_on");
		m_sigLinuxOn[2] = loader->getSignal("linuxn_on");
		m_sigLinuxOn[3] = loader->getSignal("linuxu_on");
		m_sigLinuxOn[4] = loader->getSignal("linuxx_on");
		m_sigLinuxOff[0] = loader->getSignal("linuxl_off");
		m_sigLinuxOff[1] = loader->getSignal("linuxi_off");
		m_sigLinuxOff[2] = loader->getSignal("linuxn_off");
		m_sigLinuxOff[3] = loader->getSignal("linuxu_off");
		m_sigLinuxOff[4] = loader->getSignal("linuxx_off");
		m_sigTuxAll = loader->getSignal("tux_all");
		m_sigTux[0] = loader->getSignal("tuxt");
		m_sigTux[1] = loader->getSignal("tuxu");
		m_sigTux[2] = loader->getSignal("tuxx");
		m_sigTuxOn[0] = loader->getSignal("tuxt_on");
		m_sigTuxOn[1] = loader->getSignal("tuxu_on");
		m_sigTuxOn[2] = loader->getSignal("tuxx_on");
		m_sigTuxOff[0] = loader->getSignal("tuxt_off");
		m_sigTuxOff[1] = loader->getSignal("tuxu_off");
		m_sigTuxOff[2] = loader->getSignal("tuxx_off");
		m_sigBootAll = loader->getSignal("boot_all");
		m_sigBoot[0] = loader->getSignal("bootb");
		m_sigBoot[1] = loader->getSignal("booto");
		m_sigBoot[2] = loader->getSignal("bootoo");
		m_sigBoot[3] = loader->getSignal("boott");
		m_sigBootOn[0] = loader->getSignal("bootb_on");
		m_sigBootOn[1] = loader->getSignal("booto_on");
		m_sigBootOn[2] = loader->getSignal("bootoo_on");
		m_sigBootOn[3] = loader->getSignal("boott_on");
		m_sigBootOff[0] = loader->getSignal("bootb_off");
		m_sigBootOff[1] = loader->getSignal("booto_off");
		m_sigBootOff[2] = loader->getSignal("bootoo_off");
		m_sigBootOff[3] = loader->getSignal("boott_off");

		this->clear();
	};
	~TuxBehavior() {};

	void onTick() {
		if (Keyboard::isKeyDown(SDLK_RETURN)) {
			cerr << "active "<< Score::getInstance()->active() << 
				" locked " << Score::getInstance()->locked() << endl;
		}
		// launch ball
		if (Score::getInstance()->active() == 0 && 
				Score::getInstance()->getCurrentBall() < 4 
				&& Keyboard::isKeyDown(SDLK_RETURN)) {
			switch (Score::getInstance()->getCurrentBall()) {
			case 1 :
				if (Score::getInstance()->isBallDead(PBL_BALL_1) ) {
					SendSignal( PBL_SIG_BALL1_ON, 0, this->getParent(), NULL );
					Score::getInstance()->activateBall(PBL_BALL_1);	
					Score::getInstance()->clearText();
					break;
				}	
			case 2 :
				if (Score::getInstance()->isBallDead(PBL_BALL_2)) {
					SendSignal( PBL_SIG_BALL2_ON, 0, this->getParent(), NULL );
					Score::getInstance()->activateBall(PBL_BALL_2);
					Score::getInstance()->clearText();
					break;
				}
			case 3 :
				if (Score::getInstance()->isBallDead(PBL_BALL_3)) {
					SendSignal( PBL_SIG_BALL3_ON, 0, this->getParent(), NULL );
					Score::getInstance()->activateBall(PBL_BALL_3);
					Score::getInstance()->clearText();
					break;
				}
				if (Score::getInstance()->isBallDead(PBL_BALL_1) ) {
					SendSignal( PBL_SIG_BALL1_ON, 0, this->getParent(), NULL );
					Score::getInstance()->activateBall(PBL_BALL_1);	
					Score::getInstance()->clearText();
					break;
				}	
				if (Score::getInstance()->isBallDead(PBL_BALL_2)) {
					SendSignal( PBL_SIG_BALL2_ON, 0, this->getParent(), NULL );
					Score::getInstance()->activateBall(PBL_BALL_2);
					Score::getInstance()->clearText();
					break;
				}
			default:
				throw string("all balls busy");
			}
			EM_COUT("Score::onTick() new ball", 1);
		}
	};

	void StdOnCollision() {};

	void StdOnSignal() {
		EM_COUT((int)em_signal, 1);
		
		OnSignal( PBL_SIG_RESET_ALL ) {
			this->clear();
		} else
		// ball dead
		OnSignal( PBL_SIG_BALL1_OFF OR_SI 
							PBL_SIG_BALL2_OFF OR_SI	
							PBL_SIG_BALL3_OFF OR_SI 
							PBL_SIG_BALL4_OFF ) {
			if (Score::getInstance()->active() == 1) {
				SendSignal( PBL_SIG_MULTIBALL_OFF, 0, this->getParent(), NULL );
			}
			if (Score::getInstance()->active() == 0) {
				if (Score::getInstance()->getCurrentBall() < 3 || !m_bExtraBall) {
					if (m_bExtraBall) {
						m_bExtraBall = false;
					} else {
						Score::getInstance()->setCurrentBall(Score::getInstance()->getCurrentBall()+1);
					}
				} else {
					SendSignal( PBL_SIG_GAMEOVER, 0, this->getParent(), NULL );
				}
			}
		} else
		// multiball
		OnSignal( m_sigReleaseLock ) {
			Score::getInstance()->unLockBall(0);
			Score::getInstance()->unLockBall(1);
			Score::getInstance()->unLockBall(2);
		} else
		// LINUX
		OnSignal(m_sigLinux[0]) {
			if (m_aLinux[0]) {
 				SendSignal(m_sigLinuxOff[0], 0, this->getParent(), NULL);
				m_aLinux[0] = false;
			} else {
 				SendSignal(m_sigLinuxOn[0], 0, this->getParent(), NULL);
				m_aLinux[0] = true;
			}
		} else
		OnSignal(m_sigLinux[1]) {
			if (m_aLinux[1]) {
 				SendSignal(m_sigLinuxOff[1], 0, this->getParent(), NULL);
				m_aLinux[1] = false;
			} else {
 				SendSignal(m_sigLinuxOn[1], 0, this->getParent(), NULL);
				m_aLinux[1] = true;
			}
		} else
		OnSignal(m_sigLinux[2]) {
			if (m_aLinux[2]) {
 				SendSignal(m_sigLinuxOff[2], 0, this->getParent(), NULL);
				m_aLinux[2] = false;
			} else {
 				SendSignal(m_sigLinuxOn[2], 0, this->getParent(), NULL);
				m_aLinux[2] = true;
			}
		} else
		OnSignal(m_sigLinux[3]) {
			if (m_aLinux[3]) {
 				SendSignal(m_sigLinuxOff[3], 0, this->getParent(), NULL);
				m_aLinux[3] = false;
			} else {
 				SendSignal(m_sigLinuxOn[3], 0, this->getParent(), NULL);
				m_aLinux[3] = true;
			}
		} else
		OnSignal(m_sigLinux[4]) {
			if (m_aLinux[4]) {
 				SendSignal(m_sigLinuxOff[4], 0, this->getParent(), NULL);
				m_aLinux[4] = false;
			} else {
 				SendSignal(m_sigLinuxOn[4], 0, this->getParent(), NULL);
				m_aLinux[4] = true;
			}
		} else
		// TUX
		OnSignal(m_sigTux[0]) {
			if (!m_aTux[0]) {
 				SendSignal(m_sigTuxOn[0], 0, this->getParent(), NULL);
				m_aTux[0] = true;
			}
		} else
		OnSignal(m_sigTux[1]) {
			if (!m_aTux[1]) {
 				SendSignal(m_sigTuxOn[1], 0, this->getParent(), NULL);
				m_aTux[1] = true;
			}
		} else
		OnSignal(m_sigTux[2]) {
			if (!m_aTux[2]) {
 				SendSignal(m_sigTuxOn[2], 0, this->getParent(), NULL);
				m_aTux[2] = true;
			}
		} else
 		// BOOT
		OnSignal( m_sigBoot[0] ) {
			if (m_aBoot[0]) {
 				SendSignal(m_sigBootOff[0], 0, this->getParent(), NULL);
				m_aBoot[0] = false;
			} else {
 				SendSignal(m_sigBootOn[0], 0, this->getParent(), NULL);
				m_aBoot[0] = true;
			}
		} else
		OnSignal( m_sigBoot[1] ) {
			if (m_aBoot[1]) {
 				SendSignal(m_sigBootOff[1], 0, this->getParent(), NULL);
				m_aBoot[1] = false;
			} else {
 				SendSignal(m_sigBootOn[1], 0, this->getParent(), NULL);
				m_aBoot[1] = true;
			}
		} else
		OnSignal( m_sigBoot[2] ) {
			if (m_aBoot[2]) {
 				SendSignal(m_sigBootOff[2], 0, this->getParent(), NULL);
				m_aBoot[2] = false;
			} else {
 				SendSignal(m_sigBootOn[2], 0, this->getParent(), NULL);
				m_aBoot[2] = true;
			}
		} else
		OnSignal( m_sigBoot[3] ) {
			if (m_aBoot[3]) {
 				SendSignal(m_sigBootOff[3], 0, this->getParent(), NULL);
				m_aBoot[3] = false;
			} else {
 				SendSignal(m_sigBootOn[3], 0, this->getParent(), NULL);
				m_aBoot[3] = true;
			}
		} else
		//
		OnSignal( PBL_SIG_LEFTARM_ON ) {
			// BOOT
			bool tmp = m_aBoot[0];
			for (int a=0; a<3; ++a) {
				if (m_aBoot[a] != m_aBoot[a+1]) {
					m_aBoot[a] = m_aBoot[a+1];
					if (m_aBoot[a]) SendSignal(m_sigBootOn[a], 0, this->getParent(), NULL);
					else SendSignal(m_sigBootOff[a], 0, this->getParent(), NULL);
				}
			}
			if (m_aBoot[3] != tmp) {
				m_aBoot[3] = tmp;
				if (m_aBoot[3]) SendSignal(m_sigBootOn[3], 0, this->getParent(), NULL);
				else SendSignal(m_sigBootOff[3], 0, this->getParent(), NULL);
			}
			// LINUX
			tmp = m_aLinux[0];
			for (int a=0; a<4; ++a) {
				if (m_aLinux[a] != m_aLinux[a+1]) {
					m_aLinux[a] = m_aLinux[a+1];
					if (m_aLinux[a]) SendSignal(m_sigLinuxOn[a], 0, this->getParent(), NULL);
					else SendSignal(m_sigLinuxOff[a], 0, this->getParent(), NULL);
				}
			}
			if (m_aLinux[4] != tmp) {
				m_aLinux[4] = tmp;
				if (m_aLinux[4]) SendSignal(m_sigLinuxOn[4], 0, this->getParent(), NULL);
				else SendSignal(m_sigLinuxOff[4], 0, this->getParent(), NULL);
			}
		} else
		OnSignal( PBL_SIG_RIGHTARM_ON ) {
			// BOOT
			bool tmp = m_aBoot[3];
			for (int a=3; a>0; --a) {
				if (m_aBoot[a] != m_aBoot[a-1]) {
					m_aBoot[a] = m_aBoot[a-1];
					if (m_aBoot[a]) SendSignal(m_sigBootOn[a], 0, this->getParent(), NULL);
					else SendSignal(m_sigBootOff[a], 0, this->getParent(), NULL);
				}
			}
			if (m_aBoot[0] != tmp) {
				m_aBoot[0] = tmp;
				if (m_aBoot[0]) SendSignal(m_sigBootOn[0], 0, this->getParent(), NULL);
				else SendSignal(m_sigBootOff[0], 0, this->getParent(), NULL);
			}
			// LINUX
			tmp = m_aLinux[4];
			for (int a=4; a>0; --a) {
				if (m_aLinux[a] != m_aLinux[a-1]) {
					m_aLinux[a] = m_aLinux[a-1];
					if (m_aLinux[a]) SendSignal(m_sigLinuxOn[a], 0, this->getParent(), NULL);
					else SendSignal(m_sigLinuxOff[a], 0, this->getParent(), NULL);
				}
			}
			if (m_aLinux[0] != tmp) {
				m_aLinux[0] = tmp;
				if (m_aLinux[0]) SendSignal(m_sigLinuxOn[0], 0, this->getParent(), NULL);
				else SendSignal(m_sigLinuxOff[0], 0, this->getParent(), NULL);
			}
		} else
		OnSignal(m_sigRightLoop) {
			if (m_bExtraBallWaiting) {
				m_bExtraBall = true;
				m_bExtraBallWaiting = false;
			}
		}
			
		// TUX all
		if (m_aTux[0] && m_aTux[1] && m_aTux[2]) {
			SendSignal(m_sigTuxAll, 0, this->getParent(), NULL);
			m_aTux[0] = m_aTux[1] = m_aTux[2] = false;
			m_bExtraBallWaiting = true;
		}

		// LINUX all
		if (m_aLinux[0] && m_aLinux[1] && m_aLinux[2] && m_aLinux[3] && m_aLinux[4]) {
			SendSignal(m_sigLinuxAll, 0, this->getParent(), NULL);
			m_aLinux[0] = m_aLinux[1] = m_aLinux[2] = m_aLinux[3] = m_aLinux[4] = false;
		}

		// BOOT all
		if (m_aBoot[0] && m_aBoot[1] && m_aBoot[2] && m_aBoot[3]) {
			SendSignal(m_sigBootAll, 0, this->getParent(), NULL);
			m_aBoot[0] = m_aBoot[1] = m_aBoot[2] = m_aBoot[3] = false;
		}
	}

	void clear() {
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
		m_bExtraBall = false;
		m_bExtraBallWaiting = false;
	};

private:
	int m_sigReleaseLock;
	int m_sigRightLoop;
	int m_sigLinuxAll;
	int m_sigLinux[5];
	int m_sigLinuxOn[5];
	int m_sigLinuxOff[5];
	int m_sigTuxAll;
	int m_sigTux[3];
	int m_sigTuxOn[3];
	int m_sigTuxOff[3];
	int m_sigBootAll;
	int m_sigBoot[4];
	int m_sigBootOn[4];
	int m_sigBootOff[4];
	bool m_aLinux[5];
	bool m_aBoot[4];
	bool m_aTux[3];
	bool m_bExtraBall;
	bool m_bExtraBallWaiting;
	bool m_bLaunch;
};

extern "C"  void * new_object_fct(void) {
	return new TuxBehavior();
}

