/***************************************************************************
                          ColBeh.cpp  -  description
                             -------------------
    begin                : Tue Feb 15 2000
    copyright            : (C) 2000 by 
    email                :
 ***************************************************************************/

#include "ColorBehavior.h"
#include "Group.h"
#include "Shape3D.h"

ColorBehavior::ColorBehavior() {
}

ColorBehavior::~ColorBehavior() {
}

void ColorBehavior::onCollision(const Vertex3D & vtxWall, const Vertex3D & vtxOwn, Group * g) {
	EM_COUT("ColorBehavior::onCollision()" << endl, 0);
	if (p_Parent != NULL) {
		for (int a=0; a<p_Parent->getShape3DSize(); a++) {
			p_Parent->getShape3D(a)->setColor(1.0, 1.0, 0.0, 0.0);
		}
	}
}

void ColorBehavior::onTick() {
	if (p_Parent != NULL) {
		for (int a=0; a<p_Parent->getShape3DSize(); a++) {
			p_Parent->getShape3D(a)->setColor(1.0, 0.0, 0.0, 1.0);
		}
	}
}

