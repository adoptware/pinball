/***************************************************************************
                          CollisionBounds.h  -  description
                             -------------------
    begin                : Sun Mar 19 2000
    copyright            : (C) 2000 by Henrik Enqvist, GPL
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef COLLISIONBOUNDS_H
#define COLLISIONBOUNDS_H

#include "Private.h"

#include "EMath.h"
#include "Node.h"

class Shape3D;
class Polygon;
class Group;

/** */
class CollisionBounds {
	public:
		/** CollisionBounds( radius, x, y, z ).
		 * radius is the size of the bounds x y z is center of the bounds
		 * relative to the center of the group. */
		CollisionBounds(float size, float x=0.0f, float y=0.0f, float z=0.0f);
		~CollisionBounds();
		
		/** Add all polygons in Shape3D to bounds */
		void setShape3D(Shape3D * s, int level);
		/** Used by transform visitor */
		void transform(const Matrix & mtx);
		
	private:
		friend class Group;
		friend class CollisionVisitor;
		friend class BoundsRenderVisitor;
		friend class AlignVisitor;
		friend class OctTree;

		/** <p>Split the polygons in the collsion bound into 8 smaller collision
		 * bounds. The new collision will be located at x = oldX +/- 0.25*oldSize,
		 * y = oldY +/- 0.25*oldSize and z = oldZ + +/- 0.25*oldSize and the new size
		 * will be 0.5*oldSize.</p>*/
		void split(int level);
		int intersect(Polygon* p);
		void setParent(Group* g);
		int surround(Polygon* p);
		/** <p>Removes empty bounding boxes from the tree fo bounding
			boxes.</p>*/
		void removeEmpty();
		void add(Polygon* p, int, bool*);
		void printTree(int level=0);
		
		Vertex3D m_vtxSrc;
		Vertex3D m_vtxTrans;
			
		Group* p_Parent;
		float m_fBoxSize;
		float m_fRadius;
		vector<Polygon*> m_vPolygon;
		vector<CollisionBounds*> m_vCollisionBounds;
//		float m_fR, m_fG, m_fB;
};

#endif // COLLISIONBOUNDS_H
