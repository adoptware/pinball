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
		p_CollisionVisitor = new CollisionVisitor();
	}
	return p_CollisionVisitor;
}

CollisionVisitor::CollisionVisitor() {	
	//m_iPolygonsA = 0;
	//m_iPolygonsB = 0;
	// TODO: Some way to control the global octtree
	p_OctTree = new OctTree(1, 100);
}

CollisionVisitor::~CollisionVisitor() {
}

/* Returns true if the bounds of cb1 and cb2 intersects. */
bool CollisionVisitor::collide(CollisionBounds * cb1, CollisionBounds * cb2) {
	em_bounds++;

	float dx = cb1->m_vtxTrans.x - cb2->m_vtxTrans.x;
	float dy = cb1->m_vtxTrans.y - cb2->m_vtxTrans.y;
	float dz = cb1->m_vtxTrans.z - cb2->m_vtxTrans.z;
	
	if (EMath::emSqrt(dx*dx + dy*dy + dz*dz) <= (cb1->m_fRadius + cb2->m_fRadius)) {
		EM_COUT_D("CollisionVisitor::collide(CollisionBounds*, CollisionBounds*) yes " << endl <<
			cb1->m_vtxTrans.x <<" "<< cb1->m_vtxTrans.y <<" "<< cb1->m_vtxTrans.z <<" "<< endl <<
			cb2->m_vtxTrans.x <<" "<< cb2->m_vtxTrans.y <<" "<< cb2->m_vtxTrans.z <<" "<< endl <<
			cb1->m_fRadius <<" "<< cb2->m_fRadius, 0);
		return true;
	}
	EM_COUT_D("CollisionVisitor::collide(CollisionBounds*, CollisionBounds*) no", 0);
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
#if EM_DEBUG_COLLISION
		(*iter1)->setColor(0, 0, 1, 0.5f);
#endif
		// skip if polygon already in vector
		vector<Polygon*>::iterator vectIter = m_vPolygon1.begin();
		vector<Polygon*>::iterator vectEnd = m_vPolygon1.end();
		bool isin = false;
		for (; vectIter != vectEnd; vectIter++) {
			if ((*vectIter) == (*iter1)) {
				isin = true;
				break;
			}
		}
		if (isin) continue;
		vector<Polygon*>::iterator iter2 = nb2->m_vPolygon.begin();
		vector<Polygon*>::iterator end2 = nb2->m_vPolygon.end();
		for ( ; iter2 != end2; iter2++) {
#if EM_DEBUG_COLLISION
			(*iter2)->setColor(0, 0, 1, 0.5f); // debug thing
#endif
			// skip if polygon already in vector
			vectIter = m_vPolygon1.begin();
			vectEnd = m_vPolygon1.end();
			isin = false;
			for (; vectIter != vectEnd; vectIter++) {
				if ((*vectIter) == (*iter2)) {
					isin = true;
					break;
				}
			}
			if (isin) continue;
			// TODO: if (this->isInArray( (*iter2) )) continue;
			EM_COUT_D("CollisionVisitor:collidePolygons() intersecting polygons " << a << "-" << b << endl, 0);
			if ( CollisionVisitor::intersect((*iter1), (*iter2)) ) {
#if EM_DEBUG_COLLISION
				(*iter1)->setColor(1, 0, 0, 0.5f);
				(*iter2)->setColor(1, 0, 0, 0.5f);
#endif
				m_vPolygon1.push_back(*iter1);
				m_vPolygon2.push_back(*iter2);
				bCollision = true;
			}
			EM_COUT_D("CollisionVisitor:collidePolygons() intersected polygons" << endl, 0);				
		}
	}
	return bCollision;
}

