/***************************************************************************
                          CollisionVisitor.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "CollisionVisitor.h"
#include "Group.h"
#include "Behavior.h"
#include "Shape3D.h"
#include "CollisionBounds.h"
#include "Polygon.h"
#include "OctTree.h"

int em_bounds = 0;
int em_shapes = 0;
int em_polygons = 0;
int em_groups = 0;

float em_bounds_m = 0;
float em_shapes_m = 0;
float em_polygons_m = 0;
float em_groups_m = 0;

CollisionVisitor * CollisionVisitor::p_CollisionVisitor = NULL;

CollisionVisitor * CollisionVisitor::getInstance() {
	if (p_CollisionVisitor == NULL) {
		p_CollisionVisitor = new CollisionVisitor(256);
	}
	return p_CollisionVisitor;
}

/* TODO: max not used */
CollisionVisitor::CollisionVisitor(int max) {	
	m_iPolygonsA = 0;
	m_iPolygonsB = 0;
	p_OctTree = new OctTree(1, 100);
}

CollisionVisitor::~CollisionVisitor() {
}

/* Adds polygon to the array of polygons that intersect. */
void CollisionVisitor::addToArray(Polygon* p1, Polygon* p2) {
	if (m_iPolygonsA < MAX_POLYGONS) {
		m_aPolygonsA[m_iPolygonsA++] = p1;
	}
	if (m_iPolygonsB < MAX_POLYGONS) {
		m_aPolygonsB[m_iPolygonsB++] = p2;
	}
}

/* Returns true if the bounds of cb1 and cb2 intersects. */
bool CollisionVisitor::collide(CollisionBounds * cb1, CollisionBounds * cb2) {
	em_bounds++;

	float dx = cb1->m_vtxTrans.x - cb2->m_vtxTrans.x;
	float dy = cb1->m_vtxTrans.y - cb2->m_vtxTrans.y;
	float dz = cb1->m_vtxTrans.z - cb2->m_vtxTrans.z;
	
	if (EMath::emSqrt(dx*dx + dy*dy + dz*dz) <= (cb1->m_fRadius + cb2->m_fRadius)) {
		EM_COUT("CollisionVisitor::collide(CollisionBounds*, CollisionBounds*) yes " << endl <<
			cb1->m_vtxTrans.x <<" "<< cb1->m_vtxTrans.y <<" "<< cb1->m_vtxTrans.z <<" "<< endl <<
			cb2->m_vtxTrans.x <<" "<< cb2->m_vtxTrans.y <<" "<< cb2->m_vtxTrans.z <<" "<< endl <<
			cb1->m_fRadius <<" "<< cb2->m_fRadius, 0);
		return true;
	}
	EM_COUT("CollisionVisitor::collide(CollisionBounds*, CollisionBounds*) no", 0);
	return false;
}

/* Check if bounds nb1 and nb2 intersect. Use intersect() to see if polygons in
 * shapes intersect. Use a hashtable to check that we don't make
 * the same intersection test more than once, a polygon can reside
 * in more than one collision bound. */
bool CollisionVisitor::collidePolygons(CollisionBounds * nb1, CollisionBounds * nb2) {
	bool bCollision = false;
	vector<Polygon*>::iterator iter1 = nb1->m_vPolygon.begin();
	vector<Polygon*>::iterator end1 = nb1->m_vPolygon.end();
	for ( ; iter1 != end1; iter1++) {
		// (*iter1)->setColor(1.0f, 1.0f, 0.0f, 0.0f); // debug thing
		// TODO: if (this->isInArray( (*iter1) )) continue;
		vector<Polygon*>::iterator iter2 = nb2->m_vPolygon.begin();
		vector<Polygon*>::iterator end2 = nb2->m_vPolygon.end();
		for ( ; iter2 != end2; iter2++) {
			// (*iter2)->setColor(1.0f, 1.0f, 0.0f, 0.0f); // debug thing
			// TODO: if (this->isInArray( (*iter2) )) continue;
			// EM_COUT("CollisionVisitor:collidePolygons() intersecting polygons " << a << "-" << b << endl, 0);
			if ( CollisionVisitor::intersect((*iter1), (*iter2)) ) {
				this->addToArray((*iter1), (*iter2));
				bCollision = true;
			}
			EM_COUT("CollisionVisitor:collidePolygons() intersected polygons" << endl, 0);				
		}
	}
	return bCollision;
}

