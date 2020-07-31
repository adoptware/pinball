/***************************************************************************
                          ModuleTux.cpp  -  description
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
#include "Table.h"
#include "Config.h"

#define MISSION_TEXT_TIMEOUT 6000
#define INFO_TEXT_TIME 600

enum {
  MISSION_FIRST_BUMP=1,
  MISSION_ARROWS,
  MISSION_FIRST_CAVE,
  MISSION_SECOND_BUMP,
  MISSION_SIDETARGETS,
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

class TuxBehavior : public Behavior {
public:
  TuxBehavior() : Behavior() {
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
    m_sigMultiplier[0] = loader->getSignal("multiplier1");
    m_sigMultiplier[1] = loader->getSignal("multiplier2");
    m_sigMultiplier[2] = loader->getSignal("multiplier3");
    m_sigMultiplier[3] = loader->getSignal("multiplier4");
    m_sigMultiplier[4] = loader->getSignal("multiplier5");
    m_sigMissionAll = loader->getSignal("mission_all");
    m_sigBigTuxOn[0] = loader->getSignal("big_tux_1_on");
    m_sigBigTuxOn[1] = loader->getSignal("big_tux_2_on");
    m_sigBigTuxOn[2] = loader->getSignal("big_tux_3_on");
    m_sigBigTuxOn[3] = loader->getSignal("big_tux_4_on");
    m_sigBigTuxOff[0] = loader->getSignal("big_tux_1_off");
    m_sigBigTuxOff[1] = loader->getSignal("big_tux_2_off");
    m_sigBigTuxOff[2] = loader->getSignal("big_tux_3_off");
    m_sigBigTuxOff[3] = loader->getSignal("big_tux_4_off");
    m_sigBigTuxBlink[0] = loader->getSignal("big_tux_1_blink");
    m_sigBigTuxBlink[1] = loader->getSignal("big_tux_2_blink");
    m_sigBigTuxBlink[2] = loader->getSignal("big_tux_3_blink");
    m_sigBigTuxBlink[3] = loader->getSignal("big_tux_4_blink");
    m_sigSideTarget[0] = loader->getSignal("side_target_1");
    m_sigSideTarget[1] = loader->getSignal("side_target_2");
    m_sigSideTarget[2] = loader->getSignal("side_target_3");
    m_sigSideTarget[3] = loader->getSignal("side_target_4");
    m_sigSideTargetOn[0] = loader->getSignal("side_target_1_on");
    m_sigSideTargetOn[1] = loader->getSignal("side_target_2_on");
    m_sigSideTargetOn[2] = loader->getSignal("side_target_3_on");
    m_sigSideTargetOn[3] = loader->getSignal("side_target_4_on");
    m_sigSideTargetOff[0] = loader->getSignal("side_target_1_off");
    m_sigSideTargetOff[1] = loader->getSignal("side_target_2_off");
    m_sigSideTargetOff[2] = loader->getSignal("side_target_3_off");
    m_sigSideTargetOff[3] = loader->getSignal("side_target_4_off");
    m_sigSideTargetBlink[0] = loader->getSignal("side_target_1_blink");
    m_sigSideTargetBlink[1] = loader->getSignal("side_target_2_blink");
    m_sigSideTargetBlink[2] = loader->getSignal("side_target_3_blink");
    m_sigSideTargetBlink[3] = loader->getSignal("side_target_4_blink");
    m_sigCave = loader->getSignal("cave");
    m_sigArrow[0] = loader->getSignal("arrow_1");
    m_sigArrow[1] = loader->getSignal("arrow_2");
    m_sigArrow[2] = loader->getSignal("arrow_3");
    m_sigArrow[3] = loader->getSignal("arrow_4");
    m_sigArrow[4] = loader->getSignal("arrow_5");
    m_sigArrow[5] = loader->getSignal("arrow_6");
    m_sigArrow[6] = loader->getSignal("arrow_7");
    m_sigArrow[7] = loader->getSignal("arrow_8");
    m_sigArrowOn[0] = loader->getSignal("arrow_1_on");
    m_sigArrowOn[1] = loader->getSignal("arrow_2_on");
    m_sigArrowOn[2] = loader->getSignal("arrow_3_on");
    m_sigArrowOn[3] = loader->getSignal("arrow_4_on");
    m_sigArrowOn[4] = loader->getSignal("arrow_5_on");
    m_sigArrowOn[5] = loader->getSignal("arrow_6_on");
    m_sigArrowOn[6] = loader->getSignal("arrow_7_on");
    m_sigArrowOn[7] = loader->getSignal("arrow_8_on");
    m_sigArrowOff[0] = loader->getSignal("arrow_1_off");
    m_sigArrowOff[1] = loader->getSignal("arrow_2_off");
    m_sigArrowOff[2] = loader->getSignal("arrow_3_off");
    m_sigArrowOff[3] = loader->getSignal("arrow_4_off");
    m_sigArrowOff[4] = loader->getSignal("arrow_5_off");
    m_sigArrowOff[5] = loader->getSignal("arrow_6_off");
    m_sigArrowOff[6] = loader->getSignal("arrow_7_off");
    m_sigArrowOff[7] = loader->getSignal("arrow_8_off");
    m_sigArrowBlink[0] = loader->getSignal("arrow_1_blink");
    m_sigArrowBlink[1] = loader->getSignal("arrow_2_blink");
    m_sigArrowBlink[2] = loader->getSignal("arrow_3_blink");
    m_sigArrowBlink[3] = loader->getSignal("arrow_4_blink");
    m_sigArrowBlink[4] = loader->getSignal("arrow_5_blink");
    m_sigArrowBlink[5] = loader->getSignal("arrow_6_blink");
    m_sigArrowBlink[6] = loader->getSignal("arrow_7_blink");
    m_sigArrowBlink[7] = loader->getSignal("arrow_8_blink");
    m_sigMissionCave = loader->getSignal("mission_cave");
    m_sigStopBalls = loader->getSignal("stop_balls");
    m_sigResumeBalls = loader->getSignal("resume_balls");

    this->clear();
  };
  ~TuxBehavior() {};
  
  void onTick() {
    EM_COUT("TuxBehavior::onTick", 0);
    Table * table = Table::getInstance();
    Score * score = table->getScore();
    EmAssert(score != NULL, "TuxBehavior::onTick score NULL");
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
	throw string("TuxBehavior::onTick() all balls busy");
      }
      EM_COUT("ModuleTux::onTick() new ball", 1);
    }
    EM_COUT("TuxBehavior::onTick done", 0);
		
    if (m_iMission == MISSION_RAMP_ARROWS) {
      ++m_iRampTick;
      if (m_iRampTick >= 2000) {
	if (m_aArrow[3]) {
	  m_aArrow[3] = false;
	  SendSignal( m_sigArrowOff[4], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[3], 0, this->getParent(), NULL );
	  m_iRampTick = 0;
	} else if (m_aArrow[2]) {
	  m_aArrow[2] = false;
	  SendSignal( m_sigArrowOff[3], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[2], 0, this->getParent(), NULL );
	  m_iRampTick = 0;
	}
      }      
    }
    if (m_iMission == MISSION_FOURTH_ARROWS) {
      ++m_iRampTick;
      if (m_iRampTick >= 2000) {
	if (m_aArrow[6]) {
	  m_aArrow[6] = false;
	  SendSignal( m_sigArrowOff[7], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[6], 0, this->getParent(), NULL );
	  m_iRampTick = 0;
	} else if (m_aArrow[5]) {
	  m_aArrow[5] = false;
	  SendSignal( m_sigArrowOff[6], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[5], 0, this->getParent(), NULL );
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
	score->setInfoText("shoot left cave to release tux", INFO_TEXT_TIME); break; 
      case MISSION_ARROWS:
	score->setInfoText("follow blue arrows", INFO_TEXT_TIME); break;
      case MISSION_SIDETARGETS:
	score->setInfoText("hit blue targets", INFO_TEXT_TIME); break;
      case MISSION_RAMP_ARROWS:
      case MISSION_FOURTH_ARROWS:
	score->setInfoText("follow blue arrows, hurry up", INFO_TEXT_TIME); break;
      case MISSION_FIFTH_ALL:
	score->setInfoText("hit all of them", INFO_TEXT_TIME); break;
      case MISSION_ALL:
	score->setInfoText("you did it, all penguins released!", INFO_TEXT_TIME); break;
      }
    }
  };
  
  void StdEmptyOnCollision() {};
  
  void StdOnSignal() {
    Table * table = Table::getInstance();
    Score * score = table->getScore();
    EmAssert(score != NULL, "TuxBehavior::StdOnSignal score NULL");
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
      score->setInfoText("shoot head to lock ball", INFO_TEXT_TIME);
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
      score->addScore(350 * m_iMultiplier);
      // missions
      if (m_iMission == MISSION_FIRST_BUMP) {
	++m_iBumpCounter;
	if (m_iBumpCounter == 10) {
	  m_iBumpCounter = 0;
	  m_iMission = MISSION_ARROWS;
	  SendSignal( m_sigArrowBlink[0], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[1], 0, this->getParent(), NULL );
	  //cerr << "side arrows blink" << endl;
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	}
      }
      if (m_iMission == MISSION_SECOND_BUMP) {
	++m_iBumpCounter;
	if (m_iBumpCounter == 10) {
	  m_iBumpCounter = 0;
	  m_iMission = MISSION_SIDETARGETS;
	  SendSignal( m_sigSideTargetBlink[0], 0, this->getParent(), NULL );
	  SendSignal( m_sigSideTargetBlink[1], 0, this->getParent(), NULL );
	  SendSignal( m_sigSideTargetBlink[2], 0, this->getParent(), NULL );
	  SendSignal( m_sigSideTargetBlink[3], 0, this->getParent(), NULL );
	  //cerr << "side targets blink" << endl;
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	}
      }
      if (m_iMission == MISSION_THIRD_BUMP) {
	++m_iBumpCounter;
	if (m_iBumpCounter == 10) {
	  m_iBumpCounter = 0;
	  m_iMission = MISSION_RAMP_ARROWS;
	  SendSignal( m_sigArrowBlink[2], 0, this->getParent(), NULL );
	  //cerr << "side ramp arrows blink" << endl;
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	}
      }
      if (m_iMission == MISSION_FOURTH_BUMP) {
	++m_iBumpCounter;
	if (m_iBumpCounter == 10) {
	  m_iBumpCounter = 0;
	  m_iMission = MISSION_FOURTH_ARROWS;
	  SendSignal( m_sigArrowBlink[5], 0, this->getParent(), NULL );
	  //cerr << "right ramp arrows blink" << endl;
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	}
      }
      if (m_iMission == MISSION_FIFTH_BUMP) {
	++m_iBumpCounter;
	if (m_iBumpCounter == 10) {
	  m_iBumpCounter = 0;
	  m_iMission = MISSION_FIFTH_ALL;
	  SendSignal( m_sigSideTargetBlink[0], 0, this->getParent(), NULL );
	  SendSignal( m_sigSideTargetBlink[1], 0, this->getParent(), NULL );
	  SendSignal( m_sigSideTargetBlink[2], 0, this->getParent(), NULL );
	  SendSignal( m_sigSideTargetBlink[3], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[0], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[1], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[2], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[3], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[4], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[5], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[6], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowBlink[7], 0, this->getParent(), NULL );
	  //cerr << "right ramp arrows blink" << endl;
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	}
      }
    }
    // cave
    ElseOnSignal(m_sigCave) {
      if (m_iMission == MISSION_FIRST_CAVE) {
	SendSignal( m_sigBigTuxOn[0], 0, this->getParent(), NULL );
	score->setInfoText("first tux saved", INFO_TEXT_TIME);
	score->addScore(40000);
	m_iMission = MISSION_SECOND_BUMP;
	m_iMissionTick = MISSION_TEXT_TIMEOUT - INFO_TEXT_TIME;
      }
      if (m_iMission == MISSION_SECOND_CAVE) {
	SendSignal( m_sigBigTuxOn[1], 0, this->getParent(), NULL );
	score->setInfoText("second tux saved", INFO_TEXT_TIME);
	score->addScore(50000);
	m_iMission = MISSION_THIRD_BUMP;
	m_iMissionTick = MISSION_TEXT_TIMEOUT - INFO_TEXT_TIME;
      } 
      if (m_iMission == MISSION_THIRD_CAVE) {
	SendSignal( m_sigBigTuxOn[2], 0, this->getParent(), NULL );
	score->setInfoText("third tux saved", INFO_TEXT_TIME);
	score->addScore(60000);
	m_iMission = MISSION_FOURTH_BUMP;
	m_iMissionTick = MISSION_TEXT_TIMEOUT - INFO_TEXT_TIME;
      }
      if (m_iMission == MISSION_FOURTH_CAVE) {
	SendSignal( m_sigBigTuxOn[3], 0, this->getParent(), NULL );
	score->setInfoText("fourth tux saved", INFO_TEXT_TIME);
	score->addScore(70000);
	m_iMission = MISSION_FIFTH_BUMP;
	m_iMissionTick = MISSION_TEXT_TIMEOUT - INFO_TEXT_TIME;
      }
      if (m_iMission == MISSION_FIFTH_CAVE) {
	SendSignal( m_sigMissionAll, 0, this->getParent(), NULL );
	//SendSignal( m_sigStopBalls, 0, this->getParent(), NULL );
	//SendSignal( m_sigResumeBalls, 1000, this->getParent(), NULL );
	score->setInfoText("all tuxes saved", INFO_TEXT_TIME);
	score->addScore(500000);
	m_iMission = MISSION_ALL;
	m_iMissionTick = MISSION_TEXT_TIMEOUT - INFO_TEXT_TIME;
      }
      score->addScore(1750);
    }
    // LINUX
    ElseOnSignal(m_sigLinux[0]) {
      if (!m_aLinux[0]) {
	SendSignal(m_sigLinuxOn[0], 0, this->getParent(), NULL);
	m_aLinux[0] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal(m_sigLinux[1]) {
      if (!m_aLinux[1]) {
	SendSignal(m_sigLinuxOn[1], 0, this->getParent(), NULL);
	m_aLinux[1] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal(m_sigLinux[2]) {
      if (!m_aLinux[2]) {
	SendSignal(m_sigLinuxOn[2], 0, this->getParent(), NULL);
	m_aLinux[2] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal(m_sigLinux[3]) {
      if (!m_aLinux[3]) {
	SendSignal(m_sigLinuxOn[3], 0, this->getParent(), NULL);
	m_aLinux[3] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal(m_sigLinux[4]) {
      if (!m_aLinux[4]) {
	SendSignal(m_sigLinuxOn[4], 0, this->getParent(), NULL);
	m_aLinux[4] = true;
      }
      score->addScore(50);
    } 
    // TUX
    ElseOnSignal(m_sigTux[0]) {
      if (!m_aTux[0]) {
	SendSignal(m_sigTuxOn[0], 0, this->getParent(), NULL);
	m_aTux[0] = true;
      }
      score->addScore(500);
    } 
    ElseOnSignal(m_sigTux[1]) {
      if (!m_aTux[1]) {
	SendSignal(m_sigTuxOn[1], 0, this->getParent(), NULL);
	m_aTux[1] = true;
      }
      score->addScore(500);
    } 
    ElseOnSignal(m_sigTux[2]) {
      if (!m_aTux[2]) {
	SendSignal(m_sigTuxOn[2], 0, this->getParent(), NULL);
	m_aTux[2] = true;
      }
      score->addScore(500);
    } 
    // BOOT
    ElseOnSignal( m_sigBoot[0] ) {
      if (m_aBoot[0]) {
	SendSignal(m_sigBootOff[0], 0, this->getParent(), NULL);
	m_aBoot[0] = false;
      } else {
	SendSignal(m_sigBootOn[0], 0, this->getParent(), NULL);
	m_aBoot[0] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal( m_sigBoot[1] ) {
      if (m_aBoot[1]) {
	SendSignal(m_sigBootOff[1], 0, this->getParent(), NULL);
	m_aBoot[1] = false;
      } else {
	SendSignal(m_sigBootOn[1], 0, this->getParent(), NULL);
	m_aBoot[1] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal( m_sigBoot[2] ) {
      if (m_aBoot[2]) {
	SendSignal(m_sigBootOff[2], 0, this->getParent(), NULL);
	m_aBoot[2] = false;
      } else {
	SendSignal(m_sigBootOn[2], 0, this->getParent(), NULL);
	m_aBoot[2] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal( m_sigBoot[3] ) {
      if (m_aBoot[3]) {
	SendSignal(m_sigBootOff[3], 0, this->getParent(), NULL);
	m_aBoot[3] = false;
      } else {
	SendSignal(m_sigBootOn[3], 0, this->getParent(), NULL);
	m_aBoot[3] = true;
      }
      score->addScore(50);
    } 
    ElseOnSignal( PBL_SIG_LEFTARM_ON ) {
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
      for (int aa=0; aa<4; ++aa) {
	if (m_aLinux[aa] != m_aLinux[aa+1]) {
	  m_aLinux[aa] = m_aLinux[aa+1];
	  if (m_aLinux[aa]) SendSignal(m_sigLinuxOn[aa], 0, this->getParent(), NULL);
	  else SendSignal(m_sigLinuxOff[aa], 0, this->getParent(), NULL);
	}
      } 
      if (m_aLinux[4] != tmp) {
	m_aLinux[4] = tmp;
	if (m_aLinux[4]) SendSignal(m_sigLinuxOn[4], 0, this->getParent(), NULL);
	else SendSignal(m_sigLinuxOff[4], 0, this->getParent(), NULL);
      }
    } 
    ElseOnSignal( PBL_SIG_RIGHTARM_ON ) {
      // BOOT
      bool tmp = m_aBoot[3];
      { for (int a=3; a>0; --a) {
	if (m_aBoot[a] != m_aBoot[a-1]) {
	  m_aBoot[a] = m_aBoot[a-1];
	  if (m_aBoot[a]) SendSignal(m_sigBootOn[a], 0, this->getParent(), NULL);
	  else SendSignal(m_sigBootOff[a], 0, this->getParent(), NULL);
	}
      } }
      if (m_aBoot[0] != tmp) {
	m_aBoot[0] = tmp;
	if (m_aBoot[0]) SendSignal(m_sigBootOn[0], 0, this->getParent(), NULL);
	else SendSignal(m_sigBootOff[0], 0, this->getParent(), NULL);
      }
      // LINUX
      tmp = m_aLinux[4];
      { for (int a=4; a>0; --a) {
	if (m_aLinux[a] != m_aLinux[a-1]) {
	  m_aLinux[a] = m_aLinux[a-1];
	  if (m_aLinux[a]) SendSignal(m_sigLinuxOn[a], 0, this->getParent(), NULL);
	  else SendSignal(m_sigLinuxOff[a], 0, this->getParent(), NULL);
	}
      } }
      if (m_aLinux[0] != tmp) {
	m_aLinux[0] = tmp;
	if (m_aLinux[0]) SendSignal(m_sigLinuxOn[0], 0, this->getParent(), NULL);
	else SendSignal(m_sigLinuxOff[0], 0, this->getParent(), NULL);
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
	if (m_aArrow[6]) {
	  SendSignal( m_sigArrowOff[5], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOff[6], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOff[7], 0, this->getParent(), NULL );
	  SendSignal( m_sigBigTuxBlink[3], 0, this->getParent(), NULL );
	  m_iMission = MISSION_FOURTH_CAVE;
	  SendSignal( m_sigMissionCave, 0, this->getParent(), NULL );
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	  m_aArrow[5] = m_aArrow[6] = m_aArrow[7] = false;
	} else if (m_aArrow[5]) {
	  m_aArrow[6] = true;
	  SendSignal( m_sigArrowBlink[7], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOn[6], 0, this->getParent(), NULL );
	} else {
	  m_aArrow[5] = true;
	  SendSignal( m_sigArrowBlink[6], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOn[5], 0, this->getParent(), NULL );
	}
      }
      if (m_iMission == MISSION_FIFTH_ALL) {
	SendSignal( m_sigArrowOn[5], 0, this->getParent(), NULL );
	SendSignal( m_sigArrowOn[6], 0, this->getParent(), NULL );
	SendSignal( m_sigArrowOn[7], 0, this->getParent(), NULL );
	m_aArrow[5] = m_aArrow[6] = m_aArrow[7] = true;
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
	if (m_aArrow[3]) {
	  SendSignal( m_sigArrowOff[2], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOff[3], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOff[4], 0, this->getParent(), NULL );
	  SendSignal( m_sigBigTuxBlink[2], 0, this->getParent(), NULL );
	  m_iMission = MISSION_THIRD_CAVE;
	  SendSignal( m_sigMissionCave, 0, this->getParent(), NULL );
	  m_iMissionTick = MISSION_TEXT_TIMEOUT;
	  m_aArrow[2] = m_aArrow[3] = m_aArrow[4] = false;
	} else if (m_aArrow[2]) {
	  m_aArrow[3] = true;
	  SendSignal( m_sigArrowBlink[4], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOn[3], 0, this->getParent(), NULL );
	} else {
	  m_aArrow[2] = true;
	  SendSignal( m_sigArrowBlink[3], 0, this->getParent(), NULL );
	  SendSignal( m_sigArrowOn[2], 0, this->getParent(), NULL );
	}
      }
      if (m_iMission == MISSION_FIFTH_ALL) {
	SendSignal( m_sigArrowOn[2], 0, this->getParent(), NULL );
	SendSignal( m_sigArrowOn[3], 0, this->getParent(), NULL );
	SendSignal( m_sigArrowOn[4], 0, this->getParent(), NULL );
	m_aArrow[2] = m_aArrow[3] = m_aArrow[4] = true;
      }
    }
    // Side Targets
    ElseOnSignal(m_sigSideTarget[0]) {
      if (m_iMission == MISSION_SIDETARGETS || m_iMission == MISSION_FIFTH_ALL) {
	m_aSideTarget[0] = true;
      }
      score->addScore(350*m_iMultiplier);
    }
    ElseOnSignal(m_sigSideTarget[1]) {
      if (m_iMission == MISSION_SIDETARGETS || m_iMission == MISSION_FIFTH_ALL) {
	m_aSideTarget[1] = true;
      }
      score->addScore(350*m_iMultiplier);
    }
    ElseOnSignal(m_sigSideTarget[2]) {
      if (m_iMission == MISSION_SIDETARGETS || m_iMission == MISSION_FIFTH_ALL) {
	m_aSideTarget[2] = true;
      }
      score->addScore(350*m_iMultiplier);
    }
    ElseOnSignal(m_sigSideTarget[3]) {
      if (m_iMission == MISSION_SIDETARGETS || m_iMission == MISSION_FIFTH_ALL) {
	m_aSideTarget[3] = true;
      }
      score->addScore(350*m_iMultiplier);
    }
    // Arrows
    ElseOnSignal(m_sigArrow[0]) {
      if (m_iMission == MISSION_ARROWS || m_iMission == MISSION_FIFTH_ALL) {
	SendSignal( m_sigArrowOn[0], 0, this->getParent(), NULL );
	m_aArrow[0] = true;
      }
      score->addScore(750);
    }
    ElseOnSignal(m_sigArrow[1]) {
      if (m_iMission == MISSION_ARROWS || m_iMission == MISSION_FIFTH_ALL) {
	SendSignal( m_sigArrowOn[1], 0, this->getParent(), NULL );
	m_aArrow[1] = true;
      }
      score->addScore(750);
    }

    // Side arrows all
    if (m_aArrow[0] && m_aArrow[1] && m_iMission == MISSION_ARROWS) {
      SendSignal( m_sigArrowOff[0], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[1], 0, this->getParent(), NULL );
      SendSignal( m_sigBigTuxBlink[0], 0, this->getParent(), NULL );
      m_aArrow[0] = m_aArrow[1] = false;
      m_iMission = MISSION_FIRST_CAVE;
      SendSignal( m_sigMissionCave, 0, this->getParent(), NULL );
      m_iMissionTick = MISSION_TEXT_TIMEOUT;
    }

    // Side targets all
    if (m_aSideTarget[0] && m_aSideTarget[1] && m_aSideTarget[2] && m_aSideTarget[3] &&
	m_iMission == MISSION_SIDETARGETS) {
      SendSignal( m_sigSideTargetOff[0], 0, this->getParent(), NULL );
      SendSignal( m_sigSideTargetOff[1], 0, this->getParent(), NULL );
      SendSignal( m_sigSideTargetOff[2], 0, this->getParent(), NULL );
      SendSignal( m_sigSideTargetOff[3], 0, this->getParent(), NULL );
      SendSignal( m_sigBigTuxBlink[1], 0, this->getParent(), NULL );
      m_aSideTarget[0] = m_aSideTarget[1] = m_aSideTarget[2] = m_aSideTarget[3] = false;
      m_iMission = MISSION_SECOND_CAVE;
      SendSignal( m_sigMissionCave, 0, this->getParent(), NULL );
      m_iMissionTick = MISSION_TEXT_TIMEOUT;
    }
    
    // last mission all
    if (m_iMission == MISSION_FIFTH_ALL &&
	m_aSideTarget[0] && m_aSideTarget[1] && m_aSideTarget[2] && m_aSideTarget[3] &&
	m_aArrow[0] && m_aArrow[1] && m_aArrow[2] && m_aArrow[3] && 
	m_aArrow[4] && m_aArrow[5] && m_aArrow[6] && m_aArrow[7]) {
      SendSignal( m_sigSideTargetOff[0], 0, this->getParent(), NULL );
      SendSignal( m_sigSideTargetOff[1], 0, this->getParent(), NULL );
      SendSignal( m_sigSideTargetOff[2], 0, this->getParent(), NULL );
      SendSignal( m_sigSideTargetOff[3], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[0], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[1], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[2], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[3], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[4], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[5], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[6], 0, this->getParent(), NULL );
      SendSignal( m_sigArrowOff[7], 0, this->getParent(), NULL );
      m_aSideTarget[0] = m_aSideTarget[1] = m_aSideTarget[2] = m_aSideTarget[3] = false;
      m_aArrow[0] = m_aArrow[1] = m_aArrow[2] = m_aArrow[3] =
	m_aArrow[4] = m_aArrow[5] = m_aArrow[6] = m_aArrow[7] = false;
      m_iMission = MISSION_FIFTH_CAVE;
      SendSignal( m_sigMissionCave, 0, this->getParent(), NULL );
    }

    // TUX all
    if (m_aTux[0] && m_aTux[1] && m_aTux[2]) {
      SendSignal(m_sigTuxAll, 0, this->getParent(), NULL);
      m_aTux[0] = m_aTux[1] = m_aTux[2] = false;
      m_bExtraBallWaiting = true;
      score->addScore(5000);
    }
    
    // LINUX all
    if (m_aLinux[0] && m_aLinux[1] && m_aLinux[2] && m_aLinux[3] && m_aLinux[4]) {
      SendSignal(m_sigLinuxAll, 0, this->getParent(), NULL);
      m_aLinux[0] = m_aLinux[1] = m_aLinux[2] = m_aLinux[3] = m_aLinux[4] = false;
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
    
    // BOOT all
    if (m_aBoot[0] && m_aBoot[1] && m_aBoot[2] && m_aBoot[3]) {
      SendSignal(m_sigBootAll, 0, this->getParent(), NULL);
      m_aBoot[0] = m_aBoot[1] = m_aBoot[2] = m_aBoot[3] = false;
      score->addScore(5000);
    }
  }
  
  void clear() {
    m_iMultiplier = 1;
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
    m_aSideTarget[0] = false;
    m_aSideTarget[1] = false;
    m_aSideTarget[2] = false;
    m_aSideTarget[3] = false;
    m_aArrow[0] = false;
    m_aArrow[1] = false;
    m_aArrow[2] = false;
    m_aArrow[3] = false;
    m_aArrow[4] = false;
    m_aArrow[5] = false;
    m_aArrow[6] = false;
    m_aArrow[7] = false;
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
  int m_sigMultiplier[5];
  int m_sigMissionAll;
  int m_sigBigTuxOn[4];
  int m_sigBigTuxOff[4];
  int m_sigBigTuxBlink[4];
  int m_sigSideTarget[4];
  int m_sigSideTargetOn[4];
  int m_sigSideTargetOff[4];
  int m_sigSideTargetBlink[4];
  int m_sigCave;
  int m_sigArrow[8];
  int m_sigArrowOn[8];
  int m_sigArrowOff[8];
  int m_sigArrowBlink[8];
  int m_sigMissionCave;
  int m_sigStopBalls;
  int m_sigResumeBalls;
  bool m_aLinux[5];
  bool m_aBoot[4];
  bool m_aTux[3];
  bool m_aSideTarget[4];
  bool m_aArrow[8];
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
  return new TuxBehavior();
}

