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

void ColorBehavior::onCollision(const Vertex3D & vtxWall, const Vertex3D & vtxOwn, Group * g) {
	EmAssert(this->getParent() != NULL, "ColorBehavior::onCollision parent NULL");
	EM_COUT("ColorBehavior::onCollision()" << endl, 0);
	if (this->getParent() != NULL) {
		for (int a=0; a<this->getParent()->getShape3DSize(); a++) {
			this->getParent()->getShape3D(a)->setColor(1.0, 1.0, 0.0, 0.0);
		}
	}
}

void ColorBehavior::onTick() {
	EmAssert(this->getParent() != NULL, "ColorBehavior::onTick parent NULL");
	for (int a=0; a<this->getParent()->getShape3DSize(); a++) {
		this->getParent()->getShape3D(a)->setColor(1.0, 0.0, 0.0, 1.0);
	}
}