/* Count a median normal for all polygons */
void CollisionVisitor::countNormal(Vertex3D & vtx, Polygon** paPolygons, int piPolygons) {
	EM_COUT("CollisionVisitor::countNormal " << piPolygons << " polygons" << endl, 0);
	vtx.x = 0;
	vtx.y = 0;
	vtx.z = 0;
	for (int a=0; a<piPolygons; a++) {
		vtx.x += paPolygons[a]->m_nmlTrans.x;
		vtx.y += paPolygons[a]->m_nmlTrans.y;
		vtx.z += paPolygons[a]->m_nmlTrans.z;
	}
	if ( EM_ZERO(vtx.x) &&	
			EM_ZERO(vtx.y) &&
			EM_ZERO(vtx.z) ) {
		vtx.y = 1;
	}
	EMath::normalizeVector(vtx);
}


/* Detects collision between two CollisionBounds objects.
 * If the boxes intersect, the shapes are detected for collision.
 * If there are no shape the function returns true. */
bool CollisionVisitor::detectCollision(CollisionBounds * cb1, CollisionBounds * cb2, 
																			 Vertex3D & n1, Vertex3D & n2) {
	EM_COUT("CollisionVisitor::detectCollision()", 0);
	if (this->collide(cb1, cb2)) {
		EM_COUT("CollisionVisitor::detectCollision() bounds collide", 0);
		// cb1 and cb2 is split up into more bounds collide all of them	
		if (cb1->m_vCollisionBounds.size() > 0 && cb2->m_vCollisionBounds.size() > 0) {
			bool bCollision = false;
			vector<CollisionBounds*>::iterator iter1 = cb1->m_vCollisionBounds.begin();
			vector<CollisionBounds*>::iterator end1 = cb1->m_vCollisionBounds.end();
			for ( ; iter1 != end1; iter1++) {
				vector<CollisionBounds*>::iterator iter2 = cb2->m_vCollisionBounds.begin();
				vector<CollisionBounds*>::iterator end2 = cb2->m_vCollisionBounds.end();
				for ( ; iter2 != end2; iter2++) {
					if (this->detectCollision((*iter1), (*iter2), n1, n2)) {
						bCollision = true;
					}
				}
			}
			return bCollision;
		// cb1 is split up and cb2 is a leaf
		} else if (cb1->m_vCollisionBounds.size() > 0) {
			bool bCollision = false;
			vector<CollisionBounds*>::iterator iter = cb1->m_vCollisionBounds.begin();
			vector<CollisionBounds*>::iterator end = cb1->m_vCollisionBounds.end();
			for ( ; iter != end; iter++) {
				if (this->detectCollision((*iter), cb2, n1, n2)) {
					bCollision = true;
				}
			}
			return bCollision;
		// cb1 is a leaf and cb2 is split up
		}	else if (cb2->m_vCollisionBounds.size() > 0) {
			bool bCollision = false;
			vector<CollisionBounds*>::iterator iter = cb2->m_vCollisionBounds.begin();
			vector<CollisionBounds*>::iterator end = cb2->m_vCollisionBounds.end();
			for ( ; iter != end; iter++) {
				if (this->detectCollision(cb1, (*iter), n1, n2)) {
					bCollision = true;
				}
			}
			return bCollision;
		// cb1 and cb2 are leaves
		} else {
		 	if (this->collidePolygons(cb1, cb2)) {
				this->countNormal(n1, m_aPolygonsA, m_iPolygonsA);
				this->countNormal(n2, m_aPolygonsB, m_iPolygonsB);
				EM_COUT("CollisionVisitor::detectCollision() A "<< m_iPolygonsA <<" polys "<< n1.x <<" "<< n1.y <<
								" "<< n1.z, 0);
				EM_COUT("CollisionVisitor::detectCollision() B "<< m_iPolygonsB <<" polys "<< n2.x <<" "<< n2.y <<
								" "<< n2.z, 0);
				return true;
			}
			EM_COUT("CollisionVisitor::detectCollision() false alarm", 0);
		}
	}
	return false;
}

