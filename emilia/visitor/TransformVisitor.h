/***************************************************************************
                          TVisitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef TRANSFORMVISITOR_H
#define TRANSFORMVISITOR_H

#include "Private.h"

#include "Visitor.h"
#include "EMath.h"

/**
 * <p><i>Only for internal use.</i></p>
 */
class TransformVisitor : public Visitor {
 public:
	TransformVisitor();
	~TransformVisitor();
	void setCamera(Group*);
	void visit(Group*);
	void empty();
 private:
	Group* p_GroupCamera;
};

#endif // TRANSFORMVISITOR_H
