/***************************************************************************
                          BounceBehavior.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "BounceBehavior.h"
#include "Group.h"
#include "Pinball.h"
#include "Keyboard.h"
#include "StateBehavior.h"
#include "CaveBehavior.h"
#include "Score.h"
#include <string>

#define MAX_SPEED 0.5f
#define MAX_SPEED_Y_DOWN (MAX_SPEED*0.5f)
#define SPEED_FCT 0.6f
#define Y_GRAVITY -(SPEED_FCT*0.005f) // -SPEED_FCT/200
#define Z_GRAVITY (SPEED_FCT*0.002f) //  SPEED_FCT/500
#define BORDER (SPEED_FCT*0.05f)
#define BORDER2 (SPEED_FCT*0.02f)


#define CHECK_SPEED()    \
{                        \
	float l;               \
	if ( (l=EMath::vectorLength(m_vtxDir)) > MAX_SPEED ) { \
		l /= MAX_SPEED;      \
		m_vtxDir.x /= l;     \
		m_vtxDir.y /= l;     \
		m_vtxDir.z /= l;     \
	}	                     \
  if (m_vtxDir.y > MAX_SPEED_Y_DOWN) m_vtxDir.y = MAX_SPEED_Y_DOWN; \
}


BounceBehavior::BounceBehavior(int ball) : Behavior() {
	m_iBall = ball;
	m_bAlive = false;
	m_vtxDir.x = 0.0;
	m_vtxDir.y = 0.0;
	m_vtxDir.z = 0.0;
	m_vtxOldDir.x = 0.0;
	m_vtxOldDir.y = 0.0;
	m_vtxOldDir.z = 0.0;
	m_iDirFactor = 0;
	m_iCollisionPrio = 0;
	switch (m_iBall) {
	case PBL_BALL_1: if (p_Parent != NULL) p_Parent->setTranslation(-4,0,34); break;
	case PBL_BALL_2: if (p_Parent != NULL) p_Parent->setTranslation(-8, 0, 34); break;
	case PBL_BALL_3: if (p_Parent != NULL) p_Parent->setTranslation(-12, 0, 34); break;
	default: if (p_Parent != NULL) p_Parent->setTranslation(-16, 0, 34);
	}
	this->setType(PBL_TYPE_BOUNCEBEH);
}

BounceBehavior::~BounceBehavior() {
}

/* The current direction is need by behaviors objects of other balls.
 * The upcomming direction may change, therefore we need to give the old direction. */
void BounceBehavior::getDirection(Vertex3D & vtx) {
	vtx.x = m_vtxOldDir.x;
	vtx.y = m_vtxOldDir.y;
	vtx.z = m_vtxOldDir.z;
}

void BounceBehavior::StdOnSignal() {
	EM_COUT("BounceBehavior::onSignal()", 0);
	OnSignal( PBL_SIG_RESET_ALL) {
		m_bAlive = false;
		switch (m_iBall) {
		case PBL_BALL_1: p_Parent->setTranslation(-4,0,40); break;
		case PBL_BALL_2: p_Parent->setTranslation(-8, 0, 40); break;
		case PBL_BALL_3: p_Parent->setTranslation(-12, 0, 40); break;
		default: p_Parent->setTranslation(-16, 0, 40);
		}
	}

	OnSignal( PBL_SIG_BNUDGE) {
		m_vtxDir.z -= SPEED_FCT*0.1f;
	} else
	OnSignal( PBL_SIG_TNUDGE) {
		m_vtxDir.z += SPEED_FCT*0.1f;
	} else
	OnSignal( PBL_SIG_LNUDGE) {
		m_vtxDir.x -= SPEED_FCT*0.1f;
	} else
	OnSignal( PBL_SIG_RNUDGE) {
		m_vtxDir.x += SPEED_FCT*0.1f;
	}

#define ACTIVATE_BALL				  			\
		m_bAlive = true;								\
		p_Parent->setTranslation(22, 0, 20);	\
		m_vtxDir.x = 0; 	              \
		m_vtxDir.y = 0;									\
		m_vtxDir.z = -MAX_SPEED + 0.02*SPEED_FCT*rand()/RAND_MAX + 0.08;

	// debug stuff
	if (Keyboard::isKeyDown(SDLK_a)) {
		ACTIVATE_BALL;
	}

	OnSignal( PBL_SIG_BALL1_ON ) {
		if (m_iBall == PBL_BALL_1) {
			EM_COUT("BounceBehavior::onSignal() ball 1", 0);
			ACTIVATE_BALL;
		}
	}
	OnSignal( PBL_SIG_BALL2_ON ) {
		if (m_iBall == PBL_BALL_2) {
			ACTIVATE_BALL;
		}
	}
	OnSignal( PBL_SIG_BALL3_ON ) {
		if (m_iBall == PBL_BALL_3) {
			ACTIVATE_BALL;
		}
	}
	OnSignal( PBL_SIG_BALL4_ON ) {
		if (m_iBall == PBL_BALL_4) {
			ACTIVATE_BALL;
		}
	}
#undef ACTIVATE_BALL
}

