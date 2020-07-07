/***************************************************************************
                          OpenGLVisitor.h  -  description
                             -------------------
    begin                : Sat Jan 6 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef OPENGLVISITOR_H
#define OPENGLVISITOR_H

#include "RenderVisitor.h"

#define EM_GL_GCOL_TEX        0 
#define EM_GL_GCOL_TEX_TRANS  2
#define EM_GL_CLEAN           4


class OpenGLVisitor
  : public RenderVisitor
{
public:
  static OpenGLVisitor* getInstance();
public:
  virtual void visit(Group* g);
  virtual void empty();
protected:
  OpenGLVisitor();
  virtual ~OpenGLVisitor();
protected:
  static OpenGLVisitor* p_OpenGLVisitor;
};

#endif // OPENGLVISITOR_H