/* TODO: */
bool CollisionVisitor::detection2d(Polygon * p1, Polygon * p2) {
//	int axis = 1;
//	float nx = ABS(p1->vtxTrNormal.x);
//	float ny = ABS(p1->vtxTrNormal.y);
//	float nz = ABS(p1->vtxTrNormal.z);

	// find which axis is most suitable to project on
//	if (nx > ny) {
//		if (nx > nz) axis = 1;
//		else if (ny > nz) axis = 2;
//		else axis = 3;
//	}
//	else if (ny > nz) axis = 2;
//	else axis = 3;

	
//	for (int a=0; a<p1->iPolygonEdges-1; a++) {
		
//	}
//	cerr << "-2D-" << endl;

	return false;
}

/* Call this method each render loop. */
void CollisionVisitor::empty() {
	em_bounds_m = em_bounds_m*0.5 + em_bounds*0.5;
	em_shapes_m = em_shapes_m*0.5 + em_shapes*0.5;
	em_polygons_m = em_polygons_m*0.5 + em_polygons*0.5;
	em_groups_m = em_groups_m*0.5 + em_groups*0.5;

	EM_COUT("CollisionVisitor::empty() polys " << em_polygons, 0);

	em_bounds = 0;
	em_shapes = 0;
	em_polygons = 0;
	em_groups = 0;

	// empty the octtree
	//	p_OctTree->printTree();
	p_OctTree->clear();
}

/* Macro to project line from v1 to v2 onto a axis.
 * Axis = 1 : x-axis. Axis = 2 : y-axis. Axis = 3 : z-axis. 
 * TODO: move the if clause outside this macro to speed things up. */
#define AXISPROJECTION(_axis, _v1, _v2, _dist1, _dist2, _ans)			\
{																																	\
	float _dx;																											\
	float _xDd;	    			                                					\
	_dist1 = EM_ABS(_dist1);																				\
	_dist2 = _dist1 + EM_ABS(_dist2);		                       			\
	_xDd = _dist1/_dist2;																						\
	/* x axis */																										\
	if (_axis == 1)	{		                      											\
		_dx = _v2.x - _v1.x;		                            					\
		_ans = _v1.x + _dx*_xDd;																			\
	}	                                              								\
	/* y axis */																										\
	else if (_axis == 2) {	                             						\
		_dx = _v2.y - _v1.y;		                   										\
		_ans = _v1.y + _dx*_xDd;                       								\
	}	                                       												\
	/* z axis */	                                        					\
	else {		                                                			\
		_dx = _v2.z - _v1.z;                                					\
		_ans = _v1.z + _dx*_xDd;                             					\
	}                                                     					\
}

/* NOT USED: replace by the a function (a bit easier to debug) */
#define FINDLINE(_poly, _A, _B, _C, _D, _iter, _end, _axis, _ans)	\
{	                                                          			\
	/* Find line in poly that intersects plane A, B, C, D */				\
	for ( ; ; _iter++)	{	                                					\
		/* Exit if not enough lines found */	          	    				\
		if (_loop == _end) return false;															\
		/* The distance from a point x, y, z is		       							\
		 * Ax + By + Cz + D / sqrt(A*A + B*B + C*C)										\
		 * The normal is normalized, meaning that											\
		 * sqrt(A*A + B*B + C*C) = 1, we							            		\
		 * can skip the division. */								             		 	\
		float _dist1 = _A*(*_iter).vtx->trVtx.x												\
			+ _B*(*_iter).vtx->trVtx.y																	\
			+ _C*(*_iter).vtx->trVtx.z + _D;														\
		int _nextIter = (_iter+1)%_poly->iPolygonEdges;										\
		float _dist2 = _A*_poly->aPolygonEdges[_nextLoop].vtx->trVtx.x		\
			+ _B*_poly->aPolygonEdges[_nextLoop].vtx->trVtx.y        				\
			+ _C*_poly->aPolygonEdges[_nextLoop].vtx->trVtx.z + _D;   			\
		/* Distances have diffrent signs */                       				\
		if (_dist1*_dist2 <0)	{																						\
			Vertex3D _vtxA = _poly->aPolygonEdges[_loop].vtx->trVtx;				\
			Vertex3D _vtxB = _poly->aPolygonEdges[_nextLoop].vtx->trVtx;		\
			AXISPROJECTION(_axis, _vtxA, _vtxB, _dist1, _dist2, _ans);			\
			_loop++;																										\
			break;                                                  		\
		}                                                         		\
	}                                                           		\
}															
	
