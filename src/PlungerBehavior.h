/***************************************************************************
                          PlungerBehavior.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef PLUNGERBEHAVIOR_H
#define PLUNGERBEHAVIOR_H

#include "Behavior.h"
#include "StateMachine.h"
#include "EMath.h"

/** A plunger, the . */
class PlungerBehavior : public Behavior {
 public:
  PlungerBehavior();
  ~PlungerBehavior();
  void onTick();
  void StdOnSignal() {};
  void StdOnCollision();
  inline void setSound(int s) { m_iSound = s; };
  inline int getSound() { return m_iSound; };
  /** zero power is 0.0f max power is 1.0f */
  inline float getPower() { return m_fPower; };
  inline bool getLaunch() { return m_iLaunchState == 2; };
 private:
  int m_iLaunchState;
  float m_fPower;
  int m_iCounter;
  int m_sigPlunger;
  int m_iSound;
  bool m_bFirst;
  Vertex3D m_vtxTr;

  bool m_bDoublePress;
};

#endif // ARMBEHAVIOR_H
