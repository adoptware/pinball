//#ident "$Id: ModuleGnu.cpp,v 1.8 2003/07/16 20:02:04 rzr Exp $"
/***************************************************************************
                          Score.cpp  -  description
                             -------------------
    begin                : Fri Jan 26 2001
    copyright            : (C) 2001 by Henrik Enqvist
                           (C) 2013 Ben Asselstine
    email                : henqvist@excite.com


    ========================= Modifications =========================

        Apr. 6, 2017:
            Send the "game_start" signal. (c30zD)

***************************************************************************/

#include "Private.h"
//#include "Keyboard.h"
#include "Config.h"
#include "Behavior.h"
#include "Group.h"
#include "Pinball.h"
#include "Loader.h"
#include "StateMachine.h"
#include "Table.h"
#include "Score.h"
#include "BallGroup.h"
#include "Shape3D.h"

/*
 *
 * 4 simultaneous missions.
 * 1. fill in GNU at the top.  It triggers the left and right lower shoot 
 * objects that save the ball from dying.
 * 2. knock down the letters COPY LEFT.  this makes the 0, 1, 2, 3 light go up.
 * 3. knock down the letters GPL.  this makes the v1, v2, v3 light up.
 * 4. fill in FREE at the bottom.  when both balls are locked in the spinners, 
 * it triggers multiball.
 *
 * when the 0, 1, 2, 3 lights are all lit up, the freedom target lights up.
 * upon hitting this target, a cylinder pops up in between the flippers.  the
 * cylinder stays up for the remainder of the game.
 *
 * when the v1, v2, v3 lights are all lit up, the free ball target lights up.
 * upon hitting this target, a free ball is awarded.
 *
 * it is possible to put a ball behind the top right flipper, and it goes 
 * back down to the plunger area.  doing this scores some points, but that's
 * all.
 *
 * strategy:
 * getting a free ball is fairly easy.  just plunger the ball barely up out
 * of the shoot, and it will end up on the flipper.  from there it's pretty
 * easy to hit the GPL letters (mission 3) if there isn't a ball in the spinner.
 * it's especially easy to do this after a multiball when the spinners won't
 * capture a ball.
 *
 * repeatedly complete mission 1 to make the FREE letters easily attainable.
 * then go for mission 4 multiball.
 * then mission 2 COPYLEFT targets should go down pretty fast during multiball.
 *
 * lastly, the free ball target is impossible to directly hit if the cylinder has popped up between the flippers.  an indirect hit is the only way.
 * if you can do this repeatedly, getting free balls will be REALLY easy.
 */
