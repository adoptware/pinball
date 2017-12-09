/***************************************************************************
                          ArmBehavior.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 


    ========================= Modifications =========================

        Apr. 6, 2017:
            Replace name of "m_bOn" with "m_bActive". (c30zD)

 ***************************************************************************/

#ifndef ARMBEHAVIOR_H
#define ARMBEHAVIOR_H

#include <string>

#include "Behavior.h"
#include "StateMachine.h"
#include "EMath.h"
#include "Keyboard.h"

/** A Behavior for the pinball demo. */
class ArmBehavior : public Behavior {
 public:
  ArmBehavior(bool right = true);
  ~ArmBehavior();
  void onTick();
  void StdOnSignal();
  void StdEmptyOnCollision() {};
  void doArm(EMKey key);
  inline void setSound(int s) { m_iSound = s; };
  inline int getSound() { return m_iSound; };
  inline void setIsRight(bool r) { m_bRight = r; };
  inline bool getIsRight() { return m_bRight; };
 private:
  Vertex3D m_vtxRot;
  bool m_bRight;
  int m_iCount;
  bool m_bActive;
  int m_iSound;
  bool m_bFirst;
  bool m_bTilt;
};

#endif // ARMBEHAVIOR_H
