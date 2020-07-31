/***************************************************************************
                          PointLightVisit.h  -  description
                             -------------------
    begin                : Sun Jan 30 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef POINTLIGHTVISITOR_H
#define POINTLIGHTVISITOR_H

#include <vector>

#include "Visitor.h"
#include "EMath.h"

class Light;
class Group;
class Shape3D;

/** Only for internal use. */
class PointLightVisitor : public Visitor {
 protected:
	PointLightVisitor(int size = 4);
 public:
	~PointLightVisitor();
	static PointLightVisitor * getInstance();
	void add(Light* l);
	void visit(Group* g);
	void clear();
 private:
	vector<Light*> m_vLight;
	void visit(Shape3D* s, Group* g);
	static PointLightVisitor * p_PointLightVisitor;
};

#endif // POINTLIGHTVISITOR_H
