/***************************************************************************
                          KeyBeh.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/


#include "EyeBehavior.h"
#include "Group.h"
#include "Pinball.h"

#define TX 0 
#define TY 30
#define TZ 6
#define RX 0.23f
#define RY 0
#define RZ 0

EyeBehavior::EyeBehavior(Group * g) : Behavior() {
	m_gBall = g;
}

EyeBehavior::~EyeBehavior() {
}

void EyeBehavior::onTick() {
	EmAssert(m_gBall != NULL, "Ball group NULL");
	EmAssert(p_Parent != NULL, "Parent group NULL");

	float bx, by, bz, ex, ey, ez, sx, sy, sz;
	m_gBall->getTranslation(bx, by, bz);
	p_Parent->getTranslation(ex, ey, ez);

	sx = (TX+bx*0.4) - ex;
	sy = (TY+by*0.4) - ey;
	sz = (TZ+bz*0.4) - ez;

	p_Parent->addTranslation(sx*0.1f, sy*0.1, sz*0.1f);
	p_Parent->setRotation(RX, RY, RZ);
}

#undef TX
#undef TY
#undef TZ
#undef RX
#undef RY
#undef RZ
