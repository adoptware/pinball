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
#include "Config.h"
#include "Score.h"

#define TX 0 
#define TY 30
#define TZ 6
#define RX 0.23f
#define RY 0
#define RZ 0

EyeBehavior::EyeBehavior(Group * g1, Group * g2, Group * g3) : Behavior() {
	p_gBall1 = g1;
	p_gBall2 = g2;
	p_gBall3 = g3;
	p_Score = Score::getInstance();
}

EyeBehavior::~EyeBehavior() {
}

void EyeBehavior::onTick() {
	EmAssert(p_gBall1 != NULL, "Ball1 group NULL");
	EmAssert(p_gBall2 != NULL, "Ball2 group NULL");
	EmAssert(p_gBall3 != NULL, "Ball3 group NULL");
	EmAssert(p_Parent != NULL, "Parent group NULL");

	if (Config::getInstance()->getView() == 1) {
		p_Parent->setTransform(0, 25, 25, 0.175f, 0, 0);
		return;
	}

	float bx=0, by=0, bz=0;
	int balls = 0;
	float ex, ey, ez, sx, sy, sz;

	if (p_Score->isBallActive(PBL_BALL_1)) {
		float x, y, z;
		p_gBall1->getTranslation(x, y, z);
		bx = x; by = y; bz = z;
		balls++;
	}
	if (p_Score->isBallActive(PBL_BALL_2)) {
		float x, y, z;
		p_gBall2->getTranslation(x, y, z);
		if (balls > 0) {
			bx += x; 
			by = EM_MAX(by, y); 
			bz = EM_MAX(bz, z);
		} else {
			bx = x; by = y; bz = z;
		}
		balls++;
	}
	if (p_Score->isBallActive(PBL_BALL_3)) {
		float x, y, z;
		p_gBall3->getTranslation(x, y, z);
		if (balls > 0) {
			bx += x; 
			by = EM_MAX(by, y); 
			bz = EM_MAX(bz, z);
		} else {
			bx = x; by = y; bz = z;
		}
		balls++;
	}
	if (balls > 0) {
		bx /= balls;
	}

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