void BounceBehavior::onTick() {
	EmAssert(p_Parent != NULL, "BounceBehavior::onTick()");
	if (!m_bAlive) return;

	// reset collision
	m_iCollisionPrio = 0;

	// debug stuff
#if EM_DEBUG
	if (Keyboard::isKeyDown(SDLK_i)) m_vtxDir.z -= 0.005f;
	if (Keyboard::isKeyDown(SDLK_k)) m_vtxDir.z += 0.005f;
	if (Keyboard::isKeyDown(SDLK_j)) m_vtxDir.x -= 0.005f;
	if (Keyboard::isKeyDown(SDLK_l)) m_vtxDir.x += 0.005f;

	if (Keyboard::isKeyDown(SDLK_v)) return;
#endif

	// Gravity
	m_vtxDir.z += Z_GRAVITY;
	m_vtxDir.y += Y_GRAVITY;
	float x, y, z;
	p_Parent->getTranslation(x, y, z);
	if (y <= 0) {
		p_Parent->addTranslation(0,-y,0);
		m_vtxDir.y = EM_MAX(m_vtxDir.y, 0);
	}

	EM_COUT("BounceBehavior::onTick() " << x <<" "<< y <<" "<< z, 0);

	CHECK_SPEED();
	// move the ball
	m_vtxOldDir = m_vtxDir;
	p_Parent->addTranslation(m_vtxDir.x, m_vtxDir.y, m_vtxDir.z);

	if (z > 39) {
		m_bAlive = false;
		switch (m_iBall) {
		case PBL_BALL_1: SendSignal( PBL_SIG_BALL1_OFF, 0, this->p_Parent, NULL ); 
			p_Parent->setTranslation(-4, 0, 40); break;
		case PBL_BALL_2: SendSignal( PBL_SIG_BALL2_OFF, 0, this->p_Parent, NULL ); 
			p_Parent->setTranslation(-8, 0, 40); break;
		case PBL_BALL_3: SendSignal( PBL_SIG_BALL3_OFF, 0, this->p_Parent, NULL ); 
			p_Parent->setTranslation(-12, 0, 40); break;
		default: SendSignal( PBL_SIG_BALL4_OFF, 0, this->p_Parent, NULL );
			p_Parent->setTranslation(-16, 0, 40); break;
		}
	}
}

