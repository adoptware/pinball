/***************************************************************************
                          PLVisit.h  -  description
                             -------------------
    begin                : Sun Jan 30 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef POINTLIGHTVISITOR_H
#define POINTLIGHTVISITOR_H

#include "Private.h"

#include "Visitor.h"
#include "EMath.h"

class Light;
class Group;
class Shape3D;
/**
 * <p><i>Only for internal use.</i></p>
 */
class PointLightVisitor : public Visitor {
	public:
		PointLightVisitor(int size = 4);
		~PointLightVisitor();
		void add(Light* l);
		void visit(Group* g);
	private:
		vector<Light*> m_vLight;
		void visit(Shape3D* s, Group* g);
};

#endif // POINTLIGHTVISITOR_H
