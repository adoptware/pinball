/***************************************************************************
                          OpenGLTransVisitor.h  -  description
                             -------------------
    begin                : Sat Jan 6 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef OPENGLTRANSVISITOR_H
#define OPENGLTRANSVISITOR_H

#include "Private.h"

#include "Visitor.h"

class Group;
class Shape3D;

/** @author Henrik Enqvist */

class OpenGLTransVisitor : public Visitor  {
 protected:
	OpenGLTransVisitor();
 public:
	~OpenGLTransVisitor();
	static OpenGLTransVisitor * getInstance();
	void visit(Group* g);
 private:
	void visit(Shape3D* s, Group* g);
	static OpenGLTransVisitor * p_OpenGLTransVisitor;
};

#endif // OPENGLTRANSVISITOR_H

