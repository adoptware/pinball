/***************************************************************************
                          BallGroup.h  -  description
                             -------------------
    begin                : Sat Mar 8 2003
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef BALLGROUP_H
#define BALLGROUP_H

#include "Group.h"
#include "EMath.h"

class Engine;
class Group;
class Shape3D;

/** */
class BallGroup : public Group {
 public:
  BallGroup(float r, float g, float b, int pbl);
  ~BallGroup();
  /** -1 always, 0 off, >1 timer */
  inline void setFireTimer(int timer) {
    m_iFireTimer = timer;
  };
  void updateFire();
  void resetFire();
  void tick();
  inline int getBall() { return m_iBall; };
 private:
  int m_iBall;
  int m_iFireTimer;
  Vertex3D vtxPrev;
  Group * aFire[8];
  Shape3D * aFireShape[8];
  Vertex3D aFireVtx[16];
};

#endif // BALLGROUP_H
