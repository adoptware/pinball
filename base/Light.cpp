/***************************************************************************
                          Light.cpp  -  description
                             -------------------
    begin                : Sun Jan 30 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "Light.h"

Light::Light(float c, float l, float q, float r, float g, float b) {
	m_bOn = true;
	m_fConstant = EM_MAX(c, 0.0);
	m_fLinear = EM_MAX(l, 0.0);
	m_fQuadratic = EM_MAX(q, 0.0);
	m_fBounds = 1.0;
	m_iProperties = 0;
	m_fR = EM_MAX(EM_MIN(1.0, r), 0.0);
	m_fG = EM_MAX(EM_MIN(1.0, g), 0.0);
	m_fB = EM_MAX(EM_MIN(1.0, b), 0.0);
	m_vtxSrc.x = 0;
	m_vtxSrc.y = 0;
	m_vtxSrc.z = 0;
	//	m_vtxTrans = {0,0,0};
	//	m_vtxAlign = {0,0,0};
}

Light::~Light() {
}

void Light::setProperty(int p) {
	m_iProperties |= p;
}

void Light::unsetProperty(int p) {
	m_iProperties -= (m_iProperties & p);
}
