/***************************************************************************
                          EyeBehavior.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef EYEBEHAVIOR_H
#define EYEBEHAVIOR_H

#include "Behavior.h"
#include "StateMachine.h"

// locked view
#define TX0 -1.75f
#define TY0 40.0f
#define TZ0 40.0f
#define RX0 0.15f
#define RY0 0.0f
#define RZ0 0.0f

// soft pan & scan view
#define TX1 -1.75f 
#define TY1 35.0f
#define TZ1 37.0f
#define RX1 0.14f
#define RY1 0.0f
#define RZ1 0.0f

// moving pan & scan view
#define TX2 -1.75f 
#define TY2 32.0f
#define TZ2 34.0f
#define RX2 0.14f
#define RY2 0.0f
#define RZ2 0.0f

// top view
#define TX3 -1.75f 
#define TY3 40.0f
#define TZ3 10.0f
#define RX3 0.23f
#define RY3 0.0f
#define RZ3 0.0f

// top-down
//#define TX4 0.0f  //Table posistion from center
#define TX4 -1.75f  //Table posistion from center
#define TY4 70.0f //Camera Distance from Table
#define TZ4 0.0f //Distance from bottom of screen
#define RX4 0.25f //rotation of table top-bottom
#define RY4 0.0f //rotation of table left to right
#define RZ4 0.0f //rotation of table on X- axis (clock)

// top-down-rotated
#define TX5 0.0f  //Table posistion from center
#define TY5 70.0f //Camera Distance from Table
#define TZ5 0.0f //Distance from bottom of screen
#define RX5 0.25f //rotation of table top-bottom
#define RY5 0.0f //rotation of table left to right
#define RZ5 0.25f //rotation of table on X- axis (clock)

class Score;

/**
 * <p>A Behavior for the pinball demo.</p>
 */
class EyeBehavior : public Behavior {
 public:
  EyeBehavior();
  ~EyeBehavior();
  void onTick();
  void StdOnSignal();
  void StdOnCollision() { EmAssert(false, "EyeBehavior::StdOnCollision() not implemented"); };
  inline void setSound(int s) { m_iSound = s; };
 private:
  int m_iNudgeTick;
  int m_iNudgeType;
  int m_iTiltTick;
  float m_fXNudge;
  float m_fZNudge;
  int m_iSound;
  bool m_bTilt;
  bool m_bFButton;
};

#endif // EYEBEHAVIOR_H
