/***************************************************************************
                          TexAnimation.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "TexAnimation.h"
#include "Group.h"
#include "math.h"
#include "Light.h"
#include "BillBoard.h"

/*
 */
TexAnimation::TexAnimation(int step, int nvtx, int type) {
	m_iType = type;
	m_iStep = step;
	m_iTick = 0;
	m_iTexCoord = nvtx;
}

/*
 */
TexAnimation::~TexAnimation() {
}

/*
 */
void TexAnimation::add(float u, float v) {
	TexCoord tex;
	tex.u = u;
	tex.v = v;
	m_vTexCoord.push_back(tex);
}

/*
 */
void TexAnimation::onTick() {
	EmAssert(p_Parent != NULL, "Parent not allowed to be null");
	
	float sU, sV, eU, eV, k, u, v;
  int size = (int)(m_vTexCoord.size()/m_iTexCoord);
	
	if (m_iTick >= size*m_iStep) {
		m_iTick = 0;
	}
	
	int index = (int)(m_iTick/m_iStep);

	for (int a=0; a<m_iTexCoord && a<4; a++) {
		sU = m_vTexCoord[index*m_iTexCoord + a].u;
		sV = m_vTexCoord[index*m_iTexCoord + a].v;
	
		if (index + 1 < size) {
			eU = m_vTexCoord[(index+1)*m_iTexCoord + a].u;
			eV = m_vTexCoord[(index+1)*m_iTexCoord + a].v;
		} else {
			eU = m_vTexCoord[a].u;
			eV = m_vTexCoord[a].v;
		}	

		/*
		k = ((float)(m_iTick%(m_iStep*m_iTexCoord))/m_iStep);
		u = (float)(sU + (eU - sU)*k);
		v = (float)(sV + (eV - sV)*k);
		*/
		u = sU;
		v = sV;
		
		BillBoard * b = p_Parent->getBillBoard();
		if (b != NULL) {
			b->setUV(u, v, a);
			EM_COUT("TexAnimation::onTick() " << u <<" "<< v <<" size "<< size <<" index "<< 
							index <<" tick "<< m_iTick, 0);
		}
	}

	m_iTick++;
}

