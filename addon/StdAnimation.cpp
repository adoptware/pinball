/***************************************************************************
                          StdAnimation.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "StdAnimation.h"
#include "Group.h"
#include "math.h"
#include "Light.h"
#include "BillBoard.h"

StdAnimation::StdAnimation(int step, int type) {
	m_iType = type;
	m_iStep = step;
	m_iTick = 0;
	m_iIndex = 0;
	m_vtxEnd.x = 0;
	m_vtxEnd.y = 0;
	m_vtxEnd.z = 0;
}

StdAnimation::~StdAnimation() {
}

void StdAnimation::add(float x, float y, float z) {
	Vertex3D vtx;
	vtx.x = x;
	vtx.y = y;
	vtx.z = z;
	m_vVertex.push_back(vtx);
}

void StdAnimation::setEnd(float x, float y, float z) {
	m_vtxEnd.x = x;
	m_vtxEnd.y = y;
	m_vtxEnd.z = z;
}

void StdAnimation::setEndStart() {
	if (m_vVertex.size() == 0) return;
	m_vtxEnd.x = m_vVertex[0].x;
	m_vtxEnd.y = m_vVertex[0].y;
	m_vtxEnd.z = m_vVertex[0].z;
}

void StdAnimation::onTick() {
	EmAssert(p_Parent != NULL, "Parent not allowed to be null");
	
	float x, y, z;
	float sX, sY, sZ;
	float eX, eY, eZ;
	float k=0.0;
  int index;
  int size = m_vVertex.size();
	
	if (m_iTick >= size*m_iStep) {
		m_iTick = 0;
	}
	
	index = (int)(m_iTick/m_iStep);
		
	sX = m_vVertex[index].x;
	sY = m_vVertex[index].y;
	sZ = m_vVertex[index].z;		
	
	if (index + 1 < size) {
		eX = m_vVertex[index+1].x;
		eY = m_vVertex[index+1].y;
		eZ = m_vVertex[index+1].z;		
	} else {
		eX = m_vtxEnd.x;
		eY = m_vtxEnd.y;
		eZ = m_vtxEnd.z;		
	}	

	k = ((float)(m_iTick%m_iStep)/m_iStep);
	x = (float)(sX + (eX - sX)*k);
	y = (float)(sY + (eY - sY)*k);
	z = (float)(sZ + (eZ - sZ)*k);

	m_iTick++;

	if (m_iType & EM_TRANSLATION) {
		p_Parent->setTranslation(x, y, z);
	}	else if (m_iType & EM_ROTATION) {
		p_Parent->setRotation(x, y, z);
	}	else if (m_iType & EM_LIGHT) {
		Light* l = p_Parent->getLight();
		if (l != NULL) {
			l->setColor(x, y, z);
		}
	} else if (m_iType & EM_BILLBOARD_SIZE) {
		BillBoard * b = p_Parent->getBillBoard();
		if (b != NULL) {
			b->setSize(x, y);
			b->setZOffset(z);
		}
	}
}

