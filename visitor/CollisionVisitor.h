/***************************************************************************
                          CollisionVisitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef COLLISIONVISITOR_H
#define COLLISIONVISITOR_H

#include "Private.h"

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
 * <p><i>Only for internal use.</i></p>
 * <p>Performs collision detection between Groups ( between the
 * CollisionBounds associated with each group to be specific ).</p>
 * <h2>Implementation:</h2>
 * <p>We use an array aGroups to store the Groups on which we perform
 * collision detection. At the beginning of each render loop this
 * array is empty, the 'empty' function ensures this. Each time
 * the Collision visitor visits a Group the Group calls the
 * 'visit' function. Collision detection is then performed between the current Group
 * and Groups in the array. The current Group is then added to the array.
 * We can now let CollisionVisit visit the next Group. This is simple and efficent,
 * collision detection between each pair of Groups is performed once, no more no less.</p>
 * @see Visitor
 */
class CollisionVisitor : public Visitor {
 protected:
	CollisionVisitor(int);
 public:
	~CollisionVisitor();
	static CollisionVisitor * getInstance();
	void empty();
	void visit(Group * g);
 private:
	friend class CollisionBounds;

	bool detectCollision(CollisionBounds * b1, CollisionBounds * b2, Vertex3D & v1, Vertex3D & v2);
	bool collide(CollisionBounds * cb1, CollisionBounds * cb2);
	bool collidePolygons(CollisionBounds*, CollisionBounds*);
	bool intersect(Polygon*, Polygon*);
	void traverse(Group * g, OctTree * octtree);
	void countNormal(Vertex3D & vtx, Polygon** paPolygons, int piPolygons);
	void addToArray(Polygon* p1, Polygon* p2);
	bool detection2d(Polygon* p1, Polygon* p2);		
	bool findLine(int axis, float A, float B, float C, float D, float & x, Shape3D * s,
								vector<unsigned int>::iterator & iter, vector<unsigned int>::iterator & nextIter,
								vector<unsigned int>::iterator & end,	vector<unsigned int>::iterator & begin);
	
	//vector<Group*> m_vGroup;
	Polygon* m_aPolygonsA[MAX_POLYGONS];
	Polygon* m_aPolygonsB[MAX_POLYGONS];
	int m_iPolygonsA;
	int m_iPolygonsB;
	OctTree * p_OctTree;

	static CollisionVisitor * p_CollisionVisitor;
};

#endif // COLLISIONVISITOR_H
