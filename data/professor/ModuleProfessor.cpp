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

class ProfessorBehavior : public Behavior {
public:
	ProfessorBehavior() : Behavior() {
		Loader * loader = Loader::getInstance();
		// init signals
		//m_sigRightLoop = loader->getSignal("rightloop");
		this->clear();
	};
	~ProfessorBehavior() {};

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
		//EM_COUT((int)em_signal, 1);
		
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
				if (Score::getInstance()->getCurrentBall() < 3) {
					Score::getInstance()->setCurrentBall(Score::getInstance()->getCurrentBall()+1);
				} else {
					SendSignal( PBL_SIG_GAMEOVER, 0, this->getParent(), NULL );
				}
			}
		}
	}

	void clear() {
	};

private:

};

extern "C"  void * new_object_fct(void) {
	return new ProfessorBehavior();
}