bool CollisionVisitor::findLine(int axis, float A, float B, float C, float D, float & x, Shape3D * s, 
																vector<unsigned int>::iterator & iter,	
																vector<unsigned int>::iterator & nextIter,	
																vector<unsigned int>::iterator & begin, 
																vector<unsigned int>::iterator & end) {	
	// Find first line in poly1 that intersects plane A, B, C, D.
	// Project the intersection onto the plane: x.
	for ( ; ; iter++, nextIter++ )	{
		// Exit if not enough lines found
		if (iter == end) return false;															
		if (nextIter == end) nextIter = begin;										
		// The distance from a point x, y, z to the plane is Ax + By + Cz + D / sqrt(A*A + B*B + C*C)										
		// The normal is normalized, meaning that sqrt(A*A + B*B + C*C) = 1, we							            		
		// can skip the division.
		float dist1 = A * s->m_vVtxTrans[(*iter)].x +
									B * s->m_vVtxTrans[(*iter)].y +
									C * s->m_vVtxTrans[(*iter)].z + D;
		float dist2 = A * s->m_vVtxTrans[(*nextIter)].x +
									B * s->m_vVtxTrans[(*nextIter)].y +
									C * s->m_vVtxTrans[(*nextIter)].z + D;
		// Distances have diffrent signs meaning that the points are on different sides of the
		// plane. The line intersect the plane.
		if (dist1*dist2 <0)	{																						
			Vertex3D vtxA = s->m_vVtxTrans[(*iter)];				
			Vertex3D vtxB = s->m_vVtxTrans[(*nextIter)];		
			AXISPROJECTION(axis, vtxA, vtxB, dist1, dist2, x);			
			iter++;
			nextIter++;
			break;                                                  		
		}                                                         		
	}
	return true;
}

/* Check if two polygons intersect. This is a tough one.
 * Polygon intersection test. Only for convex polygons.
 * Checks that the polygons has at least tre vertices.
 * Use some macros to speed things up. */
