/***************************************************************************
                          AlignVisitor.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "AlignVisitor.h"
#include "Group.h"
#include "Shape3D.h"
#include "Sound.h"
#include "Light.h"
#include "BillBoard.h"
#include "EMath.h"

AlignVisitor * AlignVisitor::p_AlignVisitor = NULL;

AlignVisitor::AlignVisitor() {
	p_GroupCamera = NULL;
	m_mtxInverse = EMath::identityMatrix;
	m_vtxFront.x = 0.0;
	m_vtxFront.y = 0.0;
	m_vtxFront.z = -1.0;
	m_vtxUp.x = 0.0;
	m_vtxUp.y = 1.0;
	m_vtxUp.z = 0.0;
}

AlignVisitor::~AlignVisitor() {
	p_AlignVisitor = NULL;
}

AlignVisitor * AlignVisitor::getInstance() {
	if (p_AlignVisitor == NULL) {
		p_AlignVisitor = new AlignVisitor();
	}
	return p_AlignVisitor;
}

/* Clean up camera matrix. */
void AlignVisitor::empty() {
	if (p_GroupCamera == NULL) return;

	EM_COUT("AlignVisitor::empty()", 0);
	// backward matrix multiplication for the camera
	if (p_GroupCamera == NULL) {
		return;
	}

	EMath::inverse(p_GroupCamera->m_mtxTrans, m_mtxInverse);
}

void AlignVisitor::setCamera(Group * g) {
	p_GroupCamera = g;
}

/* Apply camera transform to all vertices and sounds. */
void AlignVisitor::visit(Group * g) {
	EM_COUT("AlignVisitor::visit() *", 0);
	// Apply transform to vertices in oShape3D.
	vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
	vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
	for ( ; shapeIter != shapeEnd; shapeIter++) {

		vector<Vertex3D>::iterator transIter = (*shapeIter)->m_vVtxTrans.begin();
		vector<Vertex3D>::iterator transEnd = (*shapeIter)->m_vVtxTrans.end();
		vector<Vertex3D>::iterator alignIter = (*shapeIter)->m_vVtxAlign.begin();
		//vector<Vertex3D>::iterator alignEnd = (*shapeIter)->m_vVtxAlign.end();
		vector<Vertex3D>::iterator nmlTransIter = (*shapeIter)->m_vNmlTrans.begin();
		//vector<Vertex3D>::iterator nmlTransEnd = (*shapeIter)->m_vNmlTrans.end();
		vector<Vertex3D>::iterator nmlAlignIter = (*shapeIter)->m_vNmlAlign.begin();
		//vector<Vertex3D>::iterator nmlAlignEnd = (*shapeIter)->m_vNmlAlign.end();

		EmAssert(((*shapeIter)->m_vVtxTrans.size () ==
							(*shapeIter)->m_vVtxAlign.size ()) &&
						 ((*shapeIter)->m_vNmlTrans.size () ==
						 (*shapeIter)->m_vNmlAlign.size ()), "size miss match");

		for ( ; transIter != transEnd; 
					transIter++, alignIter++, nmlTransIter++, nmlAlignIter++) {
			// Translation and rotation needs to be applied in wrong order
			Vertex3D vtx;
			vtx.x = (*transIter).x + m_mtxInverse.t[0];
			vtx.y = (*transIter).y + m_mtxInverse.t[1];
			vtx.z = (*transIter).z + m_mtxInverse.t[2];
			EMath::applyMatrixRot(m_mtxInverse, vtx, (*alignIter));
			// Normals only needs rotation
			EMath::applyMatrixRot(m_mtxInverse, (*nmlTransIter), (*nmlAlignIter));
			EMath::normalizeVector(*nmlAlignIter);
			// TODO: optimize = macro instead of apply-fct calls, remove normalize

			EM_COUT("AlignVisitor::visit() " <<
							(*transIter).x <<" "<< (*transIter).y <<" "<< (*transIter).z <<" -> "<<
							(*alignIter).x <<" "<< (*alignIter).y <<" "<< (*alignIter).z, 0);
		}
	}
	// Apply transform to BillBoard.
	if (g->p_BillBoard != NULL) {
		// Translation and rotation needs to be applied in wrong order
		Vertex3D vtx;
		vtx.x = g->p_BillBoard->m_vtxTrans.x + m_mtxInverse.t[0];
		vtx.y = g->p_BillBoard->m_vtxTrans.y + m_mtxInverse.t[1];
		vtx.z = g->p_BillBoard->m_vtxTrans.z + m_mtxInverse.t[2];
		EMath::applyMatrixRot(m_mtxInverse, vtx,  g->p_BillBoard->m_vtxAlign);
		EM_COUT("AlignVisitor::visit() billboard " <<
						g->p_BillBoard->m_vtxTrans.x <<" "<< 
						g->p_BillBoard->m_vtxTrans.y <<" "<< 
						g->p_BillBoard->m_vtxTrans.z <<" -> "<< 
						g->p_BillBoard->m_vtxAlign.x <<" "<< 
						g->p_BillBoard->m_vtxAlign.y <<" "<< 
						g->p_BillBoard->m_vtxAlign.z , 0);
	}
	// Apply transform to Light.
	if (g->p_Light != NULL) {
		// Translation and rotation needs to be applied in wrong order
		Vertex3D vtx;
		vtx.x = g->p_Light->m_vtxTrans.x + m_mtxInverse.t[0];
		vtx.y = g->p_Light->m_vtxTrans.y + m_mtxInverse.t[1];
		vtx.z = g->p_Light->m_vtxTrans.z + m_mtxInverse.t[2];
		EMath::applyMatrixRot(m_mtxInverse, vtx,  g->p_Light->m_vtxAlign);
	}
	// Apply transform to oSound.
//	if (g->m_Sound != NULL) {
//		EMath::applyMatrix(mtxCamera, g->m_Sound->vtxTrans, g->m_Sound->vtxAlign);
//	}

//	Apply transform to oNestedBounds.
//	if (g->m_CollisionBounds != NULL)	{
//		this->visit(g->oNestedBounds);
//	}
}

