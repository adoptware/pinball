/***************************************************************************
                          TVisitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef TRANSFORMVISITOR_H
#define TRANSFORMVISITOR_H

#include "Visitor.h"
#include "EMath.h"

/** Only for internal use. */
class TransformVisitor : public Visitor {
 protected:
	TransformVisitor();
 public:
	~TransformVisitor();
	static TransformVisitor * getInstance();
	void setCamera(Group*);
	void visit(Group*);
	void empty();
 private:
	Group* p_GroupCamera;
	static TransformVisitor * p_TransformVisitor;
};

#endif // TRANSFORMVISITOR_H