bool CollisionVisitor::intersect(Polygon * p1, Polygon * p2) {
	if (p1->m_vIndex.size()<3 || p2->m_vIndex.size()<3) return false;
	
	em_polygons++;
	EM_COUT("CollisionVisitor::intersect()", 0);

	int axis;
	float D1, D2;
	float x1=0.0, x2=0.0, x3=0.0, x4=0.0;
	Vertex3D normalA, normalB, normalC;
	
	Shape3D * s1 = p1->p_Shape3D;
	Shape3D * s2 = p2->p_Shape3D;

	// Count the plane for p1
	normalA.x = p1->m_nmlTrans.x;
	normalA.y = p1->m_nmlTrans.y;
	normalA.z = p1->m_nmlTrans.z;
	D1 = - normalA.x * s1->m_vVtxTrans[p1->m_vIndex[0]].x
			- normalA.y * s1->m_vVtxTrans[p1->m_vIndex[0]].y
			- normalA.z * s1->m_vVtxTrans[p1->m_vIndex[0]].z;
	
	// Count the plane for p2
	normalB.x = p2->m_nmlTrans.x;
	normalB.y = p2->m_nmlTrans.y;
	normalB.z = p2->m_nmlTrans.z;
	D2 = - normalB.x * s2->m_vVtxTrans[p2->m_vIndex[0]].x
			- normalB.y * s2->m_vVtxTrans[p2->m_vIndex[0]].y
			- normalB.z * s2->m_vVtxTrans[p2->m_vIndex[0]].z;

	EM_COUT("CollisionVisitor::intersect() normalA " << normalA.x <<" "<< normalA.y <<" "<< 
					normalA.z <<" "<< D1, 0);
	EM_COUT("CollisionVisitor::intersect() normalB " << normalB.x <<" "<< normalB.y <<" "<< 
					normalB.z <<" "<< D2, 0);
			
	// Check if it is a 2d collision we should do
	if ( EM_ZERO(normalB.x-normalA.x) &&
			EM_ZERO(normalB.y-normalA.y) &&
			EM_ZERO(normalB.z-normalA.z) &&
			EM_ZERO(D1-D2) ) {
		return CollisionVisitor::detection2d(p1 , p2);
	}

	// Count the cross product to find which axis is most
	// suitable to project on.
	EMath::crossProduct(normalA, normalB, normalC);
	normalC.x = EM_ABS(normalC.x);
	normalC.y = EM_ABS(normalC.y);
	normalC.z = EM_ABS(normalC.z);

	if (normalC.x > normalC.y) {
		if (normalC.x > normalC.z) axis = 1;
		else if (normalC.y > normalC.z) axis = 2;
		else axis = 3;
	}
	else if (normalC.y > normalC.z) axis = 2; 
	else axis = 3;
	
	EM_COUT("CollisionVisitor::intersect() axis " << axis, 0);
	// Find lines in p1 intersecting p2. Intersection point between line and
	// plane will be projected on the chosen axis, x1 is the projecton
	// onto the axis for line 1, x2 projection for line 2.
	// We get a projection of polygon p1 to a axis defined by x1 and x2.
	// I.e. polygon 1 lies between points x1 and x2.
	vector<unsigned int>::iterator begin = p1->m_vIndex.begin();
	vector<unsigned int>::iterator iter = p1->m_vIndex.begin();
	vector<unsigned int>::iterator nextIter = begin + 1;
	vector<unsigned int>::iterator end = p1->m_vIndex.end();
	
	float A = normalA.x;
	float B = normalA.y;
	float C = normalA.z;
	float D = D1;
	
	// Find first line in poly1 that intersects plane A, B, C, D
	// Find second line in poly1 that intersects plane A, B, C, D
	if (!CollisionVisitor::findLine(axis, normalB.x, normalB.y, normalB.z, D2, x1, s1, 
																	iter, nextIter, begin, end)) {
		return false;
	}
	if (!CollisionVisitor::findLine(axis, normalB.x, normalB.y, normalB.z, D2, x2, s1, 
																	iter, nextIter, begin, end)) {
		return false;
	}
	
//	FINDLINE(p1, normalB.x, normalB.y, normalB.z, D2, iter, end, axis, x1);
//	FINDLINE(p1, normalB.x, normalB.y, normalB.z, D2, iter, end, axis, x2);
	// Find lines in p2 intersecting p1. Intersection point between line and
	// plane will be projected on the chosen axis., x3 is value for projecton
	// onto acis for line 1, x4 for line 2.
	// We get a projection of polygon p1 to a axis defined by x3 and x4.
	// I.e. polygon 2 lies between points x3 and x4.
	begin = p2->m_vIndex.begin();
	iter = p2->m_vIndex.begin();
	nextIter = begin + 1;
	end = p2->m_vIndex.end();
	
	A = normalB.x;
	B = normalB.y;
	C = normalB.z;
	D = D2;
	
	// Find first line in poly1 that intersects plane A, B, C, D
	// Find second line in poly1 that intersects plane A, B, C, D
	if (!CollisionVisitor::findLine(axis, normalA.x, normalA.y, normalA.z, D1, x3, s2,
																	iter, nextIter, begin, end)) {
		return false;
	}
	if (!CollisionVisitor::findLine(axis, normalA.x, normalA.y, normalA.z, D1, x4, s2, 
																	iter, nextIter, begin, end)) {
		return false;
	}

	// loop=0;
	// FINDLINE(p2, normalA.x, normalA.y, normalA.z, D1, loop, axis, x3);
	// FINDLINE(p2, normalA.x, normalA.y, normalA.z, D1, loop, axis, x4);
  EM_COUT(x1 <<" "<< x2 <<" "<< x3 <<" "<< x4, 0);
	// Polygons intersect if line x1,x2 and x3,x4 intersect.	
	if ((EM_MAX(x1,x2) > EM_MIN(x3, x4)) && (EM_MAX(x3, x4) > EM_MIN(x1, x2))) return true;

	return false;
}