/* Count a median normal for all polygons */
void CollisionVisitor::countNormal(Vertex3D & vtx, vector<Polygon*> vPolygon) {
	EM_COUT_D("CollisionVisitor::countNormal " << vPolygon.size() << " polygons" << endl, 0);
	vtx.x = 0;
	vtx.y = 0;
	vtx.z = 0;
	vector<Polygon*>::iterator iter = vPolygon.begin();
	vector<Polygon*>::iterator end = vPolygon.end();
	for (; iter != end; iter++) {
		vtx.x += (*iter)->m_nmlTrans.x;
		vtx.y += (*iter)->m_nmlTrans.y;
		vtx.z += (*iter)->m_nmlTrans.z;
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
	EM_COUT_D("CollisionVisitor::detectCollision()", 0);
	if (this->collide(cb1, cb2)) {
		EM_COUT_D("CollisionVisitor::detectCollision() bounds collide", 0);
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
				this->countNormal(n1, m_vPolygon1);
				this->countNormal(n2, m_vPolygon2);
				EM_COUT_D("CollisionVisitor::detectCollision() A  polys "<< n1.x <<" "<< n1.y << " "<< n1.z, 0);
				EM_COUT_D("CollisionVisitor::detectCollision() B  polys "<< n2.x <<" "<< n2.y << " "<< n2.z, 0);
				return true;
			}
			EM_COUT_D("CollisionVisitor::detectCollision() false alarm", 0);
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
	em_bounds_m = em_bounds_m*0.7 + em_bounds*0.3;
	em_shapes_m = em_shapes_m*0.7 + em_shapes*0.3;
	em_polygons_m = em_polygons_m*0.7 + em_polygons*0.3;
	em_groups_m = em_groups_m*0.7 + em_groups*0.3;

	EM_COUT("CollisionVisitor::empty() groups " << em_groups, 0);
	EM_COUT("CollisionVisitor::empty() shapes " << em_shapes, 0);
	EM_COUT("CollisionVisitor::empty() bounds " << em_bounds, 0);
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
#define AXISPROJECTION(_axis, _v1, _v2, _dist1, _dist2, _ans)		\
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

/* FINDLINE(int axis, float A, float B, float C, float D, float & x, Shape3D * s, 
 *					vector<unsigned int>::iterator & iter,	vector<unsigned int>::iterator & nextIter,	
 *   				vector<unsigned int>::iterator & begin,	vector<unsigned int>::iterator & end) */
#define FINDLINE(d_axis, d_A, d_B, d_C, d_D, d_x, d_s, d_iter, d_nextIter, d_begin, d_end)  \
	for ( ; ; d_iter++, d_nextIter++ )	{                                                     \
		if (d_iter == d_end) return false;                                                      \
		if (d_nextIter == d_end) d_nextIter = d_begin;										                      \
		float dist1 = d_A * d_s->m_vVtxTrans[(*d_iter)].x +	d_B * d_s->m_vVtxTrans[(*iter)].y + \
									d_C * d_s->m_vVtxTrans[(*d_iter)].z + d_D;                                \
		float dist2 = d_A * d_s->m_vVtxTrans[(*d_nextIter)].x +	d_B * d_s->m_vVtxTrans[(*nextIter)].y + \
									d_C * d_s->m_vVtxTrans[(*d_nextIter)].z + d_D;                            \
		if (dist1*dist2 < 0) {																						                      \
			Vertex3D vtxA = d_s->m_vVtxTrans[(*d_iter)];				                                  \
			Vertex3D vtxB = d_s->m_vVtxTrans[(*d_nextIter)];		                                  \
			AXISPROJECTION(d_axis, vtxA, vtxB, dist1, dist2, d_x);			                          \
			iter++;	nextIter++;	break;                                                  		      \
		}                                                         		                          \
	}

#define FINDLINE_X(A, B, C, D, xx, ss, iter, nextIter, begin, end)                            \
	for ( ; ; iter++, nextIter++ )	{                                                         \
		if (iter == end) return false;                                                          \
		if (nextIter == end) nextIter = begin;										                              \
		float dist1 = A * ss->m_vVtxTrans[(*iter)].x +	B * ss->m_vVtxTrans[(*iter)].y +           \
									C * ss->m_vVtxTrans[(*iter)].z + D;                                        \
		float dist2 = A * ss->m_vVtxTrans[(*nextIter)].x +	B * ss->m_vVtxTrans[(*nextIter)].y +   \
									C * ss->m_vVtxTrans[(*nextIter)].z + D;                                    \
		if (dist1*dist2 < 0) {																						                      \
			Vertex3D vtxA = ss->m_vVtxTrans[(*iter)];				                                      \
			Vertex3D vtxB = ss->m_vVtxTrans[(*nextIter)];		                                      \
	    dist1 = EM_ABS(dist1);																                                \
	    dist2 = dist1 + EM_ABS(dist2);		                                                    \
	    float xDd = dist1/dist2;														                                  \
	    float dx = vtxB.x - vtxA.x;	                         		                              \
	    xx = vtxA.x + dx*xDd;															                                    \
			iter++;	nextIter++;	break;                                                  		      \
		}                                                         		                          \
	}

#define FINDLINE_Y(A, B, C, D, yy, ss, iter, nextIter, begin, end)                            \
	for ( ; ; iter++, nextIter++ )	{                                                         \
		if (iter == end) return false;                                                          \
		if (nextIter == end) nextIter = begin;										                              \
		float dist1 = A * ss->m_vVtxTrans[(*iter)].x +	B * ss->m_vVtxTrans[(*iter)].y +           \
									C * ss->m_vVtxTrans[(*iter)].z + D;                                        \
		float dist2 = A * ss->m_vVtxTrans[(*nextIter)].x +	B * ss->m_vVtxTrans[(*nextIter)].y +   \
									C * ss->m_vVtxTrans[(*nextIter)].z + D;                                    \
		if (dist1*dist2 < 0) {																						                      \
			Vertex3D vtxA = ss->m_vVtxTrans[(*iter)];				                                      \
			Vertex3D vtxB = ss->m_vVtxTrans[(*nextIter)];		                                      \
	    dist1 = EM_ABS(dist1);																                                \
	    dist2 = dist1 + EM_ABS(dist2);		                                                    \
	    float yDd = dist1/dist2;														                                  \
	    float dy = vtxB.y - vtxA.y;	                         		                              \
	    yy = vtxA.y + dy*yDd;															                                    \
			iter++;	nextIter++;	break;                                                  		      \
		}                                                         		                          \
	}

#define FINDLINE_Z(A, B, C, D, zz, ss, iter, nextIter, begin, end)                            \
	for ( ; ; iter++, nextIter++ )	{                                                         \
		if (iter == end) return false;                                                          \
		if (nextIter == end) nextIter = begin;										                              \
		float dist1 = A * ss->m_vVtxTrans[(*iter)].x +	B * ss->m_vVtxTrans[(*iter)].y +           \
									C * ss->m_vVtxTrans[(*iter)].z + D;                                        \
		float dist2 = A * ss->m_vVtxTrans[(*nextIter)].x +	B * ss->m_vVtxTrans[(*nextIter)].y +   \
									C * ss->m_vVtxTrans[(*nextIter)].z + D;                                    \
		if (dist1*dist2 < 0) {																						                      \
			Vertex3D vtxA = ss->m_vVtxTrans[(*iter)];				                                      \
			Vertex3D vtxB = ss->m_vVtxTrans[(*nextIter)];		                                      \
	    dist1 = EM_ABS(dist1);																                                \
	    dist2 = dist1 + EM_ABS(dist2);		                                                    \
	    float zDd = dist1/dist2;														                                  \
	    float dz = vtxB.z - vtxA.z;	                         		                              \
	    zz = vtxA.z + dz*zDd;															                                    \
			iter++;	nextIter++;	break;                                                  		      \
		}                                                         		                          \
	}

/* This is the same	as the FINDLINE macro, used for debuging */
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
		if (dist1*dist2 < 0) {																						
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

/* Check if two polygons intersect. Only for convex polygons.
 * Checks that the polygons has at least tre vertices. Use some macros to speed things up. */
bool CollisionVisitor::intersect(Polygon * p1, Polygon * p2) {
	if (p1->m_vIndex.size() < 3 || p2->m_vIndex.size() < 3) return false;
	
	em_polygons++;
	EM_COUT_D("CollisionVisitor::intersect()", 0);

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

	EM_COUT_D("CollisionVisitor::intersect() normalA " << normalA.x <<" "<< normalA.y <<" "<< 
					normalA.z <<" "<< D1, 0);
	EM_COUT_D("CollisionVisitor::intersect() normalB " << normalB.x <<" "<< normalB.y <<" "<< 
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
	
	EM_COUT_D("CollisionVisitor::intersect() axis " << axis, 0);

	// Find lines in p1 intersecting p2. Intersection point between line and
	// plane will be projected on the chosen axis, x1 is the projecton
	// onto the axis for line 1, x2 projection for line 2.
	// We get a projection of polygon p1 to a axis defined by x1 and x2.
	// I.e. polygon 1 lies between points x1 and x2.
	vector<unsigned int>::iterator begin1 = p1->m_vIndex.begin();
	vector<unsigned int>::iterator iter1 = p1->m_vIndex.begin();
	vector<unsigned int>::iterator nextIter1 = begin1 + 1;
	vector<unsigned int>::iterator end1 = p1->m_vIndex.end();

	// Find lines in p2 intersecting p1. Intersection point between line and
	// plane will be projected on the chosen axis., x3 is value for projecton
	// onto acis for line 1, x4 for line 2.
	// We get a projection of polygon p1 to a axis defined by x3 and x4.
	// I.e. polygon 2 lies between points x3 and x4.
	vector<unsigned int>::iterator begin2 = p2->m_vIndex.begin();
	vector<unsigned int>::iterator iter2 = p2->m_vIndex.begin();
	vector<unsigned int>::iterator nextIter2 = begin2 + 1;
	vector<unsigned int>::iterator end2 = p2->m_vIndex.end();

	if (axis == 1) {
		// Find first line in poly1 that intersects plane A, B, C, D
		// Find second line in poly1 that intersects plane A, B, C, D
		FINDLINE_X(normalB.x, normalB.y, normalB.z, D2, x1, s1, iter1, nextIter1, begin1, end1);
		FINDLINE_X(normalB.x, normalB.y, normalB.z, D2, x2, s1, iter1, nextIter1, begin1, end1);
	
		// Find first line in poly2 that intersects plane A, B, C, D
		// Find second line in poly2 that intersects plane A, B, C, D
		FINDLINE_X(normalA.x, normalA.y, normalA.z, D1, x3, s2, iter2, nextIter2, begin2, end2);
		FINDLINE_X(normalA.x, normalA.y, normalA.z, D1, x4, s2, iter2, nextIter2, begin2, end2);
	} else if (axis == 2) {
		// Find first line in poly1 that intersects plane A, B, C, D
		// Find second line in poly1 that intersects plane A, B, C, D
		FINDLINE_Y(normalB.x, normalB.y, normalB.z, D2, x1, s1, iter1, nextIter1, begin1, end1);
		FINDLINE_Y(normalB.x, normalB.y, normalB.z, D2, x2, s1, iter1, nextIter1, begin1, end1);
	
		// Find first line in poly2 that intersects plane A, B, C, D
		// Find second line in poly2 that intersects plane A, B, C, D
		FINDLINE_Y(normalA.x, normalA.y, normalA.z, D1, x3, s2, iter2, nextIter2, begin2, end2);
		FINDLINE_Y(normalA.x, normalA.y, normalA.z, D1, x4, s2, iter2, nextIter2, begin2, end2);
	} else {
		// Find first line in poly1 that intersects plane A, B, C, D
		// Find second line in poly1 that intersects plane A, B, C, D
		FINDLINE_Z(normalB.x, normalB.y, normalB.z, D2, x1, s1, iter1, nextIter1, begin1, end1);
		FINDLINE_Z(normalB.x, normalB.y, normalB.z, D2, x2, s1, iter1, nextIter1, begin1, end1);
	
		// Find first line in poly2 that intersects plane A, B, C, D
		// Find second line in poly2 that intersects plane A, B, C, D
		FINDLINE_Z(normalA.x, normalA.y, normalA.z, D1, x3, s2, iter2, nextIter2, begin2, end2);
		FINDLINE_Z(normalA.x, normalA.y, normalA.z, D1, x4, s2, iter2, nextIter2, begin2, end2);
	}


  EM_COUT_D(x1 <<" "<< x2 <<" "<< x3 <<" "<< x4, 0);
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
		m_vPolygon1.clear();
		m_vPolygon2.clear();
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
		if (this->detectCollision((*groupIter)->p_CollisionBounds, g->p_CollisionBounds, vtxNormal1, vtxNormal2)) {
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
		vector<OctTree*>::iterator iter = octtree->m_vOctTree.begin();
		vector<OctTree*>::iterator end = octtree->m_vOctTree.end();
		for ( ; iter != end; iter++) {
			this->traverse(g, (*iter));
		}
	}
}

void CollisionVisitor::visit(Group * g) {	
	if (g->p_CollisionBounds == NULL) return;
#if EM_DEBUG_COLLISION
	if (g->getShape3DSize() > 0) g->getShape3D(0)->setColor(1,1,1,0.3f);
#endif
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
