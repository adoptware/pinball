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

class TuxBehavior : public Behavior {
public:
  TuxBehavior() : Behavior() {
    Loader * loader = Loader::getInstance();
    // init signals
    m_sigGameStart = loader->getSignal("game_start");
    m_sigBump = loader->getSignal("bump");
    m_sigJackpot = loader->getSignal("jackpot");
    m_sigExtraBall = loader->getSignal("extraball");
    m_sigAllBallsOff = loader->getSignal("allballs_off");
    m_sigMultiballOff = loader->getSignal("multiball_off");
    m_sigReleaseLock = loader->getSignal("releaselock");
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
	//&& Keyboard::isKeyDown(Config::getInstance()->getKey(launch))) {
      switch (table->getCurrentBall()) {
      case 0 :
	if (table->isBallDead(0) ) {
	  SendSignal( m_sigGameStart, 0, this->getParent(), NULL );
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
	if (table->isBallDead(0) ) {
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
	throw string("TuxBehavior::onTick() all balls busy");
      }
      EM_COUT("ModuleTux::onTick() new ball", 1);
    }
    EM_COUT("TuxBehavior::onTick done", 0);
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
    ElseOnSignal( m_sigReleaseLock ) {
      table->unLockBall(0);
      table->unLockBall(1);
      table->unLockBall(2);
      m_bMultiBallOn = false;
      score->addScore(10000);
    } 
      // bump
    ElseOnSignal(m_sigBump) {
      score->addScore(450 * m_iMultiplier);
      score->setInfoText("bump", 200);
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
    } 
    ElseOnSignal( PBL_SIG_RIGHTARM_ON ) {
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
    } 
    ElseOnSignal(m_sigRightLoop) {
      if (m_bExtraBallWaiting) {
	SendSignal( m_sigExtraBall, 0, this->getParent(), NULL );
	m_bExtraBall = true;
	m_bExtraBallWaiting = false;
      }
      score->addScore(1500);
    } 
    ElseOnSignal(m_sigLeftLoop) {
      if (m_bMultiBallOn) {
	SendSignal( m_sigJackpot, 0, this->getParent(), NULL );
	score->addScore(100000);
      }
      score->addScore(1500);
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
    m_bExtraBall = false;
    m_bExtraBallWaiting = false;
    m_bMultiBallOn = false;
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
  int m_sigMultiplier[4];
  bool m_aLinux[5];
  bool m_aBoot[4];
  bool m_aTux[3];
  bool m_bExtraBall;
  bool m_bExtraBallWaiting;
  bool m_bMultiBallOn;
};

extern "C"  void * new_object_fct(void) {
  return new TuxBehavior();
}

