/***************************************************************************
                          CollisionBounds.h  -  description
                             -------------------
    begin                : Sun Mar 19 2000
    copyright            : (C) 2000 by Henrik Enqvist, GPL
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef COLLISIONBOUNDS_H
#define COLLISIONBOUNDS_H

#include <vector>

#include "EMath.h"

class Shape3D;
class Polygon;
class Group;

/** */
class CollisionBounds {
 public:
	/** CollisionBounds( radius, x, y, z ).
	 ** radius is the size of the bounds x y z is center of the bounds
	 ** relative to the center of the group. */
	CollisionBounds(float size, float x=0.0f, float y=0.0f, float z=0.0f);
	~CollisionBounds();
	/** Removes old configuration and adds all polygons to the bounds
	 * or to its children. Splits the bounds to 'level' amount of levels.*/
	void setShape3D(Shape3D * s, int level);
	/** Used by the transform visitor. */
	void transform(const Matrix & mtx);
	bool hasShape3D() { return m_bHasShape3D; };

 private:
	friend class Group;
	friend class CollisionVisitor;
	friend class BoundsRenderVisitor;
	friend class AlignVisitor;
	friend class OctTree;
	
	/** Split the polygons in the collsion bound into 8 smaller collision
	 ** bounds. The new collision will be located at x = oldX +/- 0.25*oldSize,
	 ** y = oldY +/- 0.25*oldSize and z = oldZ + +/- 0.25*oldSize and the new size
	 ** will be 0.5*oldSize.</p>*/
	void split(int level);
	int intersect(Polygon* p);
	void setParent(Group* g);
	int surround(Polygon* p);
	/** Removes empty bounding boxes from the tree of bounding boxes. */
	bool removeEmpty();
	bool addSurround(Polygon* p);
	void addIntersect(Polygon* p);
	void printTree(int level=0);
	
	Vertex3D m_vtxSrc;
	Vertex3D m_vtxTrans;
	
	bool m_bHasShape3D;
	Group* p_Parent;
	/** BoxSize is for internal use. */
	float m_fBoxSize;
	/** You should always use Radius when detection collision */
	float m_fRadius;
	vector<Polygon*> m_vPolygon;
	vector<CollisionBounds*> m_vCollisionBounds;
	//float m_fR, m_fG, m_fB;
};

#endif // COLLISIONBOUNDS_H
