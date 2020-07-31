/***************************************************************************
                          CollisionVisitor.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "CollisionVisitor.h"
#include "Group.h"
#include "Behavior.h"
#include "Shape3D.h"
#include "CollisionBounds.h"
#include "Polygon.h"
#include "OctTree.h"

#if EM_DEBUG
int em_bounds = 0;
int em_shapes = 0;
int em_polygons = 0;
int em_groups = 0;

float em_bounds_m = 0;
float em_shapes_m = 0;
float em_polygons_m = 0;
float em_groups_m = 0;
#endif

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
	p_CollisionVisitor = NULL;
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

/* Call this method each render loop. */
void CollisionVisitor::empty() {
#if EM_DEBUG
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
#endif
	// empty the octtree
	//	p_OctTree->printTree();
	p_OctTree->clear();
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
#if EM_DEBUG
		++em_groups;
#endif
		// groups in with same user properties are not collided
		// allows user to remove unnecessary collision detection
		if ((*groupIter)->m_iCollisionGroup == g->m_iCollisionGroup) {
			continue;
		}
		// no need to check if neither have a behavior object
// 		if ((*groupIter)->m_vBehavior.size() == 0 && g->m_vBehavior.size() == 0) {
// 			continue;
// 		}
 		if ((*groupIter)->getBehavior() == NULL && g->getBehavior() == NULL) {
 			continue;
 		}
#if EM_DEBUG
		++em_shapes;
#endif
		Vertex3D vtxNml1 = { 0.0f, 0.0f, 0.0f };
		Vertex3D vtxNml2 = { 0.0f, 0.0f, 0.0f };
		// check collision
		if (!((*groupIter)->p_CollisionBounds->hasShape3D()) && 
				!(g->p_CollisionBounds->hasShape3D())) {
			// use fast sphere-sphere collision for bounds without polygons
			if (this->intersect((*groupIter)->p_CollisionBounds, g->p_CollisionBounds, 
													vtxNml1, vtxNml2)) {
				EMath::normalizeVector(vtxNml1);
				EMath::normalizeVector(vtxNml2);
				this->notifyBehaviors((*groupIter), g, vtxNml1, vtxNml2);
			}
		} else if (!((*groupIter)->p_CollisionBounds->hasShape3D())) {
			// use fast sphere-poly collision for bounds without polygons
			m_vPolygon1.clear();
			if (this->detectCollisionEmpty((*groupIter)->p_CollisionBounds, 
																		 g->p_CollisionBounds, vtxNml1)) {
				EMath::normalizeVector(vtxNml1);
				this->countNormal(vtxNml2, m_vPolygon1);
				this->notifyBehaviors((*groupIter), g, vtxNml1, vtxNml2);
			}
		} else if (!(g->p_CollisionBounds->hasShape3D())) {
			// use fast sphere-poly collision for bounds without polygons
			m_vPolygon1.clear();
			if (this->detectCollisionEmpty(g->p_CollisionBounds, 
																		 (*groupIter)->p_CollisionBounds, vtxNml2)) {
				EMath::normalizeVector(vtxNml2);
				this->countNormal(vtxNml1, m_vPolygon1);
				this->notifyBehaviors((*groupIter), g, vtxNml1, vtxNml2);
			}
		} else {
			m_vPolygon1.clear();
			m_vPolygon2.clear();
			if (this->detectCollision((*groupIter)->p_CollisionBounds, g->p_CollisionBounds)) {
				this->countNormal(vtxNml1, m_vPolygon1);
				this->countNormal(vtxNml2, m_vPolygon2);
				this->notifyBehaviors((*groupIter), g, vtxNml1, vtxNml2);
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

void CollisionVisitor::notifyBehaviors(Group * g1, Group * g2, 
																			 const Vertex3D & nml1, const Vertex3D & nml2) {
	// call all onCollision methods for behaviors in both groups
// 	vector<Behavior*>::iterator behIter = g1->m_vBehavior.begin();
// 	vector<Behavior*>::iterator behEnd = g1->m_vBehavior.end();
// 	for(; behIter != behEnd; behIter++) {
// 		(*behIter)->onCollision(nml2, nml1, g2);
// 	}
// 	behIter = g2->m_vBehavior.begin();
// 	behEnd = g2->m_vBehavior.end();
// 	for(; behIter != behEnd; behIter++) {
// 		(*behIter)->onCollision(nml1, nml2, g1);
// 	} 
	if (g1->getBehavior() != NULL) {
		g1->getBehavior()->onCollision(nml2, nml1, g2);
	}
	if (g2->getBehavior() != NULL) {
		g2->getBehavior()->onCollision(nml1, nml2, g1);
	}
}


/*************************************************************
 * Intersect bounds
 ************************************************************/

/* Returns true if the bounds of cb1 and cb2 intersects. */
bool CollisionVisitor::intersect(CollisionBounds * cb1, CollisionBounds * cb2, 
																 Vertex3D & nml1, Vertex3D & nml2) {
#if EM_DEBUG
	++em_bounds;
#endif

	float dx = cb2->m_vtxTrans.x - cb1->m_vtxTrans.x;
	float dy = cb2->m_vtxTrans.y - cb1->m_vtxTrans.y;
	float dz = cb2->m_vtxTrans.z - cb1->m_vtxTrans.z;
	float radius = cb1->m_fRadius + cb2->m_fRadius;

	if ((dx*dx + dy*dy + dz*dz) <= radius*radius) {
		nml1.x = dx;
		nml1.y = dy;
		nml1.z = dz;
		nml2.x = -dx;
		nml2.y = -dy;
		nml2.z = -dz;
		return true;
	}
	return false;
}

bool CollisionVisitor::intersect(CollisionBounds * cb1, CollisionBounds * cb2) {
#if EM_DEBUG
	++em_bounds;
#endif

	float dx = cb2->m_vtxTrans.x - cb1->m_vtxTrans.x;
	float dy = cb2->m_vtxTrans.y - cb1->m_vtxTrans.y;
	float dz = cb2->m_vtxTrans.z - cb1->m_vtxTrans.z;
	float radius = cb1->m_fRadius + cb2->m_fRadius;

	if ((dx*dx + dy*dy + dz*dz) <= radius*radius) {
		return true;
	}
	return false;
}

/*****************************************************************
 * Detects collision between two CollisionBounds objects.
 * Recusively traverses the collision bounds.
 * If the boxes intersect, the shapes are detected for collision.
 * If there are no shape the function returns true. 
 ****************************************************************/

bool CollisionVisitor::detectCollision(CollisionBounds * cb1, CollisionBounds * cb2) {
	EM_COUT_D("CollisionVisitor::detectCollision()", 0);
	if (this->intersect(cb1, cb2)) {
		EM_COUT_D("CollisionVisitor::detectCollision() bounds collide", 0);
		// cb1 and cb2 is split up into more bounds collide all of them	
		if (cb1->m_vCollisionBounds.size() > 0 && cb2->m_vCollisionBounds.size() > 0) {
			bool collision = false;
			vector<CollisionBounds*>::iterator iter1 = cb1->m_vCollisionBounds.begin();
			vector<CollisionBounds*>::iterator end1 = cb1->m_vCollisionBounds.end();
			for ( ; iter1 != end1; iter1++) {
				vector<CollisionBounds*>::iterator iter2 = cb2->m_vCollisionBounds.begin();
				vector<CollisionBounds*>::iterator end2 = cb2->m_vCollisionBounds.end();
				for ( ; iter2 != end2; iter2++) {
					if (this->detectCollision((*iter1), (*iter2))) {
						collision = true;
					}
				}
			}
			return collision;
		// cb1 is split up and cb2 is a leaf
		} else if (cb1->m_vCollisionBounds.size() > 0) {
			bool collision = false;
			vector<CollisionBounds*>::iterator iter = cb1->m_vCollisionBounds.begin();
			vector<CollisionBounds*>::iterator end = cb1->m_vCollisionBounds.end();
			for ( ; iter != end; iter++) {
				if (this->detectCollision((*iter), cb2)) {
					collision = true;
				}
			}
			return collision;
		// cb1 is a leaf and cb2 is split up
		}	else if (cb2->m_vCollisionBounds.size() > 0) {
			bool collision = false;
			vector<CollisionBounds*>::iterator iter = cb2->m_vCollisionBounds.begin();
			vector<CollisionBounds*>::iterator end = cb2->m_vCollisionBounds.end();
			for ( ; iter != end; iter++) {
				if (this->detectCollision(cb1, (*iter))) {
					collision = true;
				}
			}
			return collision;
		// cb1 and cb2 are leaves
		} else {
			// poly-poly collision
			if (this->collidePolygons(cb1, cb2)) {
				return true;
			}
			EM_COUT_D("CollisionVisitor::detectCollision() false alarm", 0);
		}
	}
	return false;
}

/* Top level function. */
bool CollisionVisitor::detectCollisionEmpty(CollisionBounds * cb1, CollisionBounds * cb2,	
																						Vertex3D & nml1) {
	float distsqr = -1.0f;
	return this->detectCollisionEmpty(cb1, cb2, nml1, distsqr);
}

/* The first bounds does not have a shape, uses sphere-polygon detection.
 * The 'distsqr' gives the distance to the nearest polygon found so far,
 * (distsqr < 0) if nothing found yet. */
bool CollisionVisitor::detectCollisionEmpty(CollisionBounds * cb1, CollisionBounds * cb2,	
																						Vertex3D & nml1, float & distsqr) {
	EM_COUT_D("CollisionVisitor::detectCollisionEmpty", 0);
	if (this->intersect(cb1, cb2)) {
		// cb1 is a leaf and cb2 is split up
		if (cb2->m_vCollisionBounds.size() > 0) {
			bool collision = false;
			vector<CollisionBounds*>::iterator iter = cb2->m_vCollisionBounds.begin();
			vector<CollisionBounds*>::iterator end = cb2->m_vCollisionBounds.end();
			for ( ; iter != end; iter++) {
				if (this->detectCollisionEmpty(cb1, (*iter), nml1, distsqr)) {
					collision = true;
				}
			}
			return collision;
			// cb1 and cb2 are leaves
		} else {
			// sphere-poly collision
			bool collision = false;
			float radiussqr = cb1->m_fRadius*cb1->m_fRadius;
			//float distsqr = 9999.9f; // TODO MaxFloat
			vector<Polygon3D *>::iterator iter =  cb2->m_vPolygon.begin();
			vector<Polygon3D *>::iterator end = cb2->m_vPolygon.end();
			Vertex3D vtxDist;
			for (; iter != end; ++iter) {
				float d = this->vtxPolySqrDist(cb1->m_vtxTrans, (*iter), vtxDist);
				if (d <= radiussqr) {
					collision = true;
					// skip if polygon already in vector
					{
						bool isin = false;
						vector<Polygon3D *>::iterator vectIter = m_vPolygon1.begin();
						vector<Polygon3D *>::iterator vectEnd = m_vPolygon1.end();
						isin = false;
						for (; vectIter != vectEnd; vectIter++) {
							if ((*vectIter) == (*iter)) {
								isin = true;
								break;
							}
						}
						if (!isin) {
							m_vPolygon1.push_back(*iter);
						}
					}
					// use only the closest polygon
					if (d < distsqr || distsqr < 0.0f) {
						distsqr = d;
						nml1.x = vtxDist.x;
						nml1.y = vtxDist.y;
						nml1.z = vtxDist.z;
					}
				}
			}
			if (collision) return true;
			EM_COUT_D("CollisionVisitor::detectCollision() false alarm", 0);
		}
	}
	return false;
}


/******************************************************************
 * Check if bounds nb1 and nb2 intersect. Use intersect() to see if polygons in
 * shapes intersect. Use a hashtable to check that we don't make
 * the same intersection test more than once, a polygon can reside
 * in more than one collision bound. 
 *****************************************************************/

bool CollisionVisitor::collidePolygons(CollisionBounds * nb1, CollisionBounds * nb2) {
	bool collision = false;
	vector<Polygon3D*>::iterator iter1 = nb1->m_vPolygon.begin();
	vector<Polygon3D*>::iterator end1 = nb1->m_vPolygon.end();
	for ( ; iter1 != end1; ++iter1) {
		// skip if polygon already in vector
		vector<Polygon3D*>::iterator vectIter = m_vPolygon1.begin();
		vector<Polygon3D*>::iterator vectEnd = m_vPolygon1.end();
		bool isin = false;
		for (; vectIter != vectEnd; ++vectIter) {
			if ((*vectIter) == (*iter1)) {
				isin = true;
				break;
			}
		}
		if (isin) continue;
		vector<Polygon3D*>::iterator iter2 = nb2->m_vPolygon.begin();
		vector<Polygon3D*>::iterator end2 = nb2->m_vPolygon.end();
		for ( ; iter2 != end2; ++iter2) {
			// skip if polygon already in vector
			vectIter = m_vPolygon2.begin();
			vectEnd = m_vPolygon2.end();
			isin = false;
			for (; vectIter != vectEnd; vectIter++) {
				if ((*vectIter) == (*iter2)) {
					isin = true;
					break;
				}
			}
			if (isin) continue;
#if EM_DEBUG_COLLISION
			(*iter1)->setColor(0, 0, 1, 0.5f);
			(*iter2)->setColor(0, 0, 1, 0.5f);
#endif
			if ( CollisionVisitor::intersect((*iter1), (*iter2)) ) {
#if EM_DEBUG_COLLISION
				(*iter1)->setColor(1, 0, 0, 0.5f);
				(*iter2)->setColor(1, 0, 0, 0.5f);
#endif
				m_vPolygon1.push_back(*iter1);
				m_vPolygon2.push_back(*iter2);
				collision = true;
			}
		}
	}
	return collision;
}

/* Count a median normal for all polygons */
void CollisionVisitor::countNormal(Vertex3D & vtx, vector<Polygon3D*> vPolygon) {
	EM_COUT_D("CollisionVisitor::countNormal " << vPolygon.size() << " polygons" << endl, 0);
	vtx.x = 0;
	vtx.y = 0;
	vtx.z = 0;
	vector<Polygon3D*>::iterator iter = vPolygon.begin();
	vector<Polygon3D*>::iterator end = vPolygon.end();
	for (; iter != end; ++iter) {
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



/* TODO: */
bool CollisionVisitor::intersect2d(Polygon3D *, Polygon3D *) {
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

/********************************************************
 * Some sphere-polygon stuff
 *******************************************************/

float CollisionVisitor::vtxPolySqrDist(const Vertex3D & vtx, Polygon3D * poly, Vertex3D & vtxDist) {
	EmAssert(poly->m_vIndex.size() > 2, "CollisionVisitor::vtxPolySqrDist polygon has less than 3 vertices");
	float sqrdist = 9999.9f;
	Vertex3D vtxTmp = {0.0f, 1.0f, 0.0f};
	Shape3D * shape = poly->p_Shape3D;
	int aa = poly->m_vIndex.size()-2;
	for (int a=0; a < aa; ++a) {
		float tmp = this->vtxTriSqrDist(vtx, 
																		shape->m_vVtxTrans[poly->m_vIndex[0]], 
																		shape->m_vVtxTrans[poly->m_vIndex[a+1]], 
																		shape->m_vVtxTrans[poly->m_vIndex[a+2]],
																		vtxTmp);
		if (tmp < sqrdist) {
			sqrdist = tmp;
			vtxDist = vtxTmp;
		}
	}
	return sqrdist;
}

/*************************************************************
 * Distance from vertex to Polygon
 *
 * I have no idea of how this works, just stole it somewhere. 
 * Transform the triangle to a s-t plane. Transform so
 * that vtxTri0 is at 0,0, vtxTri1 to 1,0 and vtxTr2 to
 * 0,1 (or was it 1,0).
 *
 *           t
 * region 2  ^
 *         \ |
 *          \|
 *           |
 * region 3  |\  region 1
 *           | \
 *           |  \
 *           | 0 \
 *         ----------->s
 *           |     \
 * region 4  | 5    \ region 6
 *************************************************************/

float CollisionVisitor::vtxTriSqrDist(const Vertex3D & vtx, const Vertex3D & vtxTri0,
																			const Vertex3D & vtxTri1, const Vertex3D & vtxTri2,
																			Vertex3D & vtxOut) {
	Vertex3D vtxDiff, vtxE0, vtxE1;
	vtxDiff.x = vtxTri0.x - vtx.x;
	vtxDiff.y = vtxTri0.y - vtx.y;
	vtxDiff.z = vtxTri0.z - vtx.z;
	vtxE0.x = vtxTri1.x - vtxTri0.x;
	vtxE0.y = vtxTri1.y - vtxTri0.y;
	vtxE0.z = vtxTri1.z - vtxTri0.z;
	vtxE1.x = vtxTri2.x - vtxTri0.x;
	vtxE1.y = vtxTri2.y - vtxTri0.y;
	vtxE1.z = vtxTri2.z - vtxTri0.z;
	float a00 = EMath::vectorLengthSqr(vtxE0);
	float a01 = EMath::dotProduct(vtxE0, vtxE1);
	float a11 = EMath::vectorLengthSqr(vtxE1);
	float b0 = EMath::dotProduct(vtxDiff, vtxE0);
	float b1 = EMath::dotProduct(vtxDiff, vtxE1);
	float c = EMath::vectorLengthSqr(vtxDiff);
	float det = a00*a11 - a01*a01;
	det = EM_ABS(det);
	float s = a01*b1 - a11*b0;
	float t = a01*b0 - a00*b1;
	float sqrdist;

	if (s + t <= det) { // region 0, 3, 4, and 5
		if (s < 0.0f) {
			if (t < 0.0f) { // region 4
				if (b0 < 0.0f) {
					t = 0.0f;
					if (-b0 >= a00) {
						s = 1.0f;
						sqrdist = a00 + 2.0f*b0 + c;
					} else {
						// TODO fix division by zero thing
						s = -b0/a00;
						sqrdist = b0*s + c;
					}
				} else { // (b0 < 0.0f)
					s = 0.0f;
					if (b1 >= 0.0f) {
						t = 0.0f;
						sqrdist = c;
					} else if (-b0 >= a11) {
						t = 1.0f;
						sqrdist = a11 + 2.0f*b1 + c;
					} else {
						// TODO fix division by zero thing
						t = -b1/a11;
						sqrdist = b1*t + c;
					}
				}
			} else { // region 3
				s = 0.0f;
				if (b1 >= 0.0f) {
					t = 0.0f;
					sqrdist = c;
				} else if (-b1 >= a11) {
					t = 1.0f;
					sqrdist = a11 + 2.0*b1 + c;
				} else {
					// TODO fix division by zero thing
					t = -b1/a11;
					sqrdist = b1*t + c;
				}
			}
		} else if (t < 0.0f) { // region 5
			t = 0.0f;
			if (b0 >= 0.0f) {
				s = 0.0f;
				sqrdist = c;
			} else if (-b0 >= a00) {
				s = 1.0f;
				sqrdist = a00 + 2.0f*b0 + c;
			} else {
				// TODO fix division by zero thing
				s = -b0/a00;
				sqrdist = b0*s + c;
			}
		} else { // region 0
			float invdet = 1.0f/det;
			s *= invdet;
			t *= invdet;
			sqrdist = s*(a00*s + a01*t + 2.0f*b0) + t*(a01*s + a11*t + 2.0f*b1) + c;
		}
	} else { // region 1, 2, and 6
		float tmp0, tmp1, numer, denom;
		if (s < 0.0f) { // region 2
			tmp0 = a01 + b0;
			tmp1 = a11 + b1;
			if (tmp1 > tmp0) {
				numer = tmp1 - tmp0;
				denom = a00 - 2.0f*a01 + a11;
				if (numer >= denom) {
					s = 1.0f;
					t = 0.0f;
					sqrdist = a00 + 2.0f*b0 + c;
				} else {
					// TODO fix division with zero thing
					s = numer/denom;
					t = 1.0f - s;
					sqrdist = s*(a00*s + a01*t + 2.0f*b0) + t*(a01*s + a11*t + 2.0f*b1) + c;
				}
			} else {
				s = 0.0f;
				if (tmp1 <= 0.0f) {
					t = 1.0f;
					sqrdist = a11 + 2.0f*b1 + c;
				} else if (b1 >= 0.0f) {
					t = 0.0f;
					sqrdist = c;
				} else {
					// TODO fix division by zero thing
					t = -b1/a11;
					sqrdist = b1*t + c;
				}
			}
		} else if (t < 0.0f) { // region 6
			tmp0 = a01 + b1;
			tmp1 = a00 + b0;
			if (tmp1 > tmp0) {
				numer = tmp1 - tmp0;
				denom = a00 -  2.0f*a01 + a11;
				if (numer >= denom) {
					t = 1.0f;
					s = 0.0f;
					sqrdist = a11 + 2.0f*b1 + c;
				} else {
					// numer is greater than zero, denom is greater than denom => denom > 0.0f
					t = numer/denom;
					s = 1.0f - t;
					sqrdist = s*(a00*s + a01*t + 2.0f*b0) + t*(a01*s + a11*t + 2.0f*b1) + c;
				}
			} else {
				t = 0.0f;
				if (tmp1 <= 0.0f) {
					s = 1.0f;
					sqrdist = a00 + 2.0f*b0 + c;
				} else if (b0 >= 0.0f) {
					s = 0.0f;
					sqrdist = c;
				} else {
					// TODO
					s = -b0/a00;
					sqrdist = b0*s + c;
				}
			}
		} else { // region 1
			numer = a11 + b1 - a01 - b0;
			if (numer <= 0.0f) {
				s = 0.0f;
				t = 1.0f;
				sqrdist = a11 + 2.0f*b1 + c;
			} else {
				denom = a00 - 2.0f*a01 + a11;
				if (numer >= denom) {
					s = 1.0f;
					t = 0.0f;
					sqrdist = a00 + 2.0f*b0 + c;
				} else {
					// numer is greater than zero, denom is greater than denom => denom > 0.0f
					s = numer/denom;
					t = 1.0f - s;
					sqrdist = s*(a00*s + a01*t + 2.0f*b0) + t*(a01*s + a11*t + 2.0f*b1) + c;
				}
			}
		}
	}
	
	EMath::scaleVector(vtxE0, s);
	EMath::scaleVector(vtxE1, t);
	vtxOut.x = vtxDiff.x + vtxE0.x + vtxE1.x;
	vtxOut.y = vtxDiff.y + vtxE0.y + vtxE1.y;
	vtxOut.z = vtxDiff.z + vtxE0.z + vtxE1.z;

	return EM_ABS(sqrdist);
}

/****************************************************************
 * Polygon - polygon intersection.
 * Project intersection lines onto 1D and check if lines overlap.
 ***************************************************************/

#define FINDLINE_X(A, B, C, D, xx, ss, iter, nextIter, begin, end)                          \
	for ( ; ; ++iter, ++nextIter )	{                                                         \
		if (iter == end) return false;                                                          \
		if (nextIter == end) nextIter = begin;										                              \
		float dist1 = A * ss->m_vVtxTrans[(*iter)].x +	B * ss->m_vVtxTrans[(*iter)].y +        \
									C * ss->m_vVtxTrans[(*iter)].z + D;                                       \
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
			++iter; ++nextIter; break;                                                  		      \
		}                                                         		                          \
	}

#define FINDLINE_Y(A, B, C, D, yy, ss, iter, nextIter, begin, end)                          \
	for ( ; ; ++iter, ++nextIter ) {                                                          \
		if (iter == end) return false;                                                          \
		if (nextIter == end) nextIter = begin;										                              \
		float dist1 = A * ss->m_vVtxTrans[(*iter)].x +	B * ss->m_vVtxTrans[(*iter)].y +        \
									C * ss->m_vVtxTrans[(*iter)].z + D;                                       \
		float dist2 = A * ss->m_vVtxTrans[(*nextIter)].x +	B * ss->m_vVtxTrans[(*nextIter)].y +   \
									C * ss->m_vVtxTrans[(*nextIter)].z + D;                                   \
		if (dist1*dist2 < 0) {																						                      \
			Vertex3D vtxA = ss->m_vVtxTrans[(*iter)];				                                      \
			Vertex3D vtxB = ss->m_vVtxTrans[(*nextIter)];		                                      \
	    dist1 = EM_ABS(dist1);																                                \
	    dist2 = dist1 + EM_ABS(dist2);		                                                    \
	    float yDd = dist1/dist2;														                                  \
	    float dy = vtxB.y - vtxA.y;	                         		                              \
	    yy = vtxA.y + dy*yDd;															                                    \
			++iter; ++nextIter; break;                                                  		      \
		}                                                         		                          \
	}

#define FINDLINE_Z(A, B, C, D, zz, ss, iter, nextIter, begin, end)                          \
	for ( ; ; ++iter, ++nextIter ) {                                                          \
		if (iter == end) return false;                                                          \
		if (nextIter == end) nextIter = begin;										                              \
		float dist1 = A * ss->m_vVtxTrans[(*iter)].x +	B * ss->m_vVtxTrans[(*iter)].y +        \
									C * ss->m_vVtxTrans[(*iter)].z + D;                                       \
		float dist2 = A * ss->m_vVtxTrans[(*nextIter)].x +	B * ss->m_vVtxTrans[(*nextIter)].y +   \
									C * ss->m_vVtxTrans[(*nextIter)].z + D;                                   \
		if (dist1*dist2 < 0) {																						                      \
			Vertex3D vtxA = ss->m_vVtxTrans[(*iter)];				                                      \
			Vertex3D vtxB = ss->m_vVtxTrans[(*nextIter)];		                                      \
	    dist1 = EM_ABS(dist1);																                                \
	    dist2 = dist1 + EM_ABS(dist2);		                                                    \
	    float zDd = dist1/dist2;														                                  \
	    float dz = vtxB.z - vtxA.z;	                         		                              \
	    zz = vtxA.z + dz*zDd;															                                    \
			++iter; ++nextIter; break;                                                  		      \
		}                                                         		                          \
	}

/* Check if two polygons intersect. Only for convex polygons.
 * Checks that the polygons has at least tre vertices. Use some macros to speed things up. */
bool CollisionVisitor::intersect(Polygon3D * p1, Polygon3D * p2) {
	if (p1->m_vIndex.size() < 3 || p2->m_vIndex.size() < 3) return false;
	
#if EM_DEBUG
	++em_polygons;
#endif
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
		return CollisionVisitor::intersect2d(p1 , p2);
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
	vector<int>::iterator begin1 = p1->m_vIndex.begin();
	vector<int>::iterator iter1 = p1->m_vIndex.begin();
	vector<int>::iterator nextIter1 = begin1 + 1;
	vector<int>::iterator end1 = p1->m_vIndex.end();

	// Find lines in p2 intersecting p1. Intersection point between line and
	// plane will be projected on the chosen axis., x3 is value for projecton
	// onto acis for line 1, x4 for line 2.
	// We get a projection of polygon p1 to a axis defined by x3 and x4.
	// I.e. polygon 2 lies between points x3 and x4.
	vector<int>::iterator begin2 = p2->m_vIndex.begin();
	vector<int>::iterator iter2 = p2->m_vIndex.begin();
	vector<int>::iterator nextIter2 = begin2 + 1;
	vector<int>::iterator end2 = p2->m_vIndex.end();

	if (axis == 1) {
		// Find first line in poly1 that intersects plane A, B, C, D
		FINDLINE_X(normalB.x, normalB.y, normalB.z, D2, x1, s1, iter1, nextIter1, begin1, end1);
		// Find second line in poly1 that intersects plane A, B, C, D
		FINDLINE_X(normalB.x, normalB.y, normalB.z, D2, x2, s1, iter1, nextIter1, begin1, end1);
	
		// Find first line in poly2 that intersects plane A, B, C, D
		FINDLINE_X(normalA.x, normalA.y, normalA.z, D1, x3, s2, iter2, nextIter2, begin2, end2);
		// Find second line in poly2 that intersects plane A, B, C, D
		FINDLINE_X(normalA.x, normalA.y, normalA.z, D1, x4, s2, iter2, nextIter2, begin2, end2);
	} else if (axis == 2) {
		// Find first line in poly1 that intersects plane A, B, C, D
		FINDLINE_Y(normalB.x, normalB.y, normalB.z, D2, x1, s1, iter1, nextIter1, begin1, end1);
		// Find second line in poly1 that intersects plane A, B, C, D
		FINDLINE_Y(normalB.x, normalB.y, normalB.z, D2, x2, s1, iter1, nextIter1, begin1, end1);
	
		// Find first line in poly2 that intersects plane A, B, C, D
		FINDLINE_Y(normalA.x, normalA.y, normalA.z, D1, x3, s2, iter2, nextIter2, begin2, end2);
		// Find second line in poly2 that intersects plane A, B, C, D
		FINDLINE_Y(normalA.x, normalA.y, normalA.z, D1, x4, s2, iter2, nextIter2, begin2, end2);
	} else {
		// Find first line in poly1 that intersects plane A, B, C, D
		FINDLINE_Z(normalB.x, normalB.y, normalB.z, D2, x1, s1, iter1, nextIter1, begin1, end1);
		// Find second line in poly1 that intersects plane A, B, C, D
		FINDLINE_Z(normalB.x, normalB.y, normalB.z, D2, x2, s1, iter1, nextIter1, begin1, end1);
	
		// Find first line in poly2 that intersects plane A, B, C, D
		FINDLINE_Z(normalA.x, normalA.y, normalA.z, D1, x3, s2, iter2, nextIter2, begin2, end2);
		// Find second line in poly2 that intersects plane A, B, C, D
		FINDLINE_Z(normalA.x, normalA.y, normalA.z, D1, x4, s2, iter2, nextIter2, begin2, end2);
	}


  EM_COUT_D(x1 <<" "<< x2 <<" "<< x3 <<" "<< x4, 0);
	// Polygons intersect if line x1,x2 and x3,x4 intersect.	
	if ((EM_MAX(x1,x2) > EM_MIN(x3, x4)) && (EM_MAX(x3, x4) > EM_MIN(x1, x2))) return true;

	return false;
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
