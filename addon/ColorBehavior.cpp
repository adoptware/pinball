//#ident "$Id: ColorBehavior.cpp,v 1.4 2003/07/16 20:02:02 rzr Exp $"
/***************************************************************************
                          ColorBehavior.cpp  -  description
                             -------------------
    begin                : Tue Feb 15 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
***************************************************************************/

#include "Private.h"
#include "ColorBehavior.h"
#include "Group.h"
#include "Shape3D.h"

ColorBehavior::ColorBehavior() : Behavior() {
}

ColorBehavior::~ColorBehavior() {
}

void ColorBehavior::onCollision(const Vertex3D &, const Vertex3D &, Group *) {
  EmAssert(this->getParent() != NULL, "ColorBehavior::onCollision parent NULL");
  EM_COUT("ColorBehavior::onCollision()" << endl, 0);
  if (this->getParent() != NULL) {
    for (int a=0; a<this->getParent()->getShape3DSize(); a++) {
      this->getParent()->getShape3D(a)->setColor(1.f, 1.f, 0.f, 0.f);
    }
  }
}

void ColorBehavior::onTick() {
  EmAssert(this->getParent() != NULL, "ColorBehavior::onTick parent NULL");
  for (int a=0; a<this->getParent()->getShape3DSize(); a++) {
    this->getParent()->getShape3D(a)->setColor(1.f, 0.f, 0.f, 1.f);
  }
}
