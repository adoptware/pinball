/***************************************************************************
                          StateBehavior.cpp  -  description
                             -------------------
    begin                : Thu Jan 25 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#include "StateBehavior.h"
#include "Pinball.h"
#include "Group.h"
#include "Light.h"
#include "Shape3D.h"
#include "Polygon.h"
#include "SoundUtil.h"

StateItem::StateItem(int asig, int csig, int delay) {
	m_iActSig = asig;
	m_iCollSig = csig;
	m_iSigDelay = 0;
	m_bLight = false;
	m_iProperty = PBL_NULL;
	m_iSound = -1;
	m_iDelay = delay;
	m_iTick = 0;
	p_NextStateItem = NULL;
}

StateItem::~StateItem() {
}

void StateItem::setTr(float x, float y, float z, float speed) {
	m_vtxTr.x = x;
	m_vtxTr.y = y;
	m_vtxTr.z = z;
	m_fTrSpeed = EM_ABS(speed);
}

void StateItem::setRot(float x, float y, float z, float speed) {
	m_vtxRot.x = x;
	m_vtxRot.y = y;
	m_vtxRot.z = z;
	m_fRotSpeed = EM_ABS(speed);
}

void StateItem::addTexCoord(float u, float v) { 
	TexCoord tex;
	tex.u = u;
	tex.v = v;
	m_vTexCoord.push_back(tex); 
}


StateBehavior::StateBehavior() : Behavior() {
	m_iOwnerBall = PBL_NULL;
	m_iTick = 0;
	m_iCollisionSafe = 0;
	m_bProperty = false;
	m_bTexCoord = false;
	m_bMove = false;
	p_Light = NULL;
	p_CurrentStateItem = NULL;
	this->setType(PBL_TYPE_STATEBEH);
}

StateBehavior::~StateBehavior(){
}

void StateBehavior::addStateItem(StateItem* stateitem) {
	// insert state
	m_vStateItem.push_back(stateitem);
	if (m_vStateItem.size() == 1) {
		p_CurrentStateItem = stateitem;
	}
	// make a loop of next pointers
	if (m_vStateItem.size() > 1) {
		m_vStateItem[m_vStateItem.size()-2]->p_NextStateItem = stateitem;
	}
	stateitem->p_NextStateItem = m_vStateItem[0];
}

void StateBehavior::onTick() {
	EmAssert(p_Parent != NULL, "Parent NULL");
	EmAssert(p_CurrentStateItem != NULL, "StateItem NULL");

	m_iTick++;
	p_CurrentStateItem->m_iTick++;

	if (p_CurrentStateItem->m_iDelay != 0 && 
			p_CurrentStateItem->m_iTick >= p_CurrentStateItem->m_iDelay &&
			p_CurrentStateItem->p_NextStateItem != NULL) {
		this->setState(p_CurrentStateItem->p_NextStateItem);
	}

	// the ball recently collided with this object is in a safe distance
	if (m_iTick > m_iCollisionSafe) {
		m_iOwnerBall = PBL_NULL;
	}

	// move the ball to toward next spot
	if (m_bMove) {
		float tx, ty, tz, rx, ry, rz;
		p_Parent->getTranslation(tx, ty, tz);
		p_Parent->getRotation(rx, ry, rz);
		
		float dtx, dty, dtz, drx, dry, drz;
		dtx =  p_CurrentStateItem->m_vtxTr.x - tx;
		dty =  p_CurrentStateItem->m_vtxTr.y - ty;
		dtz =  p_CurrentStateItem->m_vtxTr.z - tz;
		drx =  p_CurrentStateItem->m_vtxRot.x - rx;
		dry =  p_CurrentStateItem->m_vtxRot.y - ry;
		drz =  p_CurrentStateItem->m_vtxRot.z - rz;

		dtx = EM_MAX(EM_MIN(dtx, p_CurrentStateItem->m_fTrSpeed), -p_CurrentStateItem->m_fTrSpeed);
		dty = EM_MAX(EM_MIN(dty, p_CurrentStateItem->m_fTrSpeed), -p_CurrentStateItem->m_fTrSpeed);
		dtz = EM_MAX(EM_MIN(dtz, p_CurrentStateItem->m_fTrSpeed), -p_CurrentStateItem->m_fTrSpeed);
		drx = EM_MAX(EM_MIN(drx, p_CurrentStateItem->m_fRotSpeed), -p_CurrentStateItem->m_fRotSpeed);
		dry = EM_MAX(EM_MIN(dry, p_CurrentStateItem->m_fRotSpeed), -p_CurrentStateItem->m_fRotSpeed);
		drz = EM_MAX(EM_MIN(drz, p_CurrentStateItem->m_fRotSpeed), -p_CurrentStateItem->m_fRotSpeed);
		
		p_Parent->addTransform(dtx, dty, dtz, drx, dry, drz);
	}
}

void StateBehavior::setState(StateItem* stateitem) {
	p_Parent->unsetUserProperty(p_CurrentStateItem->m_iProperty);
	p_CurrentStateItem = stateitem;

	// set properties
	p_Parent->setUserProperty(p_CurrentStateItem->m_iProperty);
	// apply texcoords
	if (m_bTexCoord) {
		/* the second shape is the visible one*/    
		Shape3D* shape = p_Parent->getShape3D(0);   
		if (shape != NULL) {         
			/* only apply to first polygon */         
			Polygon* poly = shape->getPolygon(0);     
			if (poly != NULL) {                       
				vector<TexCoord>::iterator iter1 = p_CurrentStateItem->m_vTexCoord.begin();               
				vector<TexCoord>::iterator end1 = p_CurrentStateItem->m_vTexCoord.end();                  
				vector<TexCoord>::iterator iter2 = poly->m_vTexCoord.begin();  
				vector<TexCoord>::iterator end2 = poly->m_vTexCoord.end();     
				for (; iter1 != end1 && iter2 != end2; iter1++, iter2++) {       
					(*iter2) = (*iter1);                  
				}                                       
			}                                         
		}
	}
	// apply light
	SetLightOn(p_CurrentStateItem->m_bLight);
	// zero counter
	p_CurrentStateItem->m_iTick = 0;
}

