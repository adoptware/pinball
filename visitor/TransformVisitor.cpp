/***************************************************************************
                          TVisitor.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by
    email                :
 ***************************************************************************/

#include "TransformVisitor.h"
#include "Group.h"
#include "Shape3D.h"
#include "Sound.h"
#include "Camera.h"
#include "Light.h"
#include "BillBoard.h"
#include "CollisionBounds.h"
#include "Polygon.h"

#include "EMath.h"

TransformVisitor * TransformVisitor::p_TransformVisitor = NULL;

TransformVisitor::TransformVisitor() {
	p_GroupCamera = NULL;
}

TransformVisitor::~TransformVisitor() {
}

TransformVisitor * TransformVisitor::getInstance() {
	if (p_TransformVisitor == NULL) {
		p_TransformVisitor = new TransformVisitor();
	}
	return p_TransformVisitor;
}

void TransformVisitor::setCamera(Group * g) {
	p_GroupCamera = g;
}

void TransformVisitor::empty() {
	/*
	*/
}

void TransformVisitor::visit(Group* g) {
	// Check properties before applying transform
	if (g->m_iProperties & EM_GROUP_NO_TRANSFORM) return;

	if (g->m_iProperties & EM_GROUP_TRANSFORM_ONCE) {
		g->unsetProperty(EM_GROUP_TRANSFORM_ONCE);
		g->setProperty(EM_GROUP_NO_TRANSFORM);
	}
	// Apply the transform to the Group.
	if (g->p_Parent == NULL) {
		// The group with no parent must be the engine!
		EMath::matrixMulti(g->m_mtxSrc, identityMatrix, g->m_mtxTrans);
	}	else {
		EMath::matrixMulti(g->m_mtxSrc, g->p_Parent->m_mtxTrans, g->m_mtxTrans);
	}

	// Apple transform to all shapes.
	vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
	vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
	for ( ; shapeIter != shapeEnd; shapeIter++) {

		vector<Polygon*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
		vector<Polygon*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();

		// Apply transform to all polygon normals
		for ( ; polyIter != polyEnd; polyIter++) {
			// Rotate the normal.
			// EMath::applyMatrix(m_mtxRot, (*polyIter)->m_nmlSrc, (*polyIter)->m_nmlTrans);
			EMath::applyMatrixRot(g->m_mtxTrans, (*polyIter)->m_nmlSrc, (*polyIter)->m_nmlTrans);
			EMath::normalizeVector((*polyIter)->m_nmlTrans); // TODO: optimze = remove normalize
		}

		// Apply transform to all vertices and normals
		vector<Vertex3D>::iterator srcIter = (*shapeIter)->m_vVtxSrc.begin();
		vector<Vertex3D>::iterator srcEnd = (*shapeIter)->m_vVtxSrc.end();
		vector<Vertex3D>::iterator transIter = (*shapeIter)->m_vVtxTrans.begin();
		vector<Vertex3D>::iterator transEnd = (*shapeIter)->m_vVtxTrans.end();

		vector<Vertex3D>::iterator nmlSrcIter = (*shapeIter)->m_vNmlSrc.begin();
		vector<Vertex3D>::iterator nmlSrcEnd = (*shapeIter)->m_vNmlSrc.end();
		vector<Vertex3D>::iterator nmlTransIter = (*shapeIter)->m_vNmlTrans.begin();
		vector<Vertex3D>::iterator nmlTransEnd = (*shapeIter)->m_vNmlTrans.end();

		EmAssert(((*shapeIter)->m_vVtxSrc.size() ==
							(*shapeIter)->m_vVtxTrans.size()) &&
						 ((*shapeIter)->m_vNmlSrc.size() ==
						 (*shapeIter)->m_vNmlTrans.size()), 
						 "size miss match " <<
						 (*shapeIter)->m_vVtxSrc.size() <<" "<<
						 (*shapeIter)->m_vVtxTrans.size() <<" "<<
						 (*shapeIter)->m_vNmlSrc.size() <<" "<<
						 (*shapeIter)->m_vNmlTrans.size() );

		for ( ; srcIter != srcEnd; 
					srcIter++, transIter++, nmlSrcIter++, nmlTransIter++) {
			EMath::applyMatrix(g->m_mtxTrans, (*srcIter), (*transIter));
			EMath::applyMatrixRot(g->m_mtxTrans, (*nmlSrcIter), (*nmlTransIter));
			EMath::normalizeVector(*nmlTransIter);
			// TODO: optimize - macro instead of apply-fct calls, remove normalize

			EM_COUT_D("TransformVisitor::visit() " << srcIter <<" "<<
				(*srcIter).x <<" "<< (*srcIter).y <<" "<< (*srcIter).z <<" -> "<<
				(*transIter).x <<" "<< (*transIter).y <<" "<< (*transIter).z, 0);
		}
	}
	// Apply transform to Sound.
/*	if (g->oSound != NULL) {
		Vertex3D* vtx = &(g->oSound->vtxPos);
		Vertex3D* trVtx = &(g->oSound->vtxTrPos);
		EMath::applyMatrix(g->trMatrix, vtx, trVtx);
	}*/
	// Apply transform to BillBoard.
	if (g->p_BillBoard != NULL) {
		EMath::applyMatrix(g->m_mtxTrans, g->p_BillBoard->m_vtxSrc, g->p_BillBoard->m_vtxTrans);
	}
	// Apply transform to Light.
	if (g->p_Light != NULL) {
		EMath::applyMatrix(g->m_mtxTrans, g->p_Light->m_vtxSrc, g->p_Light->m_vtxTrans);
	}
	// Apply transform to CollisionBounds
	if (g->p_CollisionBounds != NULL) {
		g->p_CollisionBounds->transform(g->m_mtxTrans);
	}
}