class GnuBehavior : public Behavior {
public:
  GnuBehavior() : Behavior() {
    char name[32];
    //this->clear();
    Loader * loader = Loader::getInstance();
    m_sigBump = loader->getSignal("bump");
    int i = 0;
    for (const char *p = "copy"; *p != '\0'; ++p) {
      snprintf (name, sizeof (name), "knockdown_copy_%c", *p);
      m_sigKnockdown[i][0] = loader->getSignal(name);
      snprintf (name, sizeof (name), "knockup_copy_%c", *p);
      m_sigKnockdown[i][1] = loader->getSignal(name);
      snprintf (name, sizeof (name), "knockup_copy_%c_going_down", *p);
      m_sigKnocking[i][0] = loader->getSignal(name);
      snprintf (name, sizeof (name), "knockup_copy_%c_going_up", *p);
      m_sigKnocking[i][1] = loader->getSignal(name);
      i++;
    }
    i = 0;
    for (const char *p = "left"; *p != '\0'; ++p) {
      snprintf (name, sizeof (name), "knockdown_left_%c", *p);
      m_sigKnockdown[i+4][0] = loader->getSignal(name);
      snprintf (name, sizeof (name), "knockup_left_%c", *p);
      m_sigKnockdown[i+4][1] = loader->getSignal(name);
      snprintf (name, sizeof (name), "knockup_left_%c_going_down", *p);
      m_sigKnocking[i+4][0] = loader->getSignal(name);
      snprintf (name, sizeof (name), "knockup_left_%c_going_up", *p);
      m_sigKnocking[i+4][1] = loader->getSignal(name);
      i++;
    }
    m_sigCopyleftKnockdownReset = loader->getSignal("knockdown_copyleft_reset");
    i = 0;
    for (const char *p = "gpl"; *p != '\0'; ++p) {
      snprintf (name, sizeof (name), "knockdown_gpl_%c", *p);
      m_sigKnockdown[i+8][0] = loader->getSignal(name);
      snprintf (name, sizeof (name), "knockup_gpl_%c", *p);
      m_sigKnockdown[i+8][1] = loader->getSignal(name);
      snprintf (name, sizeof (name), "knockup_gpl_%c_going_down", *p);
      m_sigKnocking[i+8][0] = loader->getSignal(name);
      snprintf (name, sizeof (name), "knockup_gpl_%c_going_up", *p);
      m_sigKnocking[i+8][1] = loader->getSignal(name);
      i++;
    }
    m_sigGplKnockdownReset = loader->getSignal("knockdown_gpl_reset");
    m_sigGnuAll = loader->getSignal("gnu_all");
    m_sigGnu[0] = loader->getSignal("gnu_g");
    m_sigGnuOn[0] = loader->getSignal("gnu_g_on");
    m_sigGnuOff[0] = loader->getSignal("gnu_g_off");
    m_sigGnu[1] = loader->getSignal("gnu_n");
    m_sigGnuOn[1] = loader->getSignal("gnu_n_on");
    m_sigGnuOff[1] = loader->getSignal("gnu_n_off");
    m_sigGnu[2] = loader->getSignal("gnu_u");
    m_sigGnuOn[2] = loader->getSignal("gnu_u_on");
    m_sigGnuOff[2] = loader->getSignal("gnu_u_off");
    m_sigFreeAll = loader->getSignal("free_all");
    m_sigFree[0] = loader->getSignal("free_f");
    m_sigFreeOn[0] = loader->getSignal("free_f_on");
    m_sigFreeOff[0] = loader->getSignal("free_f_off");
    m_sigFree[1] = loader->getSignal("free_r");
    m_sigFreeOn[1] = loader->getSignal("free_r_on");
    m_sigFreeOff[1] = loader->getSignal("free_r_off");
    m_sigFree[2] = loader->getSignal("free_e");
    m_sigFreeOn[2] = loader->getSignal("free_e_on");
    m_sigFreeOff[2] = loader->getSignal("free_e_off");
    m_sigFree[3] = loader->getSignal("free_ee");
    m_sigFreeOn[3] = loader->getSignal("free_ee_on");
    m_sigFreeOff[3] = loader->getSignal("free_ee_off");
    m_sigCopyleftAll = loader->getSignal("copyleft_all");
    m_sigCopyleft[0] = loader->getSignal("copy_c");
    m_sigCopyleftOn[0] = loader->getSignal("copy_c_on");
    m_sigCopyleftOff[0] = loader->getSignal("copy_c_off");
    m_sigCopyleft[1] = loader->getSignal("copy_o");
    m_sigCopyleftOn[1] = loader->getSignal("copy_o_on");
    m_sigCopyleftOff[1] = loader->getSignal("copy_o_off");
    m_sigCopyleft[2] = loader->getSignal("copy_p");
    m_sigCopyleftOn[2] = loader->getSignal("copy_p_on");
    m_sigCopyleftOff[2] = loader->getSignal("copy_p_off");
    m_sigCopyleft[3] = loader->getSignal("copy_y");
    m_sigCopyleftOn[3] = loader->getSignal("copy_y_on");
    m_sigCopyleftOff[3] = loader->getSignal("copy_y_off");
    m_sigCopyleft[4] = loader->getSignal("left_l");
    m_sigCopyleftOn[4] = loader->getSignal("left_l_on");
    m_sigCopyleftOff[4] = loader->getSignal("left_l_off");
    m_sigCopyleft[5] = loader->getSignal("left_e");
    m_sigCopyleftOn[5] = loader->getSignal("left_e_on");
    m_sigCopyleftOff[5] = loader->getSignal("left_e_off");
    m_sigCopyleft[6] = loader->getSignal("left_f");
    m_sigCopyleftOn[6] = loader->getSignal("left_f_on");
    m_sigCopyleftOff[6] = loader->getSignal("left_f_off");
    m_sigCopyleft[7] = loader->getSignal("left_t");
    m_sigCopyleftOn[7] = loader->getSignal("left_t_on");
    m_sigCopyleftOff[7] = loader->getSignal("left_t_off");
    m_sigGplAll = loader->getSignal("gpl_all");
    m_sigGpl[0] = loader->getSignal("gpl_g");
    m_sigGplOn[0] = loader->getSignal("gpl_g_on");
    m_sigGplOff[0] = loader->getSignal("gpl_g_off");
    m_sigGpl[1] = loader->getSignal("gpl_p");
    m_sigGplOn[1] = loader->getSignal("gpl_p_on");
    m_sigGplOff[1] = loader->getSignal("gpl_p_off");
    m_sigGpl[2] = loader->getSignal("gpl_l");
    m_sigGplOn[2] = loader->getSignal("gpl_l_on");
    m_sigGplOff[2] = loader->getSignal("gpl_l_off");
    for (i = 0; i < 6; i++) {
      snprintf (name, sizeof (name), "arrow_%d_on", i+1);
      m_sigArrowOn[i] = loader->getSignal(name);
      snprintf (name, sizeof (name), "arrow_%d_off", i+1);
      m_sigArrowOff[i] = loader->getSignal(name);
      snprintf (name, sizeof (name), "arrow_%d_blink", i+1);
      m_sigArrowBlink[i] = loader->getSignal(name);
    }
    m_sigMidStopperBreached = loader->getSignal("mid_stopper_breached");
    m_sigSideStopperBreached = loader->getSignal("side_stopper_breached");
    m_sigBottomStopperBreached = loader->getSignal("bottom_stopper_breached");
    m_sig0123All = loader->getSignal("0123_all");
    m_sig0123On[0] = loader->getSignal("0123_0_on");
    m_sig0123Off[0] = loader->getSignal("0123_0_off");
    m_sig0123Announce[0] = loader->getSignal("0123_0_announce");
    m_sig0123On[1] = loader->getSignal("0123_1_on");
    m_sig0123Off[1] = loader->getSignal("0123_1_off");
    m_sig0123Announce[1] = loader->getSignal("0123_1_announce");
    m_sig0123On[2] = loader->getSignal("0123_2_on");
    m_sig0123Off[2] = loader->getSignal("0123_2_off");
    m_sig0123Announce[2] = loader->getSignal("0123_2_announce");
    m_sig0123On[3] = loader->getSignal("0123_3_on");
    m_sig0123Off[3] = loader->getSignal("0123_3_off");
    m_sig0123Announce[3] = loader->getSignal("0123_3_announce");
    m_sigv123All = loader->getSignal("v123_all");
    m_sigv123On[0] = loader->getSignal("v123_1_on");
    m_sigv123Off[0] = loader->getSignal("v123_1_off");
    m_sigv123Announce[0] = loader->getSignal("v123_1_announce");
    m_sigv123On[1] = loader->getSignal("v123_2_on");
    m_sigv123Off[1] = loader->getSignal("v123_2_off");
    m_sigv123Announce[1] = loader->getSignal("v123_2_announce");
    m_sigv123On[2] = loader->getSignal("v123_3_on");
    m_sigv123Off[2] = loader->getSignal("v123_3_off");
    m_sigv123Announce[2] = loader->getSignal("v123_3_announce");
    m_sigv123On[3] = loader->getSignal("v123_freeball_on");
    m_sigv123Off[3] = loader->getSignal("v123_freeball_off");
    m_sigv123Announce[3] = loader->getSignal("v123_freeball_announce");
    m_sigFreedomTargetAnnounce = loader->getSignal("freedom_target_announce");
    m_sigFreeBallTargetAnnounce = loader->getSignal("freeball_target_announce");
    for (i = 0; i < 6; i++) {
      snprintf (name, sizeof (name), "lock_arrow_%d_on", i+1);
      m_sigLockArrowOn[i][0] = loader->getSignal(name);
      snprintf (name, sizeof (name), "lock_arrow_%d_off", i+1);
      m_sigLockArrowOff[i][0] = loader->getSignal(name);
      snprintf (name, sizeof (name), "lock_arrow_%d_blink", i+1);
      m_sigLockArrowBlink[i][0] = loader->getSignal(name);
      snprintf (name, sizeof (name), "lock_arrow_%d_crazyblink", i+1);
      m_sigLockArrowCrazyBlink[i][0] = loader->getSignal(name);
    }
    for (i = 0; i < 6; i++) {
      snprintf (name, sizeof (name), "lock_arrow_%d_on", i+7);
      m_sigLockArrowOn[i][1] = loader->getSignal(name);
      snprintf (name, sizeof (name), "lock_arrow_%d_off", i+7);
      m_sigLockArrowOff[i][1] = loader->getSignal(name);
      snprintf (name, sizeof (name), "lock_arrow_%d_blink", i+7);
      m_sigLockArrowBlink[i][1] = loader->getSignal(name);
      snprintf (name, sizeof (name), "lock_arrow_%d_crazyblink", i+7);
      m_sigLockArrowCrazyBlink[i][1] = loader->getSignal(name);
    }
    for (i = 0; i < 14; i++) {
      snprintf (name, sizeof (name), "light_%03d_on", i+1);
      m_sigLightOn[i] = loader->getSignal(name);
      snprintf (name, sizeof (name), "light_%03d_off", i+1);
      m_sigLightOff[i] = loader->getSignal(name);
      snprintf (name, sizeof (name), "light_%03d_blink", i+1);
      m_sigLightBlink[i] = loader->getSignal(name);
      snprintf (name, sizeof (name), "light_%03d_crazyblink", i+1);
      m_sigLightCrazyBlink[i] = loader->getSignal(name);
    }
    m_sigLock[0] = loader->getSignal("lock1");
    m_sigLock[1] = loader->getSignal("lock2");
    m_sigLockRelease = loader->getSignal("releaselock");
    m_sigSensitizeLockOne = loader->getSignal("sensitize_lock1");
    m_sigFreeballTarget = loader->getSignal("freeball_target");
    m_sigFreeballTargetOn = loader->getSignal("freeball_target_on");
    m_sigFreeballTargetOff = loader->getSignal("freeball_target_off");
    m_sigFloorFreedomOn = loader->getSignal("freedom_on");
    m_sigFloorFreedomOff = loader->getSignal("freedom_off");
    m_sigFloorFreedomOn = loader->getSignal("freedom_on");
    m_sigFloorFreedomBlink = loader->getSignal("floor_freedom_all");
    m_sigGnuFloorOn = loader->getSignal("multiball_floor");
    m_sigGnuFloorOff = loader->getSignal("original_floor");
    m_sigFreedomTarget = loader->getSignal("freedom_target");
    m_sigFreedomTargetOn = loader->getSignal("freedom_target_on");
    m_sigFreedomTargetOff = loader->getSignal("freedom_target_off");
    m_sigRaiseFreedomStopper = loader->getSignal("raise_freedom_stopper");
    m_sigResetFreedomStopper = loader->getSignal("freedom_stopper_reset");

    //is this last bit necessary?
    //are we checking for locked balls before a game reset?  not likely.
    //reset triggers a clear()
    /*
    for (int i = 0; i < 2; i++)
      {
        m_aLocked[i] = false;
        m_aLockedBallSlot[i] = -1;
      }
      */
  };
  ~GnuBehavior() {};
  
