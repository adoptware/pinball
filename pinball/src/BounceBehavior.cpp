/***************************************************************************
                          BouBeh.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by
    email                :
 ***************************************************************************/

#include <stdlib.h>
#include "BounceBehavior.h"
#include "Group.h"
#include "Pinball.h"
#include "Keyboard.h"

#define MAX_SPEED 0.8f
#define Y_GRAVITY -MAX_SPEED*0.013333 // -MAX_SPEED/75
#define Z_GRAVITY MAX_SPEED*0.004 //  MAX_SPEED/250
#define BORDER 0.1f

BounceBehavior::BounceBehavior(int ball) {
	m_iBall = ball;
	m_bAlive = false;
	m_bKnock = false;
	m_vtxDir.x = 0.0;
	m_vtxDir.y = 0.0;
	m_vtxDir.z = 0.0;
	m_vtxNextDir.x = 0.0;
	m_vtxNextDir.y = 0.0;
	m_vtxNextDir.z = 0.0;
	m_iCollisionType = 0;
	switch (m_iBall) {
		case 1: if (p_Parent != NULL) p_Parent->setTranslation(-4,0,34); break;
		case 2: if (p_Parent != NULL) p_Parent->setTranslation(-8, 0, 34); break;
		case 3: if (p_Parent != NULL) p_Parent->setTranslation(-12, 0, 34); break;
		default: if (p_Parent != NULL) p_Parent->setTranslation(-16, 0, 34);
	}
}

BounceBehavior::~BounceBehavior() {
}

void BounceBehavior::getDirection(Vertex3D & vtx) {
	vtx.x = m_vtxDir.x;
	vtx.y = m_vtxDir.y;
	vtx.z = m_vtxDir.z;
}

void BounceBehavior::StdOnSignal() {
	EM_COUT("BounceBehavior::onSignal()", 0);
	OnSignal( PBL_SIG_RESET_ALL) {
		m_bAlive = false;
		switch (m_iBall) {
			case 1: p_Parent->setTranslation(-4,0,34); break;
			case 2: p_Parent->setTranslation(-8, 0, 34); break;
			case 3: p_Parent->setTranslation(-12, 0, 34); break;
			default: p_Parent->setTranslation(-16, 0, 34);
		}
	}
	#define ACTIVATE_BALL									\
		m_bAlive = true;										\
		p_Parent->setTranslation(1, 0, 29);	\
		m_vtxNextDir.x = -0.2f*MAX_SPEED; 	\
		m_vtxNextDir.y = 0;									\
		m_vtxNextDir.z = -0.88*MAX_SPEED + 0.02*MAX_SPEED*rand()/RAND_MAX;

	OnSignal( PBL_SIG_ACTIVATE_BALL_1 ) {
		if (m_iBall == 1) {
			ACTIVATE_BALL
		}
	}
	OnSignal( PBL_SIG_ACTIVATE_BALL_2 ) {
		if (m_iBall == 2) {
			ACTIVATE_BALL
		}
	}
	OnSignal( PBL_SIG_ACTIVATE_BALL_3 ) {
		if (m_iBall == 3) {
			ACTIVATE_BALL
		}
	}
	OnSignal( PBL_SIG_ACTIVATE_BALL_4 ) {
		if (m_iBall == 4) {
			ACTIVATE_BALL
		}
	}
	#undef ACTIVATE_BALL
}

void BounceBehavior::onTick() {
	EmAssert(p_Parent != NULL, "BounceBehavior::onTick()");
	if (!m_bAlive) return;

	if (Keyboard::isKeyDown(KEY_I)) m_vtxNextDir.z -= 0.01f;
	if (Keyboard::isKeyDown(KEY_K)) m_vtxNextDir.z += 0.01f;
	if (Keyboard::isKeyDown(KEY_J)) m_vtxNextDir.x -= 0.01f;
	if (Keyboard::isKeyDown(KEY_L)) m_vtxNextDir.x += 0.01f;

	if (Keyboard::isKeyDown(KEY_SPACE)) {
		if (!m_bKnock) {
			m_bKnock = true;
			m_vtxNextDir.z -= MAX_SPEED*0.2f;
		}
	} else {
		m_bKnock = false;
	}

	m_iCollisionType = 0;
	m_vtxDir = m_vtxNextDir;
	// Gravity
	m_vtxDir.z += Z_GRAVITY;
	
	float x, y, z;
	p_Parent->getTranslation(x, y, z);
	if (y <= 0) {
		m_vtxDir.y = 0;
		p_Parent->setTranslation(x, 0, z);
	} else {
		m_vtxDir.y += Y_GRAVITY;
	}
	m_vtxNextDir = m_vtxDir;
	p_Parent->addTranslation(m_vtxDir.x, m_vtxDir.y, m_vtxDir.z);

	if (z > 33) {
		m_bAlive = false;
		switch (m_iBall) {
			case 1: SendSignal( PBL_SIG_BALL_1_DEAD ); break;
			case 2: SendSignal( PBL_SIG_BALL_2_DEAD ); break;
			case 3: SendSignal( PBL_SIG_BALL_3_DEAD ); break;
			default: SendSignal( PBL_SIG_BALL_4_DEAD );
		}
	}
}

