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

/**
 * <p><i>Only for internal use.</i></p>
 * <p>AlignVisitor alignes transformed vertices to aligned vertices
 * by applying the camera matrix.</p>
 * @see visitor
 */

class AlignVisitor : public Visitor {
	public:
	  /** <p>Constructor.</p>. */
		AlignVisitor();
		/** <p>Destructor.</p> */
		~AlignVisitor();
		/** <p>Applys the camera matrix to the Shape3D and Sound objects
		 * of the group. Checks first if the objects are NULL.</p> */
		void visit(Group* g);
//		void visit(CollisionBounds* cb);
		/** <p>The empty function should allways be called before usage.</p> */
		void empty();
	  /** <p>The Engine object uses this function when setting the camera marix.</p> */
		void setCamera(Group * g);
	private:
		Group* p_GroupCamera;
		Matrix m_mtxInverse;
		Vertex3D m_vtxFront;
		Vertex3D m_vtxUp;
		Vertex3D m_vtxTrans;
		Vertex3D m_vtxRot;
};

#endif // ALIGNVISITOR_H
