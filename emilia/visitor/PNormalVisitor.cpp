/***************************************************************************
                          PNVisit.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "PNormalVisitor.h"
#include "Group.h"
#include "Shape3D.h"
#include "Polygon.h"

PNormalVisitor::PNormalVisitor() {
}

PNormalVisitor::~PNormalVisitor() {
}

void PNormalVisitor::visit(Group* g) {
	// Check properties before applying transform
	if (g->m_iProperties & EM_GROUP_NO_TRANSFORM) return;
	
	//
	vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
	vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
	for ( ; shapeIter != shapeEnd; shapeIter++) {

		vector<Polygon*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
		vector<Polygon*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();

		for ( ; polyIter != polyEnd; polyIter++) {
			// Rotate the normal.
			// EMath::applyMatrix(m_mtxRot, (*polyIter)->m_nmlSrc, (*polyIter)->m_nmlTrans);
			EMath::applyMatrixRot(g->m_mtxTrans, (*polyIter)->m_nmlSrc, (*polyIter)->m_nmlTrans);
			EMath::normalizeVector((*polyIter)->m_nmlTrans); // TODO: optimze = remove normalize
		}
	}
}