void CollisionVisitor::traverse(Group * g, OctTree * octtree) {
	EmAssert(g != NULL, "OctTree::insestGroup() group NULL");
	EmAssert(g->p_CollisionBounds, "OctTree::insertGroup() collisionbounds NULL");

	if (!octtree->collide(g->p_CollisionBounds)) return;

	// collide this group with all groups in octtree node
	vector<Group*>::iterator groupIter = octtree->m_vGroup.begin();
	vector<Group*>::iterator groupEnd = octtree->m_vGroup.end();
	
	// collide group against all groups in octtree node
	for (; groupIter != groupEnd; groupIter++) {
		em_groups++;
		m_iPolygonsA = 0;
		m_iPolygonsB = 0;
		Vertex3D vtxNormal1;
		Vertex3D vtxNormal2;
		// groups in with same user properties are not collided
		// allows user to remove unnecessary collision detection
		if ((*groupIter)->m_iUserProperties & g->m_iUserProperties) {
			continue;
		}
		// no need to check if both neither have a behavior object
		if ((*groupIter)->m_vBehavior.size() == 0 && g->m_vBehavior.size() == 0) {
			continue;
		}
		em_shapes++;
		// check collision
		if (this->detectCollision((*groupIter)->p_CollisionBounds, g->p_CollisionBounds, 
															vtxNormal1, vtxNormal2)) {
			// call all onCollision methods for behaviors in both groups
			vector<Behavior*>::iterator behIter = (*groupIter)->m_vBehavior.begin();
			vector<Behavior*>::iterator behEnd = (*groupIter)->m_vBehavior.end();
			for(; behIter != behEnd; behIter++) {
				(*behIter)->onCollision(vtxNormal2, vtxNormal1, g);
			}
			behIter = g->m_vBehavior.begin();
			behEnd = g->m_vBehavior.end();
			for(; behIter != behEnd; behIter++) {
				(*behIter)->onCollision(vtxNormal1, vtxNormal2, (*groupIter));
			}
		}
	}
	
	// propagate the group to all the nodes
	if (octtree->m_vOctTree.size() > 0) {
		// try to add the group to a child, exit if it was added
		vector<OctTree*>::iterator iter = octtree->m_vOctTree.begin();
		vector<OctTree*>::iterator end = octtree->m_vOctTree.end();
		for ( ; iter != end; iter++) {
			this->traverse(g, (*iter));
		}
	}
}

void CollisionVisitor::visit(Group * g) {	
	if (g->p_CollisionBounds == NULL) return;
	// check collisions with nodes in octtree
	this->traverse(g, p_OctTree);
	// insert group into octtree
	p_OctTree->insertGroup(g);
}


/*
 *         (y2,y2)   (x1,z1)
 *          / \       /
 *         /   \     /
 *        /     \   /
 *       /       \ /
 *   .../.......(0,0)
 *      x
 */
 /*
bool CollisionVisitor::counterClockWise(int axis, const Vertex3D & vtxA, const Vertex3D & vtxB, const Vertex3D & vtxC) {
	if (axis == 1) {
		return counterClockWiseYZ(vtxA, vtxB, vtxC);
	}
	if (axis == 2) {
		return counterClockWiseXY(vtxA, vtxB, vtxC);
	}
	return counterClockWiseXY(vtxA, vtxB, vtxC);
}
	
bool CollisionVisitor::counterClockWiseXY(const Vertex3D & vtxA, const Vertex3D & vtxB, const Vertex3D & vtxC) {
	float x1 = vtxB.x - vtxA.x;
	float y1 = vtxB.y - vtxA.y;
	float x2 = vtxC.x - vtxA.x;
	float y2 = vtxC.y - vtxA.y;
	
	if ( EM_ZERO(y1) )	{
		return ( x1*y2 < 0 );
	}
	if ( EM_ZERO(x1) )	{
		return ( y1*x2 > 0 );
	}
	float x = x2 - y2*x1/y1;
	return (x*y1 > 0);
}

bool CollisionVisitor::counterClockWiseXZ(const Vertex3D & vtxA, const Vertex3D & vtxB, const Vertex3D & vtxC) {
	float x1 = vtxB.x - vtxA.x;
	float z1 = vtxB.z - vtxA.z;
	float x2 = vtxC.x - vtxA.x;
	float z2 = vtxC.z - vtxA.z;
	
	if ( EM_ZERO(z1) )	{
		return ( x1*z2 < 0 );
	}
	if ( EM_ZERO(x1) )	{
		return ( z1*x2 > 0 );
	}
	float x = x2 - z2*x1/z1;
	return (x*z1 > 0);
}

bool CollisionVisitor::counterClockWiseYZ(const Vertex3D & vtxA, const Vertex3D & vtxB, const Vertex3D & vtxC) {
	float y1 = vtxB.y - vtxA.y;
	float z1 = vtxB.z - vtxA.z;
	float y2 = vtxC.y - vtxA.y;
	float z2 = vtxC.z - vtxA.z;
	
	if ( EM_ZERO(z1) )	{
		return ( y1*z2 < 0 );
	}
	if ( EM_ZERO(y1) )	{
		return ( y1*y2 > 0 );
	}
	float y = y2 - z2*y1/z1;
	return (y*z1 > 0);
}
*/
