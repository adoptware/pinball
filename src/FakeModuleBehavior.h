/***************************************************************************
                          FakeModuleBehavior.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef FAKEMODULEBEHAVIOR_H
#define FAKEMODULEBEHAVIOR_H

#include <cstring>
#include "Behavior.h"
#include "Pinball.h"

class FakeModuleBehavior : public Behavior {
 public:
  FakeModuleBehavior(const char * name) { 
    strncpy(m_Name, name, 255);
    this->setType(PBL_TYPE_FAKEMODULEBEH);
  };
  ~FakeModuleBehavior() {};
  const char * getName() {
    return m_Name;
  }
  void setName(const char * name) {
    strncpy(m_Name, name, 255);
  }
  void onTick() {};
  void onCollision(const Vertex3D & vtxWall, const Vertex3D & vtxOwn, Group * g) {};
  void onSignal(int signal, Group * sender) {};
 private:
  char m_Name[256];
};

#endif // KEYBEHAVIOR_H
