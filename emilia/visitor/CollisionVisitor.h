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
	public:
		/**
		 * <p>CollisionVisitor(maxNumberOfGroups)</p>
		 */
		CollisionVisitor(int);
		/*
		 * <p>Destructor.</p>
		 */
		~CollisionVisitor();
		/**
		 * <p>Must be called before every iteration in the engine tree.
		 * Empties the array of collision bounds.</p>
		 */
		void empty();
		/**
		 * <p>Performs a collision detection between the Group and Groups
		 * in the array. Adds the Group to the end of the array.</p>
		 * <p>The 'onCollision' function of the Behavior of the Group
		 * will be called with the normal to the intersected surface as
		 * parameter. The normal is normalized.</p>
		 */
		void visit(Group * g);
	private:
	  /** <p>Performs collision detection between b1 and b2. If there are Shape3Ds
	   * associated for both b1 and b2, collisin detection will be rely the function
	   * 'collide(Shap3D*, Shap3D*, Vertex3D*, Vertex3D*)'. v1 is the normal for the "wall" built up
	   * by b1. v2 is the normal for the "wall" built up by b2.</p>
	   * <p>If Shape3Ds exist v1 and v2 will be taken from 'collide(Shap3D*, Shap3D*, Vertex3D*, Vertex3D*)'
	   * otherwise it will be calculated as a line from origo of b2 to origo for b1 for v1 and
	   * the opposite for v2 ( v1 = b1->b2 ; v2 = b2->b1 ). v1 and v2 will be normalized</p>
	   */
		bool detectCollision(CollisionBounds * b1, CollisionBounds * b2, Vertex3D & v1, Vertex3D & v2);
		friend class CollisionBounds;
		/** <p>Determines intersection between two bounds.</p>
		 */
		bool collide(CollisionBounds * cb1, CollisionBounds * cb2);
		/**
	   * <p>detectCollision(b1, b2, *v1, *v2).</p>
	   * <p>Performs collision detection between b1 and b2. If there are Shape3Ds
	   * associated with b1 and b2 detection will be performed on this two
	   * Shape3Ds.</p>
	   * <p>v1 and v2 are return parameters, v1 is the normal for the surface in b1
	   * that b2 intersects, and v2 the normal for the surface on b2. Normal v1
	   * is counted as the mean value of all normals for polygons ( in b1 ) that
	   * b2 intersect. v1 and v2 will be normalized</p>
	   */
		bool collidePolygons(CollisionBounds*, CollisionBounds*);
		/**
		 * <p>Determines intersection between two polygons.</p>
		 */
		static bool intersect(Polygon*, Polygon*);
		void countNormal(Vertex3D & vtx, Polygon** paPolygons, int piPolygons);
		void addToArray(Polygon* p1, Polygon* p2);

//		static bool counterClockWiseXY(const Vertex3D & vtxA, const Vertex3D & vtxB, const Vertex3D & vtxC);
//		static bool counterClockWiseYZ(const Vertex3D & vtxA, const Vertex3D & vtxB, const Vertex3D & vtxC);
//		static bool counterClockWiseXZ(const Vertex3D & vtxA, const Vertex3D & vtxB, const Vertex3D & vtxC);
//		static bool counterClockWise(int axis, const Vertex3D & vtxA, const Vertex3D & vtxB, const Vertex3D & vtxC);
		static bool detection2d(Polygon* p1, Polygon* p2);		

		static bool findLine(int axis, float A, float B, float C, float D, float & x, Shape3D * s,
												 vector<unsigned int>::iterator & iter,	
												 vector<unsigned int>::iterator & nextIter,
												 vector<unsigned int>::iterator & end, 
												 vector<unsigned int>::iterator & begin);

		vector<Group*> m_vGroup;
		Polygon* m_aPolygonsA[MAX_POLYGONS];
		Polygon* m_aPolygonsB[MAX_POLYGONS];
		int m_iPolygonsA;
		int m_iPolygonsB;
};

#endif // COLLISIONVISITOR_H
