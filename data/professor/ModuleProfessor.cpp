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
    EmAssert(score != NULL, "ProfessorBehavior::onTick socre NULL");
    // launch ball
    string launch("launch");
    if (table->active() == 0 && 
	table->getCurrentBall() < 4 
	&& Keyboard::isKeyDown(Config::getInstance()->getKey(launch))) {
      switch (table->getCurrentBall()) {
      case 0 :
	if (table->isBallDead(0) ) {
	  SendSignal( PBL_SIG_BALL_ON, 0, this->getParent(), NULL );
	  table->activateBall(0);	
	  //score->clearText();
	  break;
	}	
      case 1 :
	if (table->isBallDead(1)) {
	  SendSignal( PBL_SIG_BALL_ON, 0, this->getParent(), NULL );
	  table->activateBall(1);
	  //score->clearText();
	  break;
	}
      case 2 :
	if (table->isBallDead(2)) {
	  SendSignal( PBL_SIG_BALL_ON, 0, this->getParent(), NULL );
	  table->activateBall(2);
	  //score->clearText();
	  break;
	}
	if (table->isBallDead(0)) {
	  SendSignal( PBL_SIG_BALL_ON, 0, this->getParent(), NULL );
	  table->activateBall(0);	
	  //score->clearText();
	  break;
	}	
	if (table->isBallDead(1)) {
	  SendSignal( PBL_SIG_BALL_ON, 0, this->getParent(), NULL );
	  table->activateBall(1);
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
    
    OnSignal( PBL_SIG_RESET_ALL ) {
      this->clear();
    } else
      // ball dead
      OnSignal( PBL_SIG_BALL_OFF ) {
      if (table->active() == 1) {
	SendSignal( PBL_SIG_MULTIBALL_OFF, 0, this->getParent(), NULL );
      }
      if (table->active() == 0) {
	if (table->getCurrentBall() < 3) {
	  table->setCurrentBall(table->getCurrentBall()+1);
	} else {
	  SendSignal( PBL_SIG_GAME_OVER, 0, this->getParent(), NULL );
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