  void onTick() {
    Table * table = Table::getInstance();
    Score * score = table->getScore();
    Loader * loader = Loader::getInstance();
    if (table)
      {
        if (m_bBallColourChanged == false)
          {
            //this is the start of a new ball.
            BallGroup *ballgroup = table->getBall(table->getCurrentBall());
            ballgroup->getShape3D(0)->setColor(1, 1, 1, 1);
            m_bBallColourChanged = true;
            m_aArrow[0] = true;
            m_iLeftLockArrowTick=9999;
            m_iRightLockArrowTick=9999;
            for (int i = 0; i < m_iMultiplier + 1; i++)
              SendSignal( m_sig0123On[i], 0, this->getParent(), NULL );
            for (int i = 0; i < m_iGplVersionLevel + 1; i++)
              SendSignal( m_sigv123On[i], 0, this->getParent(), NULL );
            SendSignal (m_sigLightOff[0], 0, this->getParent(), NULL);
            SendSignal (m_sigLightOff[1], 0, this->getParent(), NULL);
            SendSignal (m_sigLightOff[2], 0, this->getParent(), NULL);
            SendSignal (m_sigLightBlink[0], 200, this->getParent(), NULL);
            SendSignal (m_sigLightBlink[1], 200, this->getParent(), NULL);
            SendSignal (m_sigLightBlink[2], 100, this->getParent(), NULL);
            SendSignal (m_sigLightOn[6], 50, this->getParent(), NULL);
            SendSignal (m_sigLightOff[7], 0, this->getParent(), NULL);
            SendSignal (m_sigLightOff[8], 0, this->getParent(), NULL);
            SendSignal (m_sigLightBlink[7], 150, this->getParent(), NULL);
            SendSignal (m_sigLightBlink[8], 250, this->getParent(), NULL);
            SendSignal (m_sigLightOn[11], 0, this->getParent(), NULL);
            SendSignal (m_sigLightOn[12], 0, this->getParent(), NULL);
            SendSignal (m_sigLightOn[13], 0, this->getParent(), NULL);
            SendSignal (m_sigSensitizeLockOne, 0, this->getParent(), NULL);
            if (m_aLocked[0] == false && m_aLocked[1] == false)
              {
                for (int j = 0; j < 2; j++)
                  for (int i = 0; i < 6; i++) {
                    m_aLockArrow[i][j] = false;
                    SendSignal (m_sigLockArrowOff[i][j], 0, this->getParent(), NULL);
                  }
                m_aLockArrow[0][0] = true;
              }
            else if (m_aLocked[0] == true && m_aLocked[1] == false)
              {
                for (int i = 0; i < 6; i++) {
                  SendSignal (m_sigLockArrowBlink[i][1], 0, this->getParent(), NULL);
                  for (int i = 0; i < 6; i++) {
                    m_aLockArrow[i][1] = false;
                    SendSignal (m_sigLockArrowOff[i][1], 0, this->getParent(), NULL);
                  }
                  m_aLockArrow[0][1] = true;
                }
              }
            else if (m_aLocked[0] && m_aLocked[1]) 
              {
                for (int j = 0; j < 2; j++)
                  for (int i = 0; i < 6; i++) {
                    m_aLockArrow[i][j] = false;
                    SendSignal (m_sigLockArrowBlink[i][j], 0, this->getParent(), NULL);
                  }
              }
          }
      }
    // init signals
    EmAssert(score != NULL, "GnuBehavior::onTick score NULL");
    // launch ball
    string launch("launch");

    if (table->active() == 0) {
      if (m_bGameOver == false) {
        //which ball do we activate if we just locked a ball?
        int i = getFirstDeadBall();
        if (i > -1) {
          if (i == 0) SendSignal(loader->getSignal("game_start"), 0, this->getParent(), NULL);
          SendSignal( PBL_SIG_BALL_ON, 0, this->getParent(), NULL );
          table->activateBall(i, 19.5f, 0.0f, 30.0f);   
          m_iCurrentBallSlot = i;
          m_bBallColourChanged = false;
        }
      }
    }
    m_iPlungerTick++;
    if (m_iPlungerTick >= 20) {
      if (m_aArrow[0]) {
        m_aArrow[0] = false;
        m_aArrow[1] = true;
        SendSignal( m_sigArrowOn[1], 0, this->getParent(), NULL );
        SendSignal( m_sigArrowOff[0], 0, this->getParent(), NULL );
      } else if (m_aArrow[1]) {
        m_aArrow[1] = false;
        m_aArrow[2] = true;
        SendSignal( m_sigArrowOn[2], 0, this->getParent(), NULL );
        SendSignal( m_sigArrowOff[1], 0, this->getParent(), NULL );
      } else if (m_aArrow[2]) {
        m_aArrow[2] = false;
        m_aArrow[3] = true;
        SendSignal( m_sigArrowOn[3], 0, this->getParent(), NULL );
        SendSignal( m_sigArrowOff[2], 0, this->getParent(), NULL );
      } else if (m_aArrow[3]) {
        m_aArrow[3] = false;
        m_aArrow[4] = true;
        SendSignal( m_sigArrowOn[4], 0, this->getParent(), NULL );
        SendSignal( m_sigArrowOff[3], 0, this->getParent(), NULL );
      } else if (m_aArrow[4]) {
        m_aArrow[4] = false;
        m_aArrow[5] = true;
        SendSignal( m_sigArrowOn[5], 0, this->getParent(), NULL );
        SendSignal( m_sigArrowOff[4], 0, this->getParent(), NULL );
      } else if (m_aArrow[5]) {
        m_aArrow[5] = false;
        m_aArrow[0] = true;
        SendSignal( m_sigArrowOn[0], 0, this->getParent(), NULL );
        SendSignal( m_sigArrowOff[5], 0, this->getParent(), NULL );
      }
      m_iPlungerTick = 0;
    }
    if (m_aLocked[0] == false && m_aLocked[1] == false) {
      m_iLeftLockArrowTick++;
      if (m_iLeftLockArrowTick >= 20) {
        if (m_aLockArrow[0][0]) {
          m_aLockArrow[0][0] = false;
          m_aLockArrow[1][0] = true;
          SendSignal( m_sigLockArrowBlink[1][0], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[0][0], 0, this->getParent(), NULL );
        } else if (m_aLockArrow[1][0]) {
          m_aLockArrow[1][0] = false;
          m_aLockArrow[2][0] = true;
          SendSignal( m_sigLockArrowBlink[2][0], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[1][0], 0, this->getParent(), NULL );
        } else if (m_aLockArrow[2][0]) {
          m_aLockArrow[2][0] = false;
          m_aLockArrow[3][0] = true;
          SendSignal( m_sigLockArrowBlink[3][0], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[2][0], 0, this->getParent(), NULL );
        } else if (m_aLockArrow[3][0]) {
          m_aLockArrow[3][0] = false;
          m_aLockArrow[4][0] = true;
          SendSignal( m_sigLockArrowBlink[4][0], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[3][0], 0, this->getParent(), NULL );
        } else if (m_aLockArrow[4][0]) {
          m_aLockArrow[4][0] = false;
          m_aLockArrow[5][0] = true;
          SendSignal( m_sigLockArrowBlink[5][0], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[4][0], 0, this->getParent(), NULL );
        } else if (m_aLockArrow[5][0]) {
          m_aLockArrow[5][0] = false;
          m_aLockArrow[0][0] = true;
          SendSignal( m_sigLockArrowBlink[0][0], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[5][0], 0, this->getParent(), NULL );
        }
        m_iLeftLockArrowTick = 0;
      }
    }
    if (m_aLocked[0] == true && m_aLocked[1] == false) {
      m_iRightLockArrowTick++;
      if (m_iRightLockArrowTick >= 20) {
        if (m_aLockArrow[0][1]) {
          m_aLockArrow[0][1] = false;
          m_aLockArrow[1][1] = true;
          SendSignal( m_sigLockArrowBlink[1][1], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[0][1], 0, this->getParent(), NULL );
        } else if (m_aLockArrow[1][1]) {
          m_aLockArrow[1][1] = false;
          m_aLockArrow[2][1] = true;
          SendSignal( m_sigLockArrowBlink[2][1], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[1][1], 0, this->getParent(), NULL );
        } else if (m_aLockArrow[2][1]) {
          m_aLockArrow[2][1] = false;
          m_aLockArrow[3][1] = true;
          SendSignal( m_sigLockArrowBlink[3][1], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[2][1], 0, this->getParent(), NULL );
        } else if (m_aLockArrow[3][1]) {
          m_aLockArrow[3][1] = false;
          m_aLockArrow[4][1] = true;
          SendSignal( m_sigLockArrowBlink[4][1], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[3][1], 0, this->getParent(), NULL );
        } else if (m_aLockArrow[4][1]) {
          m_aLockArrow[4][1] = false;
          m_aLockArrow[5][1] = true;
          SendSignal( m_sigLockArrowBlink[5][1], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[4][1], 0, this->getParent(), NULL );
        } else if (m_aLockArrow[5][1]) {
          m_aLockArrow[5][1] = false;
          m_aLockArrow[0][1] = true;
          SendSignal( m_sigLockArrowBlink[0][1], 0, this->getParent(), NULL );
          SendSignal( m_sigLockArrowOff[5][1], 0, this->getParent(), NULL );
        }
        m_iRightLockArrowTick = 0;
      }
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
      if (m_bFreeBall == false) {
        Table * table = Table::getInstance();
        if (table->active() == 0) {
          if (table->getCurrentBall() == MAX_BALL-1) {
            SendSignal( PBL_SIG_GAME_OVER, 0, this->getParent(), NULL );
            m_bGameOver = true;
          }
          else
            table->setCurrentBall(table->getCurrentBall()+1);
          m_bMultiBall = false; 
        }
      }
      else {
        m_bFreeBall = false;
        m_iGplVersionLevel = 0;
        SendSignal (m_sigv123Off[3], 0, this->getParent(), NULL);
        //for (int i = 1; i < 4; i++)
          //SendSignal (m_sigv123Off[i], 0, this->getParent(), NULL);
        //SendSignal (m_sigv123On[0], 0, this->getParent(), NULL);
        //SendSignal (m_sigv123Announce[0], 0, this->getParent(), NULL);
      }
    }
    ElseOnSignal(m_sigBump) {
      score->addScore(withMultiplier(50));
    }
    ElseOnSignal(m_sigGnu[0]) {
      if (!m_aGnu[0]) {
	SendSignal(m_sigGnuOn[0], 0, this->getParent(), NULL);
	m_aGnu[0] = true;
      }
      score->addScore(withMultiplier(50));
    } 
    ElseOnSignal(m_sigGnu[1]) {
      if (!m_aGnu[1]) {
	SendSignal(m_sigGnuOn[1], 0, this->getParent(), NULL);
	m_aGnu[1] = true;
      }
      score->addScore(withMultiplier(50));
    } 
    ElseOnSignal(m_sigGnu[2]) {
      if (!m_aGnu[2]) {
	SendSignal(m_sigGnuOn[2], 0, this->getParent(), NULL);
	m_aGnu[2] = true;
      }
      score->addScore(withMultiplier(50));
    } 
    ElseOnSignal(m_sigFree[0]) {
      if (!m_aFree[0]) {
	SendSignal(m_sigFreeOn[0], 0, this->getParent(), NULL);
	m_aFree[0] = true;
      }
      score->addScore(withMultiplier(50));
    } 
    ElseOnSignal(m_sigFree[1]) {
      if (!m_aFree[1]) {
	SendSignal(m_sigFreeOn[1], 0, this->getParent(), NULL);
	m_aFree[1] = true;
      }
      score->addScore(withMultiplier(50));
    } 
    ElseOnSignal(m_sigFree[2]) {
      if (!m_aFree[2]) {
	SendSignal(m_sigFreeOn[2], 0, this->getParent(), NULL);
	m_aFree[2] = true;
      }
      score->addScore(withMultiplier(50));
    } 
    ElseOnSignal(m_sigFree[3]) {
      if (!m_aFree[3]) {
	SendSignal(m_sigFreeOn[3], 0, this->getParent(), NULL);
	m_aFree[3] = true;
      }
      score->addScore(withMultiplier(50));
    } 
    ElseOnSignal(m_sigKnockdown[0][0]) {
      if (!m_aKnockdown[0]) {
	SendSignal(m_sigCopyleftOn[0], 0, this->getParent(), NULL);
	m_aKnockdown[0] = true;
        score->addScore(withMultiplier(50));
      }
    } 
    ElseOnSignal(m_sigKnockdown[1][0]) {
      if (!m_aKnockdown[1]) {
	SendSignal(m_sigCopyleftOn[1], 0, this->getParent(), NULL);
	m_aKnockdown[1] = true;
        score->addScore(withMultiplier(50));
      }
    } 
    ElseOnSignal(m_sigKnockdown[2][0]) {
      if (!m_aKnockdown[2]) {
	SendSignal(m_sigCopyleftOn[2], 0, this->getParent(), NULL);
	m_aKnockdown[2] = true;
        score->addScore(withMultiplier(50));
      }
    } 
    ElseOnSignal(m_sigKnockdown[3][0]) {
      if (!m_aKnockdown[3]) {
	SendSignal(m_sigCopyleftOn[3], 0, this->getParent(), NULL);
	m_aKnockdown[3] = true;
        score->addScore(withMultiplier(50));
      }
    } 
    ElseOnSignal(m_sigKnockdown[4][0]) {
      if (!m_aKnockdown[4]) {
	SendSignal(m_sigCopyleftOn[4], 0, this->getParent(), NULL);
	m_aKnockdown[4] = true;
        score->addScore(withMultiplier(50));
      }
    } 
    ElseOnSignal(m_sigKnockdown[5][0]) {
      if (!m_aKnockdown[5]) {
	SendSignal(m_sigCopyleftOn[5], 0, this->getParent(), NULL);
	m_aKnockdown[5] = true;
        score->addScore(withMultiplier(50));
      }
    } 
    ElseOnSignal(m_sigKnockdown[6][0]) {
      if (!m_aKnockdown[6]) {
	SendSignal(m_sigCopyleftOn[6], 0, this->getParent(), NULL);
	m_aKnockdown[6] = true;
        score->addScore(withMultiplier(50));
      }
    }
    ElseOnSignal(m_sigKnockdown[7][0]) {
      if (!m_aKnockdown[7]) {
	SendSignal(m_sigCopyleftOn[7], 0, this->getParent(), NULL);
	m_aKnockdown[7] = true;
        score->addScore(withMultiplier(50));
      }
    }
    ElseOnSignal(m_sigKnockdown[8][0]) {
      if (!m_aKnockdown[8]) {
	SendSignal(m_sigGplOn[0], 0, this->getParent(), NULL);
	m_aKnockdown[8] = true;
        score->addScore(withMultiplier(50));
      }
    }
    ElseOnSignal(m_sigKnockdown[9][0]) {
      if (!m_aKnockdown[9]) {
	SendSignal(m_sigGplOn[1], 0, this->getParent(), NULL);
	m_aKnockdown[9] = true;
        score->addScore(withMultiplier(50));
      }
    }
    ElseOnSignal(m_sigKnockdown[10][0]) {
      if (!m_aKnockdown[10]) {
	SendSignal(m_sigGplOn[2], 0, this->getParent(), NULL);
	m_aKnockdown[10] = true;
        score->addScore(withMultiplier(50));
      }
    }
    ElseOnSignal(m_sigMidStopperBreached) {
      for (int i = 0; i < 6; i++)
        {
          SendSignal( m_sigArrowOff[i], 0, this->getParent(), NULL );
          m_aArrow[i] = false;
        }
      SendSignal (m_sigLightOff[6], 0, this->getParent(), NULL);
      SendSignal (m_sigLightOn[3], 0, this->getParent(), NULL);
      SendSignal (m_sigLightOn[4], 0, this->getParent(), NULL);
      if (m_bRunIt) {
        SendSignal (m_sig0123Announce[0], 0, this->getParent(), NULL);
        m_bRunIt = false;
      }
    }
    ElseOnSignal(m_sigBottomStopperBreached) {
      //working around a glitch that has some balls still locked
      //even though they aren't.
      if (m_aLocked[0] == false && m_aLocked[1] == false) {
        unLockBalls();
      }
      SendSignal (m_sigLightOff[3], 0, this->getParent(), NULL);
      SendSignal (m_sigLightOff[4], 0, this->getParent(), NULL);
      if (table->active() > 1)
        {
          SendSignal (m_sigLightOn[3], 200, this->getParent(), NULL);
          SendSignal (m_sigLightOn[4], 200, this->getParent(), NULL);
        }
    }
    ElseOnSignal(m_sigSideStopperBreached) {
      score->addScore(withMultiplier(500));
      //turn on the plunger light and arrows.
      m_aArrow[0] = true;
      SendSignal (m_sigLightOn[6], 50, this->getParent(), NULL);
    }
    ElseOnSignal(m_sigLock[0]) {
      m_aLocked[0] = true;
      m_aLockedBallSlot[0] = m_iCurrentBallSlot;
      for (int i = 0; i < 6; i++)
        SendSignal( m_sigLockArrowBlink[i][0], 0, this->getParent(), NULL );
      m_aLockArrow[0][1] = true; //start spinning the other wheel
    }
    ElseOnSignal(m_sigLock[1]) {
      m_aLocked[1] = true;
      m_aLockedBallSlot[1] = m_iCurrentBallSlot;
      for (int i = 0; i < 6; i++)
        SendSignal( m_sigLockArrowBlink[i][1], 0, this->getParent(), NULL );
    }
    ElseOnSignal( PBL_SIG_BALL_ON ) {
    Table * table = Table::getInstance();
    for (int i = 0; i < MAX_BALL; i++)
      {
            BallGroup *ballgroup = table->getBall(i);
            ballgroup->getShape3D(0)->setColor(1, 1, 1, 1);
      }
    }
    ElseOnSignal( m_sigFreeballTarget) {
      if (m_iGplVersionLevel == 2)
        {
          m_iGplVersionLevel = 3;
          SendSignal (m_sigLightOff[10], 0, this->getParent(), NULL);
          SendSignal (m_sigv123On[3], 0, this->getParent(), NULL);
          m_bFreeBall = true;
          m_bFreeBallTargetOn = false;
          SendSignal (m_sigv123On[0], 1000, this->getParent(), NULL);
          SendSignal (m_sigv123Off[1], 0, this->getParent(), NULL);
          SendSignal (m_sigv123Off[2], 0, this->getParent(), NULL);
          SendSignal (m_sigv123Announce[3], 0, this->getParent(), NULL);
          SendSignal (m_sigv123Announce[0], 2000, this->getParent(), NULL);
        }
    }
    ElseOnSignal( m_sigFreedomTarget) {
      if (m_iMultiplier == 3)
        {
          SendSignal (m_sigLightOff[9], 0, this->getParent(), NULL);
          m_bFreedomTargetOn = false;
          m_bFreedomStopperOn = true;
          SendSignal (m_sigRaiseFreedomStopper, 0, this->getParent(), NULL);
          SendSignal (m_sigFloorFreedomOn, 0, this->getParent(), NULL);
        }
    }

    // GNU all
    if (m_aGnu[0] && m_aGnu[1] && m_aGnu[2]) {
      SendSignal(m_sigGnuAll, 0, this->getParent(), NULL);
      m_aGnu[0] = m_aGnu[1] = m_aGnu[2] = false;
      score->addScore(withMultiplier(500));
    }

    // FREE all
    if (m_aFree[0] && m_aFree[1] && m_aFree[2] && m_aFree[3]) {
      SendSignal(m_sigFreeAll, 0, this->getParent(), NULL);
      m_aFree[0] = m_aFree[1] = m_aFree[2] = m_aFree[3] = false;
      score->addScore(withMultiplier(500));
      //MULTIBALL
      if (m_aLocked[0] == true && m_aLocked[1] == true) {
        score->addScore(withMultiplier(1000));
        unLockBalls();
        SendSignal(m_sigLockRelease, 0, this->getParent(), NULL);
        m_bMultiBall = true; 
        SendSignal (m_sigGnuFloorOn, 0, this->getParent(), NULL);
        SendSignal (m_sigGnuFloorOff, 200, this->getParent(), NULL);
        for (int i = 0; i < 6; i++)
          SendSignal (m_sigArrowBlink[i], 0, this->getParent(), NULL);

        for (int j = 0; j < 2; j++)
          for (int i = 0; i < 6; i++)
            SendSignal( m_sigLockArrowCrazyBlink[i][j], 0, this->getParent(), NULL );
        for (int j = 0; j < 2; j++)
          for (int i = 0; i < 6; i++) {
            SendSignal( m_sigLockArrowOn[i][j], 600, this->getParent(), NULL );
            m_aLockArrow[i][j] = false;
          }
        for (int i = 0; i < 6; i++)
          SendSignal (m_sigArrowOff[i], 600, this->getParent(), NULL);

        for (int i = 0; i < 14; i++)
          SendSignal (m_sigLightCrazyBlink[i], 0, this->getParent(), NULL);
        for (int i = 0; i < 14; i++)
          SendSignal (m_sigLightOff[i], 600, this->getParent(), NULL);
        SendSignal (m_sigLightBlink[0], 600+200, this->getParent(), NULL);
        SendSignal (m_sigLightBlink[1], 600+200, this->getParent(), NULL);
        SendSignal (m_sigLightBlink[2], 600+100, this->getParent(), NULL);
        SendSignal (m_sigLightBlink[7], 600+150, this->getParent(), NULL);
        SendSignal (m_sigLightBlink[8], 600+250, this->getParent(), NULL);
        SendSignal (m_sigLightOn[3], 650, this->getParent(), NULL);
        SendSignal (m_sigLightOn[4], 650, this->getParent(), NULL);
        SendSignal (m_sigLightOff[5], 650, this->getParent(), NULL);
        SendSignal (m_sigLightOff[6], 650, this->getParent(), NULL);
        SendSignal (m_sigLightOn[11], 650, this->getParent(), NULL);
        SendSignal (m_sigLightOn[12], 650, this->getParent(), NULL);
        SendSignal (m_sigLightOn[13], 650, this->getParent(), NULL);
        if (m_bFreeBallTargetOn)
          SendSignal (m_sigLightOn[9], 650, this->getParent(), NULL);
        else
          SendSignal (m_sigLightOff[9], 650, this->getParent(), NULL);
        if (m_bFreedomTargetOn)
          SendSignal (m_sigLightOn[10], 650, this->getParent(), NULL);
        else
          SendSignal (m_sigLightOff[10], 650, this->getParent(), NULL);
        for (int i = 0; i < 4; i++)
          SendSignal (m_sig0123Off[i], 0, this->getParent(), NULL);
        for (int i = 0; i < 4; i++)
          {
            SendSignal (m_sig0123On[i], 10 + (i+1)*15, this->getParent(), NULL);
          }
        for (int i = 0; i < 4; i++)
          SendSignal (m_sig0123Off[i], 85, this->getParent(), NULL);
        for (int i = 0; i < 4; i++)
          {
            SendSignal (m_sig0123On[i], 95 + (i+1)*15, this->getParent(), NULL);
          }
        for (int i = 0; i < 4; i++)
          SendSignal (m_sig0123Off[i], 170, this->getParent(), NULL);
        for (int i = 0; i < 4; i++)
          SendSignal (m_sig0123On[i], 185 + (i+1)*15, this->getParent(), NULL);
        for (int i = 0; i < 4; i++)
          SendSignal (m_sig0123Off[i], 260, this->getParent(), NULL);
        for (int i = 0; i < m_iMultiplier+1; i++)
          SendSignal (m_sig0123On[i], 275, this->getParent(), NULL);
        for (int i = 0; i < 4; i++)
          SendSignal (m_sigv123Off[i], 85, this->getParent(), NULL);
        for (int i = 0; i < 4; i++)
          SendSignal (m_sigv123On[i], 95 + (i+1)*15, this->getParent(), NULL);
        for (int i = 0; i < 4; i++)
          SendSignal (m_sigv123Off[i], 170, this->getParent(), NULL);
        for (int i = 0; i < 4; i++)
          SendSignal (m_sigv123On[i], 185 + (i+1)*15, this->getParent(), NULL);
        for (int i = 0; i < 4; i++)
          SendSignal (m_sigv123Off[i], 260, this->getParent(), NULL);
        for (int i = 0; i < m_iGplVersionLevel+1; i++)
          SendSignal (m_sigv123On[i], 275, this->getParent(), NULL);
      }
    }
    // COPYLEFT all
    if (m_aKnockdown[0] && m_aKnockdown[1] && m_aKnockdown[2] && 
        m_aKnockdown[3] && m_aKnockdown[4] && m_aKnockdown[5] && 
        m_aKnockdown[6] && m_aKnockdown[7]) {
      int i;
      SendSignal(m_sigCopyleftAll, 0, this->getParent(), NULL);
      score->addScore(withMultiplier(500));
      for (i = 0; i < 8; i++)
        {
          if (m_aKnockdown[i])
            {
              SendSignal(m_sigKnocking[i][1], 250, this->getParent(), NULL);
              m_aKnockdown[i] = false;
            }
        }
	SendSignal(m_sigCopyleftKnockdownReset, 300, this->getParent(), NULL);
        m_iMultiplier++;
        if (m_iMultiplier > 3)
            m_iMultiplier = 3;
        else
          {
          SendSignal(m_sig0123On[m_iMultiplier], 50, this->getParent(), NULL);
          SendSignal(m_sig0123Announce[m_iMultiplier], 0, this->getParent(), NULL);
          }
        if (m_iMultiplier == 3 && m_bFreedomStopperOn == false) {
          m_bFreedomTargetOn = true;
          SendSignal (m_sigFreedomTargetOn, 0, this->getParent(), NULL);
          SendSignal (m_sigFreedomTargetAnnounce, 0, this->getParent(), NULL);
        }
    }

    //GPL all
    if (m_aKnockdown[8] && m_aKnockdown[9] && m_aKnockdown[10]) {
      SendSignal(m_sigGplAll, 0, this->getParent(), NULL);
      score->addScore(withMultiplier(500));
      for (int i = 8; i < 11; i++)
        {
          if (m_aKnockdown[i])
            {
              SendSignal(m_sigKnocking[i][1], 250, this->getParent(), NULL);
              m_aKnockdown[i] = false;
            }
        }
	SendSignal(m_sigGplKnockdownReset, 300, this->getParent(), NULL);
        if (m_iGplVersionLevel < 2) {
          m_iGplVersionLevel++;
          for (int i = 0; i <= m_iGplVersionLevel; i++)
            SendSignal(m_sigv123On[i], 50, this->getParent(), NULL);

          SendSignal (m_sigv123Announce[m_iGplVersionLevel], 0, this->getParent(), NULL);
          }
        
        if (m_iGplVersionLevel == 2 && m_bFreeBallTargetOn == false && 
            m_bFreeBall == false) {
          //to get to the last GplVersionLevel we have to shoot a target.
          //(freeball)
          SendSignal (m_sigFreeballTargetOn, 0, this->getParent(), NULL);
          SendSignal (m_sigLightCrazyBlink[10], 0, this->getParent(), NULL);
          m_bFreeBallTargetOn = true;
          SendSignal (m_sigFreeBallTargetAnnounce, 0, this->getParent(), NULL);
        }
        else if (m_iGplVersionLevel == 2 && m_bFreeBallTargetOn == false  &&
                 m_bFreeBall == true) {
          //okay, very special case here.  we wrapped around on the v123s
          //while we got a free ball on this same ball.
          m_iGplVersionLevel = 0;
          SendSignal (m_sigv123Announce[m_iGplVersionLevel], 0, this->getParent(), NULL);
          for (int i = 1; i < 4; i++)
            SendSignal(m_sigv123Off[i], 60, this->getParent(), NULL);
          SendSignal(m_sigv123On[0], 60, this->getParent(), NULL);
        }

    }
  }
  
