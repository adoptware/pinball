/***************************************************************************
                          AVisitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by
    email                :
 ***************************************************************************/

#ifndef ALIGNVISITOR_H
#define ALIGNVISITOR_H

#include "Private.h"
#include "Visitor.h"
#include "EMath.h"

class CollisionBounds;

/** Only for internal use.
 * AlignVisitor alignes transformed vertices to aligned vertices
 * by applying the camera matrix.
 * @see visitor */
class AlignVisitor : public Visitor {
 protected:
	AlignVisitor();
 public:
	~AlignVisitor();
	static AlignVisitor * getInstance();
	/** Applys the camera matrix to the Shape3D and Sound objects
	 * of the group. Checks first if the objects are NULL. */
	void visit(Group* g);
	void empty();
	/** The Engine object uses this function when setting the camera marix. */
	void setCamera(Group * g);
 private:
	Group* p_GroupCamera;
	Matrix m_mtxInverse;
	Vertex3D m_vtxFront;
	Vertex3D m_vtxUp;
	Vertex3D m_vtxTrans;
	Vertex3D m_vtxRot;
	static AlignVisitor * p_AlignVisitor;
};

#endif // ALIGNVISITOR_H
