/***************************************************************************
                          OpenGLVisitor.h  -  description
                             -------------------
    begin                : Sat Jan 6 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef OPENGLVISITOR_H
#define OPENGLVISITOR_H

#include "Visitor.h"

#define EM_GL_GCOL_TEX        0 
#define EM_GL_GCOL_TEX_TRANS  2
#define EM_GL_CLEAN           4

class Group;
class Shape3D;

/** @author Henrik Enqvist */

class OpenGLVisitor : public Visitor  {
 protected:
	OpenGLVisitor();
 public:
	~OpenGLVisitor();
	static OpenGLVisitor * getInstance();
	void visit(Group* g);
	void empty();
	int getMode() { return m_iMode; };
	void setMode(int m) { m_iMode = m; };
 private:
	int m_iMode;
	bool m_bOffset;
	void visit(Shape3D* s, Group* g);
	static OpenGLVisitor * p_OpenGLVisitor;
};

#endif // OPENGLVISITOR_H