void StateBehavior::StdOnSignal() {
	EmAssert(p_Parent != NULL, "Parent NULL");
	EmAssert(p_CurrentStateItem != NULL, "StateItem NULL");
	EmAssert(m_vStateItem.size() > 0, "No StateItems");

	OnSignal( PBL_SIG_RESET_ALL) {
		this->setState(m_vStateItem[0]);
		// do a fast move
		if (m_bMove) {
			p_Parent->setTransform(p_CurrentStateItem->m_vtxTr.x,
														 p_CurrentStateItem->m_vtxTr.y,
														 p_CurrentStateItem->m_vtxTr.z,
														 p_CurrentStateItem->m_vtxRot.x,
														 p_CurrentStateItem->m_vtxRot.y,
														 p_CurrentStateItem->m_vtxRot.z);
			
		}
	} else {
		vector<StateItem*>::iterator iter = m_vStateItem.begin();
		vector<StateItem*>::iterator end = m_vStateItem.end();
		for (; iter != end; iter++) {
			OnSignal((*iter)->m_iActSig) {
				this->setState(*iter);
			}
		}
	}
}

void StateBehavior::StdOnCollision() {
	EmAssert(p_CurrentStateItem != NULL, "StateItem NULL");
	EM_COUT("StateBehavior::StdOnCollision()", 0);

	OnCallerProperty( (PBL_BALL_1 | PBL_BALL_2 | PBL_BALL_3 | PBL_BALL_4) ) {
		// this is to avaid one collision to generate more than one signal
		if (m_iTick <= m_iCollisionSafe) {
			m_iCollisionSafe = m_iTick + 5;
			return;
		}
		
		m_iOwnerBall = GetCallerProperty() & (PBL_BALL_1 | PBL_BALL_2 | PBL_BALL_3 | PBL_BALL_4);
		m_iCollisionSafe = m_iTick + 5;
		SoundUtil::getInstance()->play(p_CurrentStateItem->m_iSound, false);
		SendSignal(p_CurrentStateItem->m_iCollSig, p_CurrentStateItem->m_iSigDelay, this->p_Parent, NULL);
	}
}
