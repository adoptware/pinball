/***************************************************************************
                          CollisionVisitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef COLLISIONVISITOR_H
#define COLLISIONVISITOR_H

#include <vector>

#include "Visitor.h"
#include "EMath.h"

#define EPSILON 0.01
#define MAX_POLYGONS 1024

class Group;
class CollisionBounds;
class Shape3D;
class Polygon;
class OctTree;

/**
 * Only for internal use.
 * Performs collision detection between Groups ( between the
 * CollisionBounds associated with each group, to be specific ).
 * @see Visitor
 * @see OctTree
 */
class CollisionVisitor : public Visitor {
 protected:
	CollisionVisitor();
 public:
	~CollisionVisitor();
	static CollisionVisitor * getInstance();
	void empty();
	void visit(Group * g);

	/** Recursive intersect polygons in bounds. */
	bool detectCollision(CollisionBounds * b1, CollisionBounds * b2);
	/** Performs an intersection test between the sphere defined by the collision bounds 'cb1'
	 * and all polygons in on collision bounds 'cb2'. Returns true if collision and set normal
	 * 'nml1' to the distance from center of 'cb1' to closest point in closest polygon in 'cb2'.
	 * If no intersection can be found it returns false and 'nml1' is undefined. */
	bool detectCollisionEmpty(CollisionBounds * cb1, CollisionBounds * cb2, Vertex3D & nml1);
	bool detectCollisionEmpty(CollisionBounds * cb1, CollisionBounds * cb2, 
														Vertex3D & nml1, float & distsqr);
	/** Performs an intersection test between the sphere defined by the collision bounds 'cb1'
	 * and the sphere defined by 'cb2'. Returns true on intersection. */
	bool intersect(CollisionBounds * cb1, CollisionBounds * cb2);
	bool intersect(CollisionBounds * cb1, CollisionBounds * cb2, Vertex3D & nml1, Vertex3D & nml2);
	/** Tests intersection on all polygons referenced by collisionbounds 'cb1' and 'cb2'.
	 * Observe, only leaf-collision bounds have polygons. */
	bool collidePolygons(CollisionBounds* cb1, CollisionBounds* cb2);
	/** Poly-poly intersection test. */
	bool intersect(Polygon*, Polygon*);
	/** Not yet implemented. */
	bool intersect2d(Polygon* p1, Polygon* p2);		
	void traverse(Group * g, OctTree * octtree);
	void notifyBehaviors(Group * g1, Group * g2, const Vertex3D & nml1, const Vertex3D & nml2);
	void countNormal(Vertex3D & vtx, vector<Polygon*> vPolygon);
	float vtxPolySqrDist(const Vertex3D & vtx, Polygon * poly, Vertex3D & vtxDist);
	float vtxTriSqrDist(const Vertex3D & vtx, const Vertex3D & vtxTri0,
											const Vertex3D & vtxTri1, const Vertex3D & vtxTri2,
											Vertex3D & vtxOut);

	vector<Polygon*> m_vPolygon1;
	vector<Polygon*> m_vPolygon2;
	OctTree * p_OctTree;

	static CollisionVisitor * p_CollisionVisitor;
};

#endif // COLLISIONVISITOR_H