  void clear() {
    int i = 0, j = 0;
    for (i = 0; i < 3; i++)
      m_aGnu[i] = false;
    for (i = 0; i < 4; i++)
      m_aFree[i] = false;
    for (i = 0; i < 11; i++)
      m_aKnockdown[i] = false;
    m_bBallColourChanged = false;
    for (i = 0; i < 6; i++)
      m_aArrow[i] = false;
    for (i = 0; i < 6; i++)
      for (j = 0; j < 2; j++)
        m_aLockArrow[i][j] = false;
    m_iMultiplier = 0;
    m_iGplVersionLevel = 0;
    m_iLeftLockArrowTick = 0;
    m_iRightLockArrowTick = 0;
    m_iPlungerTick = 0;
    for (i = 0; i < 2; i++)
      {
        m_aLocked[i] = false;
        m_aLockedBallSlot[i] = -1;
      }
    m_bMultiBall = false;
    m_bGameOver = false;
    m_bFreeBall = false;
    m_bFreeBallTargetOn = false;
    m_bFreedomTargetOn = false;
    m_bFreedomStopperOn = false;
    m_bRunIt = true;
  };
  
  int getFirstDeadBall() {
    Table * table = Table::getInstance();
    for (int i = 0; i < MAX_BALL; i++)
      if (table->isBallDead(i))
        return i;
    return -1;
  }

