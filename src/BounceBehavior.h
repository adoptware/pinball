/***************************************************************************
                          BounceBehavior.h  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef BOUNCEBEHAVIOR_E
#define BOUNCEBEHAVIOR_E

#include "Behavior.h"
#include "EMath.h"
#include "StateMachine.h"

#define MAX_SPEED 0.5f
#define FIRE_SPEED 0.4f
#define MAX_SPEED_Y_DOWN (MAX_SPEED*0.5f)
#define SPEED_FCT 0.6f
#define Y_GRAVITY -(SPEED_FCT*0.005f) // -SPEED_FCT/200
#define Z_GRAVITY (SPEED_FCT*0.002f) //  SPEED_FCT/500
#define BORDER (SPEED_FCT*0.05f)
#define BORDER2 (SPEED_FCT*0.02f)

#define ARM_TABLE_SIZE 8 // do not use less than 3
#define ARM_WIDTH 6.1

/** A behavior for the pinball demo. */

class BounceBehavior : public Behavior {
 public:
  BounceBehavior(int ball);
  ~BounceBehavior();
  void onTick();
  void StdOnSignal();
  void onCollision(const Vertex3D & vtx1, const Vertex3D & vtx2, Group* g);
  void getDirection(Vertex3D & vtx);
  inline bool isAlive() { return m_bAlive; };
  inline void setFire(bool f) { m_bFire = f; };
	/** drops the ball at x,y,z */
  void activateBall(float x, float y, float z);
 private:
  void checkSpeed();

  bool m_bAlive;
  int m_iBall;
  Vertex3D m_vtxDir;
  Vertex3D m_vtxOldDir;
  int m_iDirFactor;
  int m_iCollisionPrio;
  bool m_bFire;
};

#endif // BounceBehavior
