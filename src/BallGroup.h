/***************************************************************************
                          BigSphere.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
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
  BallGroup(float r, float g, float b, int pbl, float x);
  ~BallGroup();
  /** -1 always, 0 off, >1 timer */
  inline void setFireTimer(int timer) {
    m_iFireTimer = timer;
  };
  void updateFire();
  void resetFire();
  void tick();
 private:
  int m_iFireTimer;
  Vertex3D vtxPrev;
  Group * aFire[8];
  Shape3D * aFireShape[8];
  Vertex3D aFireVtx[16];
};

#endif // SPHERE_H
