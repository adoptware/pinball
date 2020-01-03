/***************************************************************************
                          ModuleHurd.cpp  -  description
                             -------------------
    begin                : Mon Jan 11 2010
    copyright            : (C) 2010 by Paolo Caroni
    email                : kenren89@gmail.com

                          ModuleHurd.cpp is a develop of ModuleTux.cpp

                          ModuleTux.cpp  -  description
                             -------------------
    begin                : Fri Jan 26 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <pinball/Private.h>
#include <pinball/Behavior.h>
#include <pinball/Group.h>
#include <pinball/Pinball.h>
#include <pinball/Loader.h>
#include <pinball/StateMachine.h>
#include <pinball/Score.h>
#include <pinball/Keyboard.h>
#include <pinball/Table.h>
#include <pinball/Config.h>

#define MISSION_TEXT_TIMEOUT 6000
#define INFO_TEXT_TIME 600

enum {
  MISSION_FIRST_BUMP=1,
  MISSION_ARROW,
  MISSION_FIRST_CAVE,
  MISSION_SECOND_BUMP,
  MISSION_ARROWS,
  MISSION_SECOND_CAVE,
  MISSION_THIRD_BUMP,
  MISSION_RAMP_ARROWS,
  MISSION_THIRD_CAVE,
  MISSION_FOURTH_BUMP,
  MISSION_FOURTH_CAVE,
  MISSION_FOURTH_ARROWS,
  MISSION_FIFTH_BUMP,
  MISSION_FIFTH_ALL,
  MISSION_FIFTH_CAVE,
  MISSION_ALL
};

class HurdBehavior : public Behavior {
public:
  HurdBehavior() : Behavior() {
    Loader * loader = Loader::getInstance();
    // init shortcuts for signals
    m_sigGameStart = loader->getSignal("game_start");
    m_sigBump = loader->getSignal("bump");
    m_sigJackpot = loader->getSignal("jackpot");
    m_sigExtraBall = loader->getSignal("extraball");
    m_sigAllBallsOff = loader->getSignal("allballs_off");
    m_sigMultiballOff = loader->getSignal("multiball_off");
    m_sigReleaseLock = loader->getSignal("releaselock");
    m_sigLock[0] = loader->getSignal("lock1");
    m_sigLock[1] = loader->getSignal("lock2");
    m_sigHead = loader->getSignal("head");
    m_sigLeftLoop = loader->getSignal("leftloop");
    m_sigRightLoop = loader->getSignal("rightloop");
    m_sigGnuAll = loader->getSignal("Gnu_all");
    m_sigGnu[0] = loader->getSignal("Gnug");
    m_sigGnu[1] = loader->getSignal("Gnun");
    m_sigGnu[2] = loader->getSignal("Gnuu");
    m_sigGnuOn[0] = loader->getSignal("gnug_on");
    m_sigGnuOn[1] = loader->getSignal("gnun_on");
    m_sigGnuOn[2] = loader->getSignal("gnuu_on");
    m_sigGnuOff[0] = loader->getSignal("gnug_off");
    m_sigGnuOff[1] = loader->getSignal("gnun_off");
    m_sigGnuOff[2] = loader->getSignal("gnuu_off");
    m_sigHurdAll = loader->getSignal("hurd_all");
    m_sigHurd[0] = loader->getSignal("hurdh");
    m_sigHurd[1] = loader->getSignal("hurdu");
    m_sigHurd[2] = loader->getSignal("hurdr");
    m_sigHurd[3] = loader->getSignal("hurdd");
    m_sigHurdOn[0] = loader->getSignal("hurdh_on");
    m_sigHurdOn[1] = loader->getSignal("hurdu_on");
    m_sigHurdOn[2] = loader->getSignal("hurdr_on");
    m_sigHurdOn[3] = loader->getSignal("hurdd_on");
    m_sigHurdOff[0] = loader->getSignal("hurdh_off");
    m_sigHurdOff[1] = loader->getSignal("hurdu_off");
    m_sigHurdOff[2] = loader->getSignal("hurdr_off");
    m_sigHurdOff[3] = loader->getSignal("hurdd_off");
    m_sigMachAll = loader->getSignal("mach_all");
    m_sigMach[0] = loader->getSignal("machm");
    m_sigMach[1] = loader->getSignal("macha");
    m_sigMach[2] = loader->getSignal("machc");
    m_sigMach[3] = loader->getSignal("machh");
    m_sigMachOn[0] = loader->getSignal("machm_on");
    m_sigMachOn[1] = loader->getSignal("macha_on");
    m_sigMachOn[2] = loader->getSignal("machc_on");
    m_sigMachOn[3] = loader->getSignal("machh_on");
    m_sigMachOff[0] = loader->getSignal("machm_off");
    m_sigMachOff[1] = loader->getSignal("macha_off");
    m_sigMachOff[2] = loader->getSignal("machc_off");
    m_sigMachOff[3] = loader->getSignal("machh_off");
    m_sigMultiplier[0] = loader->getSignal("multiplier1");
    m_sigMultiplier[1] = loader->getSignal("multiplier2");
    m_sigMultiplier[2] = loader->getSignal("multiplier3");
    m_sigMultiplier[3] = loader->getSignal("multiplier4");
    m_sigMultiplier[4] = loader->getSignal("multiplier5");
    m_sigMissionAll = loader->getSignal("mission_all");
    m_sigBigHurdOn[0] = loader->getSignal("big_hurd_1_on");
    m_sigBigHurdOn[1] = loader->getSignal("big_hurd_2_on");
    m_sigBigHurdOn[2] = loader->getSignal("big_hurd_3_on");
    m_sigBigHurdOn[3] = loader->getSignal("big_hurd_4_on");
    m_sigBigHurdOff[0] = loader->getSignal("big_hurd_1_off");
    m_sigBigHurdOff[1] = loader->getSignal("big_hurd_2_off");
    m_sigBigHurdOff[2] = loader->getSignal("big_hurd_3_off");
    m_sigBigHurdOff[3] = loader->getSignal("big_hurd_4_off");
    m_sigBigHurdBlink[0] = loader->getSignal("big_hurd_1_blink");
    m_sigBigHurdBlink[1] = loader->getSignal("big_hurd_2_blink");
    m_sigBigHurdBlink[2] = loader->getSignal("big_hurd_3_blink");
    m_sigBigHurdBlink[3] = loader->getSignal("big_hurd_4_blink");
    m_sigCave = loader->getSignal("cave");
    m_sigArrow[0] = loader->getSignal("arrow_1");
    m_sigArrow[1] = loader->getSignal("arrow_2a");
    m_sigArrow[2] = loader->getSignal("arrow_2b");
    m_sigArrow[3] = loader->getSignal("arrow_3");
    m_sigArrow[4] = loader->getSignal("arrow_4");
    m_sigArrow[5] = loader->getSignal("arrow_5");
    m_sigArrow[6] = loader->getSignal("arrow_6");
    m_sigArrow[7] = loader->getSignal("arrow_7");
    m_sigArrow[8] = loader->getSignal("arrow_8");
    m_sigArrowOn[0] = loader->getSignal("arrow_1_on");
    m_sigArrowOn[1] = loader->getSignal("arrow_2a_on");
    m_sigArrowOn[2] = loader->getSignal("arrow_2b_on");
    m_sigArrowOn[3] = loader->getSignal("arrow_3_on");
    m_sigArrowOn[4] = loader->getSignal("arrow_4_on");
    m_sigArrowOn[5] = loader->getSignal("arrow_5_on");
    m_sigArrowOn[6] = loader->getSignal("arrow_6_on");
    m_sigArrowOn[7] = loader->getSignal("arrow_7_on");
    m_sigArrowOn[8] = loader->getSignal("arrow_8_on");
    m_sigArrowOff[0] = loader->getSignal("arrow_1_off");
    m_sigArrowOff[1] = loader->getSignal("arrow_2a_off");
    m_sigArrowOff[2] = loader->getSignal("arrow_2b_off");
    m_sigArrowOff[3] = loader->getSignal("arrow_3_off");
    m_sigArrowOff[4] = loader->getSignal("arrow_4_off");
    m_sigArrowOff[5] = loader->getSignal("arrow_5_off");
    m_sigArrowOff[6] = loader->getSignal("arrow_6_off");
    m_sigArrowOff[7] = loader->getSignal("arrow_7_off");
    m_sigArrowOff[8] = loader->getSignal("arrow_8_off");
    m_sigArrowBlink[0] = loader->getSignal("arrow_1_blink");
    m_sigArrowBlink[1] = loader->getSignal("arrow_2a_blink");
    m_sigArrowBlink[2] = loader->getSignal("arrow_2b_blink");
    m_sigArrowBlink[3] = loader->getSignal("arrow_3_blink");
    m_sigArrowBlink[4] = loader->getSignal("arrow_4_blink");
    m_sigArrowBlink[5] = loader->getSignal("arrow_5_blink");
    m_sigArrowBlink[6] = loader->getSignal("arrow_6_blink");
    m_sigArrowBlink[7] = loader->getSignal("arrow_7_blink");
    m_sigArrowBlink[8] = loader->getSignal("arrow_8_blink");
    m_sigMissionCave = loader->getSignal("mission_cave");
    m_sigStopBalls = loader->getSignal("stop_balls");
    m_sigResumeBalls = loader->getSignal("resume_balls");

    this->clear();
  };
  ~HurdBehavior() {};
  
  void onTick() {
    EM_COUT("HurdBehavior::onTick", 0);
    Table * table = Table::getInstance();
    Score * score = table->getScore();
    EmAssert(score != NULL, "HurdBehavior::onTick score NULL");
    // launch ball
    string launch("launch");
    if (table->active() == 0 && 
	table->getCurrentBall() < MAX_BALL) {
      switch (table->getCurrentBall()) {
      case 0 :
	if (table->isBallDead(0) ) {
	  SendSignal( m_sigGameStart, 0, this->getParent(), NULL );
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
	if (table->isBallDead(0) ) {
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
	throw string("HurdBehavior::onTick() all balls busy");
      }
      EM_COUT("ModuleHurd::onTick() new ball", 1);
    }
    EM_COUT("HurdBehavior::onTick done", 0);
		
    if (m_iMission == MISSION_RAMP_ARROWS) {
      ++m_iRampTick;
      if (m_iRampTick >= 3000) {
	if (m_aArrow[4]) {
	  m_aArrow[4] = false;
	  SendSignal( m_sigArrowOff[5], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[4], 0, this->getParent(), NULL );
	  m_iRampTick = 0;
	} else if (m_aArrow[3]) {
	  m_aArrow[3] = false;
	  SendSignal( m_sigArrowOff[4], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[3], 0, this->getParent(), NULL );
	  m_iRampTick = 0;
	}
      }      
    }
    if (m_iMission == MISSION_FOURTH_ARROWS) {
      ++m_iRampTick;
      if (m_iRampTick >= 12000) {
	if (m_aArrow[7]) {
	  m_aArrow[7] = false;
	  SendSignal( m_sigArrowOff[8], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[7], 0, this->getParent(), NULL );
	  m_iRampTick = 0;
	} else if (m_aArrow[6]) {
	  m_aArrow[6] = false;
	  SendSignal( m_sigArrowOff[7], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[6], 0, this->getParent(), NULL );
	  m_iRampTick = 0;
	}
      }      
    }

    ++m_iMissionTick;
    if (m_iMissionTick >= MISSION_TEXT_TIMEOUT) {
      m_iMissionTick = 0;
      switch (m_iMission) {
      case MISSION_FIRST_BUMP: 
      case MISSION_SECOND_BUMP:
      case MISSION_THIRD_BUMP:
      case MISSION_FOURTH_BUMP:
      case MISSION_FIFTH_BUMP:
	score->setInfoText("hit bumpers", INFO_TEXT_TIME); break;
      case MISSION_FIRST_CAVE: 
      case MISSION_SECOND_CAVE:
      case MISSION_THIRD_CAVE:
      case MISSION_FOURTH_CAVE:
      case MISSION_FIFTH_CAVE:
	score->setInfoText("shoot cave to turn a square of hurd", INFO_TEXT_TIME); break; 
      case MISSION_ARROW:
	score->setInfoText("follow arrows", INFO_TEXT_TIME); break;
      case MISSION_ARROWS:
	score->setInfoText("follow arrows in the hurd logo", INFO_TEXT_TIME); break;
      case MISSION_RAMP_ARROWS:
	score->setInfoText("follow arrows, hurry up", INFO_TEXT_TIME); break;
      case MISSION_FOURTH_ARROWS:
	score->setInfoText("follow arrows, hurry up", INFO_TEXT_TIME); break;
      case MISSION_FIFTH_ALL:
	score->setInfoText("follow all arrows", INFO_TEXT_TIME); break;
      case MISSION_ALL:
	score->setInfoText("you did it, GNU is complete", INFO_TEXT_TIME); break;
      }
    }
  };
  
  void StdEmptyOnCollision() {};
  
  void StdOnSignal() {
    Table * table = Table::getInstance();
    Score * score = table->getScore();
    EmAssert(score != NULL, "HurdBehavior::StdOnSignal score NULL");
    //EM_COUT((int)em_signal, 1);
    
    OnSignal( PBL_SIG_RESET_ALL ) {
      this->clear();
    } 
    // ball dead
    ElseOnSignal( PBL_SIG_BALL_OFF ) {
      if (table->active() == 1) {
	// multiball is dead
	SendSignal( m_sigMultiballOff, 0, this->getParent(), NULL );
	m_bMultiBallOn = false;
      }
      if (table->active() == 0) {
	// no active ball
	SendSignal( m_sigAllBallsOff, 0, this->getParent(), NULL );
	if (m_bExtraBall) {
	  m_bExtraBall = false;
	} else if (table->getCurrentBall() < MAX_BALL) {
	  table->setCurrentBall(table->getCurrentBall()+1);
	  if (table->getCurrentBall() == MAX_BALL) {
	    SendSignal( PBL_SIG_GAME_OVER, 0, this->getParent(), NULL );
	    EM_COUT("game over", 1);
	  }
	}
      }
    } 
    // multiball
    ElseOnSignal( m_sigHead ) {
      score->setInfoText("shoot to lock ball", INFO_TEXT_TIME);
    }		
    ElseOnSignal( m_sigLock[0] ) {
      score->setInfoText("first ball locked", INFO_TEXT_TIME);
    }		
    ElseOnSignal( m_sigLock[1] ) {
      score->setInfoText("second ball locked", INFO_TEXT_TIME);
    }		
    ElseOnSignal( m_sigReleaseLock ) {
      score->setInfoText("multiball", INFO_TEXT_TIME);
      table->unLockBall(0);
      table->unLockBall(1);
      table->unLockBall(2);
      m_bMultiBallOn = true;
      score->addScore(10000);
    } 
    // bump
    ElseOnSignal(m_sigBump) {
      score->addScore(150 * m_iMultiplier);
      // missions
      if (m_iMission == MISSION_FIRST_BUMP) {
	++m_iBumpCounter;
	if (m_iBumpCounter == 10) {
	  m_iBumpCounter = 0;
	  m_iMission = MISSION_ARROW;
	  SendSignal( m_sigArrowBlink[0], 0, this->getParent(), NULL );
	  //cerr << "side arrow blink" << endl;
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	}
      }
      if (m_iMission == MISSION_SECOND_BUMP) {
	++m_iBumpCounter;
	if (m_iBumpCounter == 10) {
	  m_iBumpCounter = 0;
	  m_iMission = MISSION_ARROWS;
	  SendSignal( m_sigArrowBlink[1], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[2], 0, this->getParent(), NULL );
	  //cerr << "side arrows blink" << endl;
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	}
      }
      if (m_iMission == MISSION_THIRD_BUMP) {
	++m_iBumpCounter;
	if (m_iBumpCounter == 10) {
	  m_iBumpCounter = 0;
	  m_iMission = MISSION_RAMP_ARROWS;
	  SendSignal( m_sigArrowBlink[3], 0, this->getParent(), NULL );
	  //cerr << "side ramp arrows blink" << endl;
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	}
      }
      if (m_iMission == MISSION_FOURTH_BUMP) {
	++m_iBumpCounter;
	if (m_iBumpCounter == 10) {
	  m_iBumpCounter = 0;
	  m_iMission = MISSION_FOURTH_ARROWS;
	  SendSignal( m_sigArrowBlink[6], 0, this->getParent(), NULL );
	  //cerr << "right ramp arrows blink" << endl;
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	}
      }
      if (m_iMission == MISSION_FIFTH_BUMP) {
	++m_iBumpCounter;
	if (m_iBumpCounter == 10) {
	  m_iBumpCounter = 0;
	  m_iMission = MISSION_FIFTH_ALL;
	  SendSignal( m_sigArrowBlink[0], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[1], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[2], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[3], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[4], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[5], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[6], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[7], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[8], 0, this->getParent(), NULL );
	  //cerr << "right ramp arrows blink" << endl;
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	}
      }
    }
    // cave
    ElseOnSignal(m_sigCave) {
      if (m_iMission == MISSION_FIRST_CAVE) {
	SendSignal( m_sigBigHurdOn[0], 0, this->getParent(), NULL );
	score->setInfoText("first develop of hurd", INFO_TEXT_TIME);
	score->addScore(40000);
	m_iMission = MISSION_SECOND_BUMP;
	m_iMissionTick = MISSION_TEXT_TIMEOUT - INFO_TEXT_TIME;
      }
      if (m_iMission == MISSION_SECOND_CAVE) {
	SendSignal( m_sigBigHurdOn[1], 0, this->getParent(), NULL );
	score->setInfoText("second develop of hurd", INFO_TEXT_TIME);
	score->addScore(50000);
	m_iMission = MISSION_THIRD_BUMP;
	m_iMissionTick = MISSION_TEXT_TIMEOUT - INFO_TEXT_TIME;
      } 
      if (m_iMission == MISSION_THIRD_CAVE) {
	SendSignal( m_sigBigHurdOn[2], 0, this->getParent(), NULL );
	score->setInfoText("third develop of hurd", INFO_TEXT_TIME);
	score->addScore(60000);
	m_iMission = MISSION_FOURTH_BUMP;
	m_iMissionTick = MISSION_TEXT_TIMEOUT - INFO_TEXT_TIME;
      }
      if (m_iMission == MISSION_FOURTH_CAVE) {
	SendSignal( m_sigBigHurdOn[3], 0, this->getParent(), NULL );
	score->setInfoText("fourth develop of hurd", INFO_TEXT_TIME);
	score->addScore(70000);
	m_iMission = MISSION_FIFTH_BUMP;
	m_iMissionTick = MISSION_TEXT_TIMEOUT - INFO_TEXT_TIME;
      }
      if (m_iMission == MISSION_FIFTH_CAVE) {
	SendSignal( m_sigMissionAll, 0, this->getParent(), NULL );
	//SendSignal( m_sigStopBalls, 0, this->getParent(), NULL );
	//SendSignal( m_sigResumeBalls, 1000, this->getParent(), NULL );
	score->setInfoText("hurd is stable", INFO_TEXT_TIME);
	score->addScore(500000);
	m_iMission = MISSION_ALL;
	m_iMissionTick = MISSION_TEXT_TIMEOUT - INFO_TEXT_TIME;
      }
      score->addScore(1750);
    }
    // GNU
    ElseOnSignal(m_sigGnu[0]) {
      if (!m_aGnu[0]) {
	SendSignal(m_sigGnuOn[0], 0, this->getParent(), NULL);
	m_aGnu[0] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal(m_sigGnu[1]) {
      if (!m_aGnu[1]) {
	SendSignal(m_sigGnuOn[1], 0, this->getParent(), NULL);
	m_aGnu[1] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal(m_sigGnu[2]) {
      if (!m_aGnu[2]) {
	SendSignal(m_sigGnuOn[2], 0, this->getParent(), NULL);
	m_aGnu[2] = true;
      }
      score->addScore(50);
    } 
    // HURD
    ElseOnSignal( m_sigHurd[0] ) {
      if (m_aHurd[0]) {
	SendSignal(m_sigHurdOff[0], 0, this->getParent(), NULL);
	m_aHurd[0] = false;
      } else {
	SendSignal(m_sigHurdOn[0], 0, this->getParent(), NULL);
	m_aHurd[0] = true;
      }
      score->addScore(500);
    } 
    ElseOnSignal( m_sigHurd[1] ) {
      if (m_aHurd[1]) {
	SendSignal(m_sigHurdOff[1], 0, this->getParent(), NULL);
	m_aHurd[1] = false;
      } else {
	SendSignal(m_sigHurdOn[1], 0, this->getParent(), NULL);
	m_aHurd[1] = true;
      }
      score->addScore(500);
    } 
    ElseOnSignal( m_sigHurd[2] ) {
      if (m_aHurd[2]) {
	SendSignal(m_sigHurdOff[2], 0, this->getParent(), NULL);
	m_aHurd[2] = false;
      } else {
	SendSignal(m_sigHurdOn[2], 0, this->getParent(), NULL);
	m_aHurd[2] = true;
      }
      score->addScore(500);
    } 
    ElseOnSignal( m_sigHurd[3] ) {
      if (m_aHurd[3]) {
	SendSignal(m_sigHurdOff[3], 0, this->getParent(), NULL);
	m_aHurd[3] = false;
      } else {
	SendSignal(m_sigHurdOn[3], 0, this->getParent(), NULL);
	m_aHurd[3] = true;
      }
      score->addScore(500);
    } 
    // MACH
    ElseOnSignal( m_sigMach[0] ) {
      if (m_aMach[0]) {
	SendSignal(m_sigMachOff[0], 0, this->getParent(), NULL);
	m_aMach[0] = false;
      } else {
	SendSignal(m_sigMachOn[0], 0, this->getParent(), NULL);
	m_aMach[0] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal( m_sigMach[1] ) {
      if (m_aMach[1]) {
	SendSignal(m_sigMachOff[1], 0, this->getParent(), NULL);
	m_aMach[1] = false;
      } else {
	SendSignal(m_sigMachOn[1], 0, this->getParent(), NULL);
	m_aMach[1] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal( m_sigMach[2] ) {
      if (m_aMach[2]) {
	SendSignal(m_sigMachOff[2], 0, this->getParent(), NULL);
	m_aMach[2] = false;
      } else {
	SendSignal(m_sigMachOn[2], 0, this->getParent(), NULL);
	m_aMach[2] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal( m_sigMach[3] ) {
      if (m_aMach[3]) {
	SendSignal(m_sigMachOff[3], 0, this->getParent(), NULL);
	m_aMach[3] = false;
      } else {
	SendSignal(m_sigMachOn[3], 0, this->getParent(), NULL);
	m_aMach[3] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal( PBL_SIG_LEFTARM_ON ) {
      // MACH
      bool tmp = m_aMach[0];
      for (int a=0; a<3; ++a) {
	if (m_aMach[a] != m_aMach[a+1]) {
	  m_aMach[a] = m_aMach[a+1];
	  if (m_aMach[a]) SendSignal(m_sigMachOn[a], 0, this->getParent(), NULL);
	  else SendSignal(m_sigMachOff[a], 0, this->getParent(), NULL);
	} 
      } 
      if (m_aMach[3] != tmp) {
	m_aMach[3] = tmp;
	if (m_aMach[3]) SendSignal(m_sigMachOn[3], 0, this->getParent(), NULL);
	else SendSignal(m_sigMachOff[3], 0, this->getParent(), NULL);
      }
      // GNU
      tmp = m_aGnu[0];
      for (int aa=0; aa<2; ++aa) {
	if (m_aGnu[aa] != m_aGnu[aa+1]) {
	  m_aGnu[aa] = m_aGnu[aa+1];
	  if (m_aGnu[aa]) SendSignal(m_sigGnuOn[aa], 0, this->getParent(), NULL);
	  else SendSignal(m_sigGnuOff[aa], 0, this->getParent(), NULL);
	}
      } 
      if (m_aGnu[2] != tmp) {
	m_aGnu[2] = tmp;
	if (m_aGnu[2]) SendSignal(m_sigGnuOn[2], 0, this->getParent(), NULL);
	else SendSignal(m_sigGnuOff[2], 0, this->getParent(), NULL);
      }
    } 
    ElseOnSignal( PBL_SIG_RIGHTARM_ON ) {
      // MACH
      bool tmp = m_aMach[3];
      { for (int a=3; a>0; --a) {
	if (m_aMach[a] != m_aMach[a-1]) {
	  m_aMach[a] = m_aMach[a-1];
	  if (m_aMach[a]) SendSignal(m_sigMachOn[a], 0, this->getParent(), NULL);
	  else SendSignal(m_sigMachOff[a], 0, this->getParent(), NULL);
	}
      } }
      if (m_aMach[0] != tmp) {
	m_aMach[0] = tmp;
	if (m_aMach[0]) SendSignal(m_sigMachOn[0], 0, this->getParent(), NULL);
	else SendSignal(m_sigMachOff[0], 0, this->getParent(), NULL);
      }
      // GNU
      tmp = m_aGnu[2];
      { for (int a=2; a>0; --a) {
	if (m_aGnu[a] != m_aGnu[a-1]) {
	  m_aGnu[a] = m_aGnu[a-1];
	  if (m_aGnu[a]) SendSignal(m_sigGnuOn[a], 0, this->getParent(), NULL);
	  else SendSignal(m_sigGnuOff[a], 0, this->getParent(), NULL);
	}
      } }
      if (m_aGnu[0] != tmp) {
	m_aGnu[0] = tmp;
	if (m_aGnu[0]) SendSignal(m_sigGnuOn[0], 0, this->getParent(), NULL);
	else SendSignal(m_sigGnuOff[0], 0, this->getParent(), NULL);
      }
    } 
    ElseOnSignal(m_sigRightLoop) {
      if (m_bExtraBallWaiting) {
	SendSignal( m_sigExtraBall, 0, this->getParent(), NULL );
	m_bExtraBall = true;
	m_bExtraBallWaiting = false;
      }
      score->addScore(2500);
      if (m_iMission == MISSION_FOURTH_ARROWS) {
	m_iRampTick = 0;
	if (m_aArrow[7]) {
	  SendSignal( m_sigArrowOff[6], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOff[7], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOff[8], 0, this->getParent(), NULL );
	  SendSignal( m_sigBigHurdBlink[3], 0, this->getParent(), NULL );
	  m_iMission = MISSION_FOURTH_CAVE;
	  SendSignal( m_sigMissionCave, 0, this->getParent(), NULL );
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	  m_aArrow[6] = m_aArrow[7] = m_aArrow[8] = false;
	} else if (m_aArrow[6]) {
	  m_aArrow[7] = true;
	  SendSignal( m_sigArrowBlink[8], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOn[7], 0, this->getParent(), NULL );
	} else {
	  m_aArrow[6] = true;
	  SendSignal( m_sigArrowBlink[7], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOn[6], 0, this->getParent(), NULL );
	}
      }
      if (m_iMission == MISSION_FIFTH_ALL) {
	SendSignal( m_sigArrowOn[6], 0, this->getParent(), NULL );
	SendSignal( m_sigArrowOn[7], 0, this->getParent(), NULL );
	SendSignal( m_sigArrowOn[8], 0, this->getParent(), NULL );
	m_aArrow[6] = m_aArrow[7] = m_aArrow[8] = true;
      }
    } 
    ElseOnSignal(m_sigLeftLoop) {
      if (m_bMultiBallOn) {
	SendSignal( m_sigJackpot, 0, this->getParent(), NULL );
	score->addScore(100000);
      }
      score->addScore(2500);
      if (m_iMission == MISSION_RAMP_ARROWS) {
	m_iRampTick = 0;
	if (m_aArrow[4]) {
	  SendSignal( m_sigArrowOff[3], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOff[4], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOff[5], 0, this->getParent(), NULL );
	  SendSignal( m_sigBigHurdBlink[2], 0, this->getParent(), NULL );
	  m_iMission = MISSION_THIRD_CAVE;
	  SendSignal( m_sigMissionCave, 0, this->getParent(), NULL );
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	  m_aArrow[3] = m_aArrow[4] = m_aArrow[5] = false;
	} else if (m_aArrow[3]) {
	  m_aArrow[4] = true;
	  SendSignal( m_sigArrowBlink[5], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOn[4], 0, this->getParent(), NULL );
	} else {
	  m_aArrow[3] = true;
	  SendSignal( m_sigArrowBlink[4], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOn[3], 0, this->getParent(), NULL );
	}
      }
      if (m_iMission == MISSION_FIFTH_ALL) {
	SendSignal( m_sigArrowOn[3], 0, this->getParent(), NULL );
	SendSignal( m_sigArrowOn[4], 0, this->getParent(), NULL );
	SendSignal( m_sigArrowOn[5], 0, this->getParent(), NULL );
	m_aArrow[3] = m_aArrow[4] = m_aArrow[5] = true;
      }
    }
    // Arrows
    ElseOnSignal(m_sigArrow[1]) {
      if (m_iMission == MISSION_ARROWS || m_iMission == MISSION_FIFTH_ALL) {
	SendSignal( m_sigArrowOn[1], 0, this->getParent(), NULL );
	m_aArrow[1] = true;
      }
      score->addScore(750);
    }
    ElseOnSignal(m_sigArrow[2]) {
      if (m_iMission == MISSION_ARROWS || m_iMission == MISSION_FIFTH_ALL) {
	SendSignal( m_sigArrowOn[2], 0, this->getParent(), NULL );
	m_aArrow[2] = true;
      }
      score->addScore(750);
    }

    // Arrow
    ElseOnSignal(m_sigArrow[0]) {
      if (m_iMission == MISSION_ARROW || m_iMission == MISSION_FIFTH_ALL) {
	SendSignal( m_sigArrowOn[0], 0, this->getParent(), NULL );
	m_aArrow[0] = true;
      }
      score->addScore(750);
    }


    // Side arrow all
    if (m_aArrow[0] && m_iMission == MISSION_ARROW) {
      SendSignal( m_sigArrowOff[0], 0, this->getParent(), NULL );
      SendSignal( m_sigBigHurdBlink[0], 0, this->getParent(), NULL );
      m_aArrow[0] = false;
      m_iMission = MISSION_FIRST_CAVE;
      SendSignal( m_sigMissionCave, 0, this->getParent(), NULL );
      m_iMissionTick = MISSION_TEXT_TIMEOUT;
    }

    // Side arrows all
    if (m_aArrow[1] && m_aArrow[2] && m_iMission == MISSION_ARROWS) {
      SendSignal( m_sigArrowOff[1], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[2], 0, this->getParent(), NULL );
      SendSignal( m_sigBigHurdBlink[1], 0, this->getParent(), NULL );
      m_aArrow[1] = m_aArrow[2] = false;
      m_iMission = MISSION_SECOND_CAVE;
      SendSignal( m_sigMissionCave, 0, this->getParent(), NULL );
      m_iMissionTick = MISSION_TEXT_TIMEOUT;
    }
    
    // last mission all
    if (m_iMission == MISSION_FIFTH_ALL &&
	m_aArrow[0] && m_aArrow[1] && m_aArrow[2] && m_aArrow[3] && m_aArrow[4] && 
	m_aArrow[5] && m_aArrow[6] && m_aArrow[7] && m_aArrow[8]) {
      SendSignal( m_sigArrowOff[0], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[1], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[2], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[3], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[4], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[5], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[6], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[7], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[8], 0, this->getParent(), NULL );
      m_aArrow[0] = m_aArrow[1] = m_aArrow[2] = m_aArrow[3] = m_aArrow[4] =
	m_aArrow[5] = m_aArrow[6] = m_aArrow[7] = m_aArrow[8] = false;
      m_iMission = MISSION_FIFTH_CAVE;
      SendSignal( m_sigMissionCave, 0, this->getParent(), NULL );
    }

    // HURD all
    if (m_aHurd[0] && m_aHurd[1] && m_aHurd[2] && m_aHurd[3]) {
      SendSignal(m_sigHurdAll, 0, this->getParent(), NULL);
      m_aHurd[0] = m_aHurd[1] = m_aHurd[2] = m_aHurd[3] = false;
      m_bExtraBallWaiting = true;
      score->addScore(5000);
    }
    
    // GNU all
    if (m_aGnu[0] && m_aGnu[1] && m_aGnu[2]) {
      SendSignal(m_sigGnuAll, 0, this->getParent(), NULL);
      m_aGnu[0] = m_aGnu[1] = m_aGnu[2] = false;
      switch (m_iMultiplier) {
      case 1: 
	m_iMultiplier = 2;
	SendSignal(m_sigMultiplier[1], 0, this->getParent(), NULL);
	break;
      case 2: 
	m_iMultiplier = 3;
	SendSignal(m_sigMultiplier[2], 0, this->getParent(), NULL);
	break;
      case 3: 
	m_iMultiplier = 4;
	SendSignal(m_sigMultiplier[3], 0, this->getParent(), NULL);
	break;
      case 4: 
	m_iMultiplier = 5;
	SendSignal(m_sigMultiplier[4], 0, this->getParent(), NULL);
	break;
      }
      score->addScore(5000);
    }
    
    // MACH all
    if (m_aMach[0] && m_aMach[1] && m_aMach[2] && m_aMach[3]) {
      SendSignal(m_sigMachAll, 0, this->getParent(), NULL);
      m_aMach[0] = m_aMach[1] = m_aMach[2] = m_aMach[3] = false;
      score->addScore(5000);
    }
  }
  
  void clear() {
    m_iMultiplier = 1;
    m_aGnu[0] = false;
    m_aGnu[1] = false;
    m_aGnu[2] = false;
    m_aMach[0] = false;
    m_aMach[1] = false;
    m_aMach[2] = false;
    m_aMach[3] = false;
    m_aHurd[0] = false;
    m_aHurd[1] = false;
    m_aHurd[2] = false;
    m_aHurd[3] = false;
    m_aArrow[0] = false;
    m_aArrow[1] = false;
    m_aArrow[2] = false;
    m_aArrow[3] = false;
    m_aArrow[4] = false;
    m_aArrow[5] = false;
    m_aArrow[6] = false;
    m_aArrow[7] = false;
    m_aArrow[8] = false;
    m_bExtraBall = false;
    m_bExtraBallWaiting = false;
    m_bMultiBallOn = false;
    m_iMission = MISSION_FIRST_BUMP;
    //m_iMission = MISSION_FIFTH_BUMP;
    m_iMissionTextCounter = 0;
    m_iBumpCounter = 0;
    m_iRampTick = 0;
    m_iMissionTick = 0;
  };

private:
  int m_iMultiplier;
  int m_sigGameStart;
  int m_sigBump;
  int m_sigJackpot;
  int m_sigAllBallsOff;
  int m_sigExtraBall;
  int m_sigMultiballOff;
  int m_sigReleaseLock;
  int m_sigLock[2];
  int m_sigHead;
  int m_sigLeftLoop;
  int m_sigRightLoop;
  int m_sigGnuAll;
  int m_sigGnu[3];
  int m_sigGnuOn[3];
  int m_sigGnuOff[3];
  int m_sigHurdAll;
  int m_sigHurd[4];
  int m_sigHurdOn[4];
  int m_sigHurdOff[4];
  int m_sigMachAll;
  int m_sigMach[4];
  int m_sigMachOn[4];
  int m_sigMachOff[4];
  int m_sigMultiplier[5];
  int m_sigMissionAll;
  int m_sigBigHurdOn[4];
  int m_sigBigHurdOff[4];
  int m_sigBigHurdBlink[4];
  int m_sigCave;
  int m_sigArrow[9];
  int m_sigArrowOn[9];
  int m_sigArrowOff[9];
  int m_sigArrowBlink[9];
  int m_sigMissionCave;
  int m_sigStopBalls;
  int m_sigResumeBalls;
  bool m_aGnu[3];
  bool m_aMach[4];
  bool m_aHurd[4];
  bool m_aArrow[9];
  bool m_bExtraBall;
  bool m_bExtraBallWaiting;
  bool m_bMultiBallOn;
  int m_iMission;
  int m_iMissionTextCounter;
  int m_iBumpCounter;
  int m_iRampTick;
  int m_iMissionTick;
};

extern "C"  void * new_object_fct(void) {
  return new HurdBehavior();
}

