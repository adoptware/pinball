/***************************************************************************
                          TriggerBehavior.cpp  -  description
                             -------------------
    begin                : Thu Jan 25 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#include "TriggerBehavior.h"
#include "Pinball.h"
#include "Group.h"
#include "Light.h"
#include "Shape3D.h"
#include "Polygon.h"

TriggerBehavior::TriggerBehavior(int aSignal, int uSignal, int aSignalOut, int uSignalOut, bool init) 
	: Behavior() {
	m_iActiveSignal = aSignal;
	m_iUnActiveSignal = uSignal;
	m_iActiveSignalOut = aSignalOut;
	m_iUnActiveSignalOut = uSignalOut;
	m_iTick = 0;
	m_iCollisionSafe = 0;
	m_iMoveEnd = 0;
	m_bTexCoord = false;
	m_bMove = false;
	m_bActive = init;
	m_bInit = init;
	p_Light = NULL;
	m_vtxRot.x = 0;
	m_vtxRot.y = 0;
	m_vtxRot.z = 0;
	this->setType(PBL_TYPE_TRIGGERBEH);
}

TriggerBehavior::~TriggerBehavior(){
}


#define SET_TEX(vTex) {                       \
	/* the second shape is the visible one*/    \
	Shape3D* shape = p_Parent->getShape3D(1);   \
	if (m_bTexCoord && shape != NULL) {         \
  	/* only apply to first polygon */         \
		Polygon* poly = shape->getPolygon(0);     \
		if (poly != NULL) {                       \
			vector<TexCoord>::iterator iter1 = vTex.begin();               \
			vector<TexCoord>::iterator end1 = vTex.end();                  \
			vector<TexCoord>::iterator iter2 = poly->m_vTexCoord.begin();  \
			vector<TexCoord>::iterator end2 = poly->m_vTexCoord.end();     \
			for (; iter1 != end1, iter2 != end2; iter1++, iter2++) {       \
				(*iter2) = (*iter1);                  \
			}                                       \
		}                                         \
	}                                           \
}

void TriggerBehavior::setTexCoordAct() {
	SET_TEX(m_vTexCoordAct);
	EM_COUT("TriggerBehavior::setTexCoordAct()", 0);
}

void TriggerBehavior::setTexCoordUn() {
	SET_TEX(m_vTexCoordUn);
	EM_COUT("TriggerBehavior::setTexCoordUn()", 0);
}

#undef SET_TEX

void TriggerBehavior::onTick() {
	m_iTick++;
	if (!m_bActive) {
		if (m_iTick < m_iMoveEnd && m_bMove) {
			m_vtxRot.x += m_vtxActRot.x;
			m_vtxRot.y += m_vtxActRot.y;
			m_vtxRot.z += m_vtxActRot.z;
			p_Parent->setRotation(m_vtxRot.x, m_vtxRot.y, m_vtxRot.z);
		}
		//&& p_Light != NULL) p_Light->setOn(false);
	} else {
		if (m_iTick < m_iMoveEnd && m_bMove) {
			m_vtxRot.x += m_vtxUnRot.x;
			m_vtxRot.y += m_vtxUnRot.y;
			m_vtxRot.z += m_vtxUnRot.z;
			p_Parent->setRotation(m_vtxRot.x, m_vtxRot.y, m_vtxRot.z);
		}
	} //if (p_Light != NULL) p_Light->setOn(true);
}

void TriggerBehavior::StdOnSignal() {
	OnSignal( PBL_SIG_RESET_ALL) {
		m_bActive = m_bInit;
		if (m_bInit) { 
			m_bActive = true;
			SetLightOn(true);
			if (m_bTexCoord) this->setTexCoordAct();
		} else {
			m_bActive = false;
			SetLightOn(false);
			if (m_bTexCoord) this->setTexCoordUn();
		}
		if (m_bMove) {
			p_Parent->setRotation(0,0,0);
		}
	} else
	OnSignal( m_iActiveSignal) {
		if (!m_bActive) {
			m_bActive = true;
			SetLightOn(true);
			if (m_bTexCoord) this->setTexCoordAct();
			if (m_bMove) {
				m_iMoveEnd = m_iTick + 20;
			}
		}
	} else
	OnSignal( m_iUnActiveSignal) {
		if (m_bActive) {
			m_bActive = false;
			SetLightOn(false);
			if (m_bMove) {
				m_iMoveEnd = m_iTick + 20;
			}
			if (m_bTexCoord) this->setTexCoordUn();
		}
	}
}

void TriggerBehavior::StdOnCollision() {
	OnCallerProperty( (PBL_BALL_1 | PBL_BALL_2 | PBL_BALL_3 | PBL_BALL_4) ) {
		// this is to avaid one collision to generate more than one signal
		if (m_iTick < m_iCollisionSafe) {
			m_iCollisionSafe = m_iTick + 3;
			return;
		}
		m_iCollisionSafe = m_iTick + 3;
		if (m_bActive) {
			SendSignal(m_iActiveSignalOut, 0, this->p_Parent, NULL);
		} else {
			SendSignal(m_iUnActiveSignalOut, 0, this->p_Parent, NULL);
		}
	}
}
