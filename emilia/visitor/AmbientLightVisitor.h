/***************************************************************************
                          PLVisit.h  -  description
                             -------------------
    begin                : Sun Jan 30 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef AMBIENTLIGHTVISITOR_H
#define AMBIENTLIGHTVISITOR_H

#include "Private.h"

#include "Visitor.h"
#include "EMath.h"

class Light;
class Group;
class Shape3D;
/**
 * <p><i>Only for internal use.</i></p>
 */
class AmbientLightVisitor : public Visitor {
	public:
		AmbientLightVisitor();
		~AmbientLightVisitor();
		void setLightning(float s, float bg);
		void add(Light* l);
		void visit(Group* g);
	private:
		vector<Light*> m_vLight;
		float m_fStrength;
		float m_fBackground;
};

#endif // POINTLIGHTVISITOR_H