void BounceBehavior::onCollision(const Vertex3D & vtxW, const Vertex3D & vtxOwn, Group * pGroup) {
	Vertex3D vtxWall;
	vtxWall.x = -vtxOwn.x;
	vtxWall.y = -vtxOwn.y;
	vtxWall.z = -vtxOwn.z;
	EmAssert(p_Parent != NULL && pGroup != NULL, "BounceBehavior::onCollision()");
	EM_COUT("BounceBehavior::onCollision() wall " << vtxWall.x <<" "<< vtxWall.y <<" "<< vtxWall.z, 0);
	if (!m_bAlive) return;
	// Undo last translation
	p_Parent->addTranslation(-m_vtxNextDir.x, -m_vtxNextDir.y, -m_vtxNextDir.z);
	if (pGroup->getUserProperties() & PBL_BUMPER && m_iCollisionType < PBL_BUMPER) {
		m_iCollisionType = PBL_BUMPER;
		EMath::reflectionDamp(m_vtxNextDir, vtxWall, m_vtxNextDir, (float)1.0, (float)MAX_SPEED*0.5, 1, true);
		EM_COUT("BounceBehavior.onCollision() bumper\n", 0);
	} else
	if (pGroup->getUserProperties() & PBL_ACTIVE_ARM && m_iCollisionType < PBL_ACTIVE_ARM) {
		m_iCollisionType = PBL_ACTIVE_ARM;
		EMath::reflectionDamp(m_vtxNextDir, vtxWall, m_vtxNextDir, (float)1.0, (float)MAX_SPEED, 1, true);
		EM_COUT("BounceBehavior.onCollision() active arm\n", 0);
	} else
	if (pGroup->getUserProperties() & PBL_TRAP_BOUNCE && m_iCollisionType < PBL_TRAP_BOUNCE) {
		m_iCollisionType = PBL_TRAP_BOUNCE;
		EMath::reflectionDamp(m_vtxNextDir, vtxWall, m_vtxNextDir, (float)0, (float)MAX_SPEED*0.5, 1, true);
		EM_COUT("BounceBehavior.onCollision() cave bounce\n", 0);
	} else
	if (pGroup->getUserProperties() & PBL_UNACTIVE_ARM && m_iCollisionType < PBL_UNACTIVE_ARM) {
		m_iCollisionType = PBL_UNACTIVE_ARM;
		EMath::reflectionDamp(m_vtxNextDir, vtxWall, m_vtxNextDir, (float)0.2, 0, 1);
		EM_COUT("BounceBehavior.onCollision() unactive arm\n", 0);
	} else
	if (pGroup->getUserProperties() & PBL_WALLS && m_iCollisionType < PBL_WALLS) {
		m_iCollisionType = PBL_WALLS;
		EMath::reflectionDamp(m_vtxNextDir, vtxWall, m_vtxNextDir, (float)0.5, 0, 1);
		EM_COUT("BounceBehavior.onCollision() walls\n" , 0);
	} else
	if (pGroup->getUserProperties() & (PBL_BALL_1 | PBL_BALL_2 | PBL_BALL_3 | PBL_BALL_4) && 
			m_iCollisionType < PBL_BALL_1) {
		m_iCollisionType = PBL_BALL_1;
		BounceBehavior* beh = (BounceBehavior*) pGroup->getBehavior();
		if (beh != NULL) {
			Vertex3D vtxDir2;
			beh->getDirection(vtxDir2);
			EMath::projection(vtxDir2, vtxWall, vtxDir2);
			EMath::reflectionDamp(m_vtxNextDir, vtxWall, m_vtxNextDir, (float)1, 0, (float)0.5);
			m_vtxNextDir.x += vtxDir2.x*0.5;
			m_vtxNextDir.y += vtxDir2.y*0.5;
			m_vtxNextDir.z += vtxDir2.z*0.5;
		}
		EM_COUT("BounceBehavior.onCollision() ball\n", 0);
	} else
	if (pGroup->getUserProperties() & PBL_TRAP && m_iCollisionType < PBL_TRAP) {
		m_iCollisionType = PBL_TRAP;
		float tx, ty, tz;
		float bx, by, bz;
		pGroup->getTranslation(tx, ty, tz);
		p_Parent->getTranslation(bx, by, bz);
		m_vtxNextDir.x = (tx-bx)*0.1;
		m_vtxNextDir.y = 0;
		m_vtxNextDir.z = (tz-bz)*0.1;
		EM_COUT("BounceBehavior.onCollision() cave\n", 0);
	} else {
		EM_COUT("BounceBehavior.onCollision() unknown\n" ,0);
	}
	// allowed max speed
	float l;
	if ( (l=EMath::vectorLength(m_vtxDir)) > MAX_SPEED ) {
		l /= MAX_SPEED;
		m_vtxNextDir.x /= l;
		m_vtxNextDir.y /= l;
		m_vtxNextDir.z /= l;
	}	
	p_Parent->addTranslation(m_vtxNextDir.x, m_vtxNextDir.y, m_vtxNextDir.z);
}

