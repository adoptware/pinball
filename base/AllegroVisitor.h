/***************************************************************************
                          AllegroVisitor.h  -  description
                             -------------------
    begin                : Fri Jun 7 2002
    copyright            : (C) 2002 by Henrik Enqvist IB (GPL)
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef ALLEGROVISITOR_H
#define ALLEGROVISITOR_H

#ifndef PRIVATE_H
#error Must include Private.h before AllegroVisitor.h
#endif

#if EM_USE_ALLEGRO
#include <allegro.h>
#endif

#include "Visitor.h"

#define EM_ALLEGRO_GCOL_TEX        0 
#define EM_ALLEGRO_GCOL_TEX_TRANS  2
#define EM_ALLEGRO_CLEAN           4

class Group;
class Shape3D;

#if EM_DEBUG
extern int em_poly;
extern float em_poly_m;
#endif

/** @author Henrik Enqvist */

class AllegroVisitor : public Visitor  {
 protected:
	AllegroVisitor();
 public:
	~AllegroVisitor();
	static AllegroVisitor * getInstance();
	void visit(Group* g);
	void empty();
	int getMode() { return m_iMode; };
	void setMode(int m) { m_iMode = m; };
 private:
	void visit(Shape3D* s, Group* g);
	static AllegroVisitor * p_AllegroVisitor;
	int m_iMode;
#if EM_USE_ALLEGRO
	V3D_f alleg_vtx[256];
	V3D_f * alleg_vtx_p[256];
	V3D_f alleg_clip[256];
	V3D_f * alleg_clip_p[256];
	V3D_f alleg_tmp[256];
	V3D_f * alleg_tmp_p[256];
	int int_tmp[256];
#endif
};

#endif // ALLEGROVISITOR_H

