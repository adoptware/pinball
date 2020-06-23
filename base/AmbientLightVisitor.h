/***************************************************************************
                          AmbientLightVisitor.h  -  description
                             -------------------
    begin                : Sun Jan 30 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef AMBIENTLIGHTVISITOR_H
#define AMBIENTLIGHTVISITOR_H

#include <vector>

#include "Visitor.h"
#include "EMath.h"

class Light;
class Group;
class Shape3D;

/** Only for internal use. */
class AmbientLightVisitor : public Visitor {
 protected:
  AmbientLightVisitor(int size = 4);
 public:
  ~AmbientLightVisitor();
  static AmbientLightVisitor * getInstance();
  void setLightning(float s, float bg);
  void add(Light* l);
  void visit(Group* g);
  void clear();
 private:
  void visitAmbient(Shape3D* s);
  void visitPoint(Shape3D* s);
  
  vector<Light*> m_vLight;
  float m_fStrength;
  float m_fBackground;
  static AmbientLightVisitor * p_AmbientLightVisitor;
};

#endif // AMBIENTLIGHTVISITOR_H
