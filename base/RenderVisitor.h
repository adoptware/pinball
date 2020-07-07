// -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
// SPDX-License-Identifier: GPL-2.0+
// Copyright: Henrik Enqvist and contributors 2001-present

/***************************************************************************
                          RenderVisitor.h  -  description
                             -------------------
    begin                : Sat Jan 6 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef RENDERVISITOR_H
#define RENDERVISITOR_H

#include <cstddef>

class Group;
class Shape3D;

#include "Visitor.h"


/// Dummy renderer to be derived
class RenderVisitor
  : public Visitor
{
public:
  static RenderVisitor* getInstance(); //< may be derived
public:
  virtual ~RenderVisitor() {}
protected:
  RenderVisitor() : Visitor(), m_iPoly(0) {}
public:
  virtual void visit(Group* g) {}
  virtual void empty() {}
  virtual int getMode() { return m_iMode; };
  virtual void setMode(int m) { m_iMode = m; };
  virtual int getPolys() { return m_iPoly; };
private:
  virtual void visit(Shape3D* s, Group* g) {}
protected:
  int m_iMode;
  bool m_bOffset;
  int m_iPoly;
};

#endif // RENDERVISITOR_H

