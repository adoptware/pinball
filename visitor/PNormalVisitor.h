/***************************************************************************
                          PNVisit.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef PNORMALVISITOR_H
#define PNORMALVISITOR_H

#include "Private.h"

#include "Visitor.h"
#include "EMath.h"

class Shape3D;

/** Only for internal use.
 * Polygon Normal Visitor. When traversed trough the tree it visites all 
 * Shape3Ds and transform their normals.
 * Requires: All elements from 0 to Shape3D.iPolygons in Shape3D.aPolygons[] 
 * are pointers to Polygons. All Polygons have vtxNormal initialized.
 * Ensures: vtxTrNormal for all Polygons are normal to the transformed 
 * polygon. */
class PNormalVisitor : public Visitor {
 protected:
	PNormalVisitor();
 public:
	~PNormalVisitor();
	static PNormalVisitor * getInstance();
	void visit(Group* g);
 private:
	Matrix m_mtxRot;
	static PNormalVisitor * p_PNormalVisitor;
};
#endif // PNORMALVISITOR_H
