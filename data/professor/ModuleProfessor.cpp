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
#include "Table.h"
#include "Keyboard.h"
#include "Score.h"
#include "Config.h"

class ProfessorBehavior : public Behavior {
public:
  ProfessorBehavior() : Behavior() {
    this->clear();
  };
  ~ProfessorBehavior() {};
  
  void onTick() {
    Table * table = Table::getInstance();
    Score * score = table->getScore();
    Loader * loader = Loader::getInstance();
    // init signals
    EmAssert(score != NULL, "ProfessorBehavior::onTick socre NULL");
    // launch ball
    string launch("launch");
    if (table->active() == 0 && 
				table->getCurrentBall() < MAX_BALL) {
      switch (table->getCurrentBall()) {
      case 0 :
				if (table->isBallDead(0) ) {
					SendSignal( loader->getSignal("game_start"), 0, this->getParent(), NULL );
					SendSignal( PBL_SIG_BALL_ON, 0, this->getParent(), NULL );
					table->activateBall(0, 19.5f, 0.0f, 30.0f);	
					//score->clearText();
					break;
				}	
      case 1 :
				if (table->isBallDead(1)) {
					SendSignal( PBL_SIG_BALL_ON, 0, this->getParent(), NULL );
					table->activateBall(1, 19.5f, 0.0f, 30.0f);
					//score->clearText();
					break;
				}
      case 2 :
				if (table->isBallDead(2)) {
					SendSignal( PBL_SIG_BALL_ON, 0, this->getParent(), NULL );
					table->activateBall(2, 19.5f, 0.0f, 30.0f);
					//score->clearText();
					break;
				}
				if (table->isBallDead(0)) {
					SendSignal( PBL_SIG_BALL_ON, 0, this->getParent(), NULL );
					table->activateBall(0, 19.5f, 0.0f, 30.0f);	
					//score->clearText();
					break;
				}	
				if (table->isBallDead(1)) {
					SendSignal( PBL_SIG_BALL_ON, 0, this->getParent(), NULL );
					table->activateBall(1, 19.5f, 0.0f, 30.0f);
					//score->clearText();
					break;
				}
      default:
				throw string("all balls busy");
      }
      EM_COUT("Table::onTick() new ball", 1);
    }
  };
  
  void StdOnCollision() {};
  
  void StdOnSignal() {
    //EM_COUT((int)em_signal, 1);
    Table * table = Table::getInstance();
    Score * score = table->getScore();
    Loader * loader = Loader::getInstance();

    OnSignal( PBL_SIG_RESET_ALL ) {
      this->clear();
    } 
		// ball dead
		ElseOnSignal( PBL_SIG_BALL_OFF ) {
      if (table->active() == 1) {
				SendSignal( loader->getSignal("multiball_off"), 0, this->getParent(), NULL );
      }
      if (table->active() == 0) {
				SendSignal( loader->getSignal("allballs_off"), 0, this->getParent(), NULL );
				if (table->getCurrentBall() < MAX_BALL) {
					table->setCurrentBall(table->getCurrentBall()+1);
					if (table->getCurrentBall() == MAX_BALL) {
						SendSignal( PBL_SIG_GAME_OVER, 0, this->getParent(), NULL );
						EM_COUT("game over", 1);
					}
				}
      }
    }
    ElseOnSignal(loader->getSignal("bump")) {
      score->addScore(450);
    } 
  }
  
  void clear() {
  };
  
private:
  
};

extern "C"  void * new_object_fct(void) {
  return new ProfessorBehavior();
}

