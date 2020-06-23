/***************************************************************************
                          StateBehavior.h  -  description
                             -------------------
    begin                : Thu Feb 24 2002
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef STATEBEHAVIOR_H
#define STATEBEHAVIOR_H

#include "Behavior.h"
#include "StateMachine.h"

class Light;

/** A state in the a StateBehavior object. */
class StateItem {
 public:
  StateItem();
  ~StateItem();
  void setMoveSteps(int steps) { m_iMoveSteps = steps; };
  void setTr(float x, float y, float z);
  void setRot(float x, float y, float z);
  void addTexCoord(float u, float v);
  void setUserProperty(int p) { m_iUserProperty = p; };
  void setShapeProperty(int p) { m_iShapeProperty = p; };
  void setLight(bool b) { m_bLight = b; };
  void setSound(int s) { m_iSound = s; };
  void setMusic(int m) { m_iMusic = m; };
  void setDelayState(int state, int delay) { m_iDelayState = state; m_iDelay = delay; };
  void setCollState(int state) { m_iCollState = state; };
  void setActSig(int s) { m_iActSig = s; };
  void setCollSig(int s) { m_iCollSig = s; };
  void setSigDelay(int d) { m_iSigDelay = d; };
  void addShapeEnable(int i, bool e);
  
  int getUserProperty() { return m_iUserProperty; }
  int getShapeProperty() { return m_iShapeProperty; }
  bool getLight() { return m_bLight; };
  int getSound() { return m_iSound; };
  int getMusic() { return m_iMusic; };
  int getDelay() { return m_iDelay; };
  int getDelayState() { return m_iDelayState; };
  int getCollState() { return m_iCollState; };
  int getActSig() { return m_iActSig; };
  int getCollSig() { return m_iCollSig; };
  int getSigDelay() { return m_iSigDelay; };
  bool getShapeEnable(int i);
  
  int m_iActSig;
  int m_iCollSig;
  int m_iSigDelay;
  bool m_bLight;
  int m_iMoveSteps;
  Vertex3D m_vtxTr;
  Vertex3D m_vtxRot;
  //float m_fTrSpeed;
  //float m_fRotSpeed;
  vector<TexCoord> m_vTexCoord;
  vector<bool> m_vShapeEnable;
  int m_iUserProperty;
  int m_iShapeProperty;
  int m_iMusic;
  int m_iSound;
  int m_iDelay;
  int m_iTick;
  int m_iDelayState;
  int m_iCollState;
};

/** A statemachine. Create StateItems and insert them into this behavior. */
class StateBehavior : public Behavior  {
 public:
  StateBehavior();
  ~StateBehavior();
  void onTick();
  void StdOnSignal();
  void StdOnCollision();
  void useMove(bool t) { m_bMove = t; };
  bool usesMove() { return m_bMove; };
  void useTexCoord(bool t) { m_bTexCoord = t; };
  bool usesTexCoord() { return m_bTexCoord; };
  void useShape(bool t) { m_bShape = t; };
  bool usesShape() { return m_bShape; };
  //void useProperty(bool t) { m_bProperty = t; };
  void addStateItem(StateItem * s);
  void removeStateItem(StateItem * s);
  StateItem * getStateItem(int i);
  int getOwnerBall() { return m_iOwnerBall; };
  void setState(StateItem* s);
 private:
  int m_iOwnerBall;
  int m_iTick;
  int m_iCollisionSafe;
  Vertex3D m_vtxTr;
  Vertex3D m_vtxRot;
  //bool m_bProperty;
  bool m_bTexCoord;
  bool m_bMove;
  bool m_bShape;
  StateItem* p_CurrentStateItem;
  vector<StateItem*> m_vStateItem;
};

#endif