// each collision changes the direction of the ball
void BounceBehavior::onCollision(const Vertex3D & vtxW, const Vertex3D & vtxOwn, Group * pGroup) {
	EmAssert(p_Parent != NULL && pGroup != NULL, "BounceBehavior::onCollision()");
	if (!m_bAlive) return;
	EM_COUT("BounceBehavior::onCollision() wall " << vtxOwn.x <<" "<< vtxOwn.y <<" "<< vtxOwn.z, 0);

	// Undo last translation
	//p_Parent->addTranslation(-m_vtxDir.x, -m_vtxDir.y, -m_vtxDir.z);

	// we need a wall to do the bouncing on

	Score::getInstance()->unLockBall(m_iBall);
	// change direction depending on which type the colliding object is
	if (pGroup->getUserProperties() & (PBL_LOCK | PBL_TRAP)) {
		// trap in
		Behavior* beh = pGroup->getBehavior(0);
		EmAssert(beh != NULL, "No behavior");
		
		if (beh->getType() == PBL_TYPE_STATEBEH) {
 			if (((StateBehavior*)beh)->getOwnerBall() != m_iBall) {
 				cerr << ((StateBehavior*)beh)->getOwnerBall() << " != " << m_iBall << endl;
 				return;
 			}
		} else if (beh->getType() == PBL_TYPE_CAVEBEH) {
			// 			if (((CaveBehavior*)beh)->getOwnerBall() != m_iBall) {
			// 				cerr << ((CaveBehavior*)beh)->getOwnerBall() << " != " << m_iBall << endl;
			// 				return;
			// 			}
		} else {
			throw string("StateBehavior or CaveBehavior expected");
		}

		if (pGroup->getUserProperties() & PBL_LOCK) {
			Score::getInstance()->lockBall(m_iBall);
		}

		if (m_iCollisionPrio > 5) return;
		m_iCollisionPrio = 5;
		float tx, ty, tz;
		float bx, by, bz;
		// move the ball slowly towards the center of the cave
		pGroup->getTranslation(tx, ty, tz);
		p_Parent->getTranslation(bx, by, bz);
		m_vtxDir.x = (tx-bx)*0.1;
		m_vtxDir.y = 0;
		m_vtxDir.z = (tz-bz)*0.1;
		EM_COUT("BounceBehavior.onCollision() cave\n", 0);

	} else if (pGroup->getUserProperties() & PBL_BUMPER) {
		// bumper
		if (m_iCollisionPrio > 3) return;
		m_iCollisionPrio = 3;
		// use bumper as base
		EMath::reflectionDamp(m_vtxOldDir, vtxW, m_vtxDir, (float)1.0, (float)SPEED_FCT*0.5, 1, true);
		EM_COUT("BounceBehavior.onCollision() bumper\n", 0);

	} else if (pGroup->getUserProperties() & PBL_ACTIVE_ARM) {
		// active arm
		Behavior* beh = pGroup->getBehavior(0);
		EmAssert(beh != NULL, "No behavior");
		if (beh->getType() != PBL_TYPE_ARMBEH) {
			throw string("ArmBehavior expected in BounceBehavior.cpp");
		}
		if (m_iCollisionPrio > 3) return;
		m_iCollisionPrio = 3;
		// use the distance from the ball to the node of the arm as a function for the speed
		Vertex3D vtxArm;
		pGroup->getTranslation(vtxArm.x, vtxArm.y, vtxArm.z);
		Vertex3D vtxBall;
		p_Parent->getTranslation(vtxBall.x, vtxBall.y, vtxBall.z);
		vtxBall.x -= vtxArm.x; 
		vtxBall.y -= vtxArm.y; 
		vtxBall.z -= vtxArm.z;
		// the radius of the ball is 1 and the radius of the arm at the node is ~1 so
		// we remove this distance
		float dist = EMath::vectorLength(vtxBall) - 1.7; 
		//dist = EM_ABS(dist);
		dist = EM_MIN(EM_MAX(dist, 0.3f), 1.0f);
		cerr << dist << endl;
		// use the ball as the base
		Vertex3D vtxWall;
		vtxWall.x = -vtxOwn.x;
 		vtxWall.y = 0;
 		vtxWall.z = -vtxOwn.z;
		// add the arm to give more variation to the punch, make the arm smootly curved with lotsa polys
		vtxWall.x += vtxW.x;
		vtxWall.y = 0;
		vtxWall.z += vtxW.z;
		EMath::normalizeVector(vtxWall);
		EMath::reflectionDamp(m_vtxOldDir, vtxWall, m_vtxDir, (float)1.0, (float)SPEED_FCT*dist, 1, true);
		// move the ball slightly off the arm
		p_Parent->addTranslation(0, 0, -0.5);
		EM_COUT("BounceBehavior.onCollision() active arm\n", 0);

	} else if (pGroup->getUserProperties() & PBL_TRAP_BOUNCE) {
		// trap out
		if (m_iCollisionPrio > 3) return;
		m_iCollisionPrio = 3;
		// use the trap polygon as base
		// Bbounce ball out of cave, 0 means ignore old speed
		EMath::reflectionDamp(m_vtxOldDir, vtxW, m_vtxDir, (float)0, (float)SPEED_FCT, 1, true);
		EM_COUT("BounceBehavior.onCollision() cave bounce\n", 1);
		EM_COUT(vtxW.x << " " << vtxW.y << " " << vtxW.z, 1);

	} else if (pGroup->getUserProperties() & PBL_WALLS_ONE) {
		// oneway wall
		if (m_iCollisionPrio > 2) return;
		m_iCollisionPrio = 2;
		// use wall as base
		EMath::reflectionDamp(m_vtxOldDir, vtxW, m_vtxDir, (float)0.5, 0, 1);
		// move the ball slightly off the wall
		p_Parent->addTranslation(vtxW.x * BORDER, vtxW.y * BORDER, vtxW.z * BORDER);

	} else if (pGroup->getUserProperties() & PBL_UNACTIVE_ARM) {
		// unactive arm
		if (m_iCollisionPrio > 2) return;
		m_iCollisionPrio = 2;
		// use ball as base
		Vertex3D vtxWall;
		vtxWall.x = -vtxOwn.x;
		vtxWall.y = 0;
		vtxWall.z = -vtxOwn.z;
		EMath::reflectionDamp(m_vtxOldDir, vtxWall, m_vtxDir, (float)0.2, 0, 1);
		// move the ball slightly off the wall
		p_Parent->addTranslation(vtxWall.x * BORDER, vtxWall.y * BORDER, vtxWall.z * BORDER);
		EM_COUT("BounceBehavior.onCollision() unactive arm\n", 0);

	} else if (pGroup->getUserProperties() & PBL_HALFACTIVE_ARM) {
		// unactive arm
		if (m_iCollisionPrio > 2) return;
		m_iCollisionPrio = 2;
		// use ball as base
		Vertex3D vtxWall;
		vtxWall.x = -vtxOwn.x;
		vtxWall.y = 0;
		vtxWall.z = -vtxOwn.z;
		EMath::reflectionDamp(m_vtxOldDir, vtxWall, m_vtxDir, (float)0.2, 0, 1);
		// move the ball slightly off the wall
		p_Parent->addTranslation(vtxWall.x * BORDER, vtxWall.y * BORDER, vtxWall.z * BORDER);
		p_Parent->addTranslation(0, 0, -0.2);
		EM_COUT("BounceBehavior.onCollision() unactive arm\n", 0);

	} else if (pGroup->getUserProperties() & PBL_WALLS) {
		// walls
		if (m_iCollisionPrio > 1) return;
		m_iCollisionPrio = 1;
		Vertex3D vtxWall;
		if (vtxW.y > 0.2 || vtxW.y < -0.2) {
			// this a ramp! use the wall as a base for collision,the ball is actually a cylinder 
			// so we can not use it with ramps, but look out !!! , using the walls as a base is "evil"
			// it easily causes the ball to pass through walls 
			vtxWall.x = vtxW.x;
			vtxWall.y = vtxW.y;
			vtxWall.z = vtxW.z;
		} else {
			// else use the ball as the base
			vtxWall.x = -vtxOwn.x;
			vtxWall.y = 0;
			vtxWall.z = -vtxOwn.z;
		}
		EMath::reflectionDamp(m_vtxOldDir, vtxWall, m_vtxDir, (float)0.5, 0, 1);
		// move the ball slightly off the wall
		p_Parent->addTranslation(vtxWall.x * BORDER, vtxWall.y * BORDER, vtxWall.z * BORDER);
		EM_COUT("BounceBehavior.onCollision() walls\n" , 0);

	} else if (pGroup->getUserProperties() & (PBL_BALL_1 | PBL_BALL_2 | PBL_BALL_3 | PBL_BALL_4)) {
		// ball
		if (m_iCollisionPrio > 0) return;
		m_iCollisionPrio = 0;
		// use ball as base
		Vertex3D vtxWall;
		vtxWall.x = -vtxOwn.x;
		vtxWall.y = 0;
		vtxWall.z = -vtxOwn.z;

		Behavior* beh = pGroup->getBehavior(0);
		EmAssert(beh != NULL && beh->getType() == PBL_TYPE_BOUNCEBEH, 
						 "The behavior is not a BounceBeavior");

		Vertex3D vtxDir2, vtxPrj1, vtxPrj2;
		((BounceBehavior*)beh)->getDirection(vtxDir2);
		// the speed given from the other ball to this ball is the projection of the speed onto the wall
		EMath::projection(vtxDir2, vtxWall, vtxPrj2);
		// the speed given from this ball to the other ball is the projection of the speed
		// onto the inverse wall (which was vtxOwn).
		EMath::projection(m_vtxOldDir, vtxOwn, vtxPrj1);
		// From my physics book m1*u1 + m2u2 = m1*v1 + m2*v2. (m1 == m2 in this case)
		// Gives us v1 = u1 + vtxPrj2 - vtxPrj1. And v2 = u2 + vtxPrj1 - vtxPrj2.
		m_vtxDir.x += (vtxPrj2.x - vtxPrj1.x);
		m_vtxDir.y += (vtxPrj2.y - vtxPrj1.y);
		m_vtxDir.z += (vtxPrj2.z - vtxPrj1.z);
		EM_COUT("BounceBehavior.onCollision() ball\n", 0);
		// move the ball slightly away the other
		p_Parent->addTranslation(vtxWall.x * BORDER, vtxWall.y * BORDER, vtxWall.z * BORDER);

	} else {
		EM_COUT("BounceBehavior.onCollision() unknown\n" ,0);
	}
	
	//	p_Parent->addTranslation(m_vtxDir.x, m_vtxDir.y, m_vtxDir.z);
}