  void  unLockBalls() {
    Table * table = Table::getInstance();
    for (int i = 0; i < 2; i++) {
      if (m_aLocked[i]) {
        BallGroup *ballgroup = table->getBall(m_aLockedBallSlot[i]);
        ballgroup->getShape3D(0)->getParent()->addTranslation(1, 0, 1);
        m_aLockedBallSlot[i] = -1;
        m_aLocked[i] = false;
        table->unLockBall(m_aLockedBallSlot[i]);
      }
    }
    for (int i = 0; i < MAX_BALL; i++)
      {
        if (table->isBallLocked(i))
          table->unLockBall(i);
      }
  };
  int withMultiplier(int score) {
    return score * (m_iMultiplier + 1);
  };
private:
  int m_sigGnuAll;
  bool m_aGnu[3];
  int m_sigGnu[3];
  int m_sigGnuOn[3];
  int m_sigGnuOff[3];
  bool m_bBallColourChanged;
  int m_sigBump;
  int m_sigKnockdown[11][2]; //0 = down, 1 = up
  int m_sigKnocking[11][2]; //0 = down, 1 = up
  int m_sigCopyleftKnockdownReset;
  int m_sigGplKnockdownReset;
  int m_sigFreeAll;
  bool m_aFree[4];
  int m_sigFree[4];
  int m_sigFreeOn[4];
  int m_sigFreeOff[4];
  int m_sigCopyleftAll;
  int m_sigCopyleft[8]; //the COPYLEFT lights
  int m_sigCopyleftOn[8];
  int m_sigCopyleftOff[8];
  int m_sigGplAll;
  int m_sigGpl[3]; //the GPL lights
  int m_sigGplOn[3];
  int m_sigGplOff[3];
  bool m_aKnockdown[11]; //the first 8 are COPYLEFT, the rest are GPL
  int m_sigArrowOn[6];
  int m_sigArrowOff[6];
  int m_sigArrowBlink[6];
  bool m_aArrow[6];
  int m_iPlungerTick;
  int m_sigMidStopperBreached;
  int m_sigBottomStopperBreached;
  int m_sigSideStopperBreached;
  int m_iMultiplier;
  int m_iGplVersionLevel;
  int m_sig0123All;
  int m_sig0123On[4];
  int m_sig0123Off[4];
  int m_sig0123Announce[4];
  int m_sigv123All;
  int m_sigv123On[4];
  int m_sigv123Off[4];
  int m_sigv123Announce[4];
  int m_sigLockArrowOn[6][2]; //0 = left lock arrows, 1 = right lock arrows
  int m_sigLockArrowOff[6][2];
  int m_sigLockArrowBlink[6][2];
  int m_sigLockArrowCrazyBlink[6][2];
  bool m_aLockArrow[6][2];
  int m_iLeftLockArrowTick;
  int m_iRightLockArrowTick;
  int m_sigLightOn[14]; //0, 1, 2 bumper lights.  3, 4 flippers, 5 gnu head
  int m_sigLightOff[14]; //6 plunger, 7, 8 flat bumper lights,
  int m_sigLightBlink[14]; //9 freedom target, 10 freeball target
  int m_sigLightCrazyBlink[14]; //11, 12, 13 under walled roofs
  int m_sigLock[2];
  int m_sigLockRelease;
  int m_sigSensitizeLockOne;
  bool m_aLocked[2];
  int m_aLockedBallSlot[2];
  bool m_bMultiBall;
  bool m_bGameOver;
  int m_iCurrentBallSlot;
  int m_sigFreeballTarget;
  int m_sigFreeballTargetOn;
  int m_sigFreeballTargetOff;
  bool m_bFreeBall;
  int m_sigFreedomTarget;
  int m_sigFreedomTargetOn;
  int m_sigFreedomTargetOff;
  int m_sigGnuFloorOn;
  int m_sigGnuFloorOff;
  int m_sigFloorFreedomOn;
  int m_sigFloorFreedomOff;
  int m_sigFloorFreedomBlink;
  int m_sigRaiseFreedomStopper;
  int m_sigResetFreedomStopper;
  bool m_bFreedomTargetOn;
  bool m_bFreeBallTargetOn;
  bool m_bFreedomStopperOn;
  bool m_bRunIt;
  int m_sigFreedomTargetAnnounce;
  int m_sigFreeBallTargetAnnounce;
};

extern "C"  void * new_object_fct(void) {
  return new GnuBehavior();
}

