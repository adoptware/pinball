/***************************************************************************
                          Light.h  -  description
                             -------------------
    begin                : Sun Jan 30 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef LIGHT_H
#define LIGHT_H

#define EM_IGNORE_ANGLE_HALF 1
#define EM_IGNORE_ANGLE_FULL 2
#define EM_IGNORE_DISTANCE 4
#define EM_USE_BOUNDS 8

class Group;

#include "Node.h"
/** Example:
 * <pre>
 * Group* groupL = new group;
 * Light* l = new Light(1.0, 10.0);
 * 
 * groupL->setLight(l);
 * engine->add(groupL);
 * 
 * engine->addLight(l);
 * </pre> */
class Light {
 public:
	/** Light(constant, linear, quadratic, r, g, b). */
	Light(float c, float l, float q, float r=1.0, float g=1.0, float b=1.0);
	~Light();
	void setOn(bool on=true) { m_bOn = on;};
	bool getOn() { return m_bOn;};
	// TODO fast clamp
	void setColor(float r, float g, float b) {
		m_fR = EM_MAX(EM_MIN(1.0, r), 0.0);
		m_fG = EM_MAX(EM_MIN(1.0, g), 0.0);	
		m_fB = EM_MAX(EM_MIN(1.0, b), 0.0);	
	};
	void getColor(float & r, float & g, float & b) { r = m_fR; g = m_fG; b = m_fB; };
	void setProperty(int);
	void unsetProperty(int);
	void setBounds(float b) { m_fBounds = b; };
	void setIndex(int i);
	Group * getParent() {
		return p_Parent;
	};
	
	Vertex3D m_vtxSrc;
	Vertex3D m_vtxTrans;
	Vertex3D m_vtxAlign;
 private:
	friend class PointLightVisitor;
	friend class TransformVisitor;
	friend class Group;
	
	void setParent(Group * p) {
		p_Parent = p;
	};
	Group * p_Parent;
	float m_fConstant;
	float m_fLinear;
	float m_fQuadratic;
	float m_fR, m_fG, m_fB;
	float m_fBounds;
	bool m_bOn;
	int m_iProperties;
};

#endif // LIGHT_H
