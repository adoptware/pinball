/***************************************************************************
                          NestedBo.cpp  -  description
                             -------------------
    begin                : Sun Mar 19 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "CollisionBounds.h"
#include "Polygon.h"
#include "Shape3D.h"
#include "Group.h"
#include "TransformVisitor.h"
#include "CollisionVisitor.h"

bool abPolygon[256];

/*
 *
 */
CollisionBounds::CollisionBounds(float size, float x=0.0f, float y=0.0f, float z=0.0f) {
//	m_fR = (float)(rand())/RAND_MAX;
//	m_fG = (float)(rand())/RAND_MAX;
//	m_fB = (float)(rand())/RAND_MAX;

	EM_COUT("CollisionBounds() size"<< size <<" pos "<< x <<" "<< y <<" "<< z << endl, 0);
	m_vtxSrc.x = x;
	m_vtxSrc.y = y;
	m_vtxSrc.z = z;
	m_vtxTrans.x = x;
	m_vtxTrans.y = y;
	m_vtxTrans.z = z;
	m_fBoxSize = size;
	m_fRadius = size*EM_SQRT_3;
//	m_fRadius = size;
	p_Parent = NULL;
}

CollisionBounds::~CollisionBounds() {
}

/* Adds a polygon to the collision bound.
 * If the the collision bound has children the polygon is
 * added to all fo the children. If a leaf collision bound
 * fully surrounds a polygon, there is no need to add it to
 * other bounds, the pabPolygons[i] is set true. However
 * the algorithm does not remove polygons from other
 * bounds if it's later discovered to be fully surrounded
 * by a bound, could be optimized to remove polygons ...
 */
void CollisionBounds::add(Polygon* p, int i, bool* pabPolygon) {
	EmAssert(p != NULL, "CollisionBounds::add()");
	if (p == NULL) return;

	//	if (this->surround(p) > 0) {
	if (this->intersect(p)) {
		EM_COUT("CollisionBounds::add()" << endl, 0);
		if (m_vCollisionBounds.size() > 0) {
			vector<CollisionBounds*>::iterator iter = m_vCollisionBounds.begin();
			vector<CollisionBounds*>::iterator end = m_vCollisionBounds.end();
   		for ( ; iter != end; iter++) {
				(*iter)->add(p, i, pabPolygon);
			}
		} else {
			m_vPolygon.push_back(p);
/*			if (this->surround(p) == 2) {
				pabPolygons[i] = true;
				aPolygons[iPolygons++] = p;
//				p->setColor(1.0, fR, fG, fB);
			} else if (this->surround(p) == 1 && !pabPolygons[i]) {
				aPolygons[iPolygons++] = p;
//				p->setColor(1.0, fR, fG, fB);
			}*/
		}
	}
}

/* Does a fast conservative polygon-box intersection test.
 * May return intersection == true even if polygon-box does not intersect.
 */
int CollisionBounds::intersect(Polygon* poly) {
	if (poly == NULL) return 0;
	if (poly->p_Shape3D == NULL) return 0;
	if (poly->m_vIndex.size() == 0) return 0;
	// a variable is true if all vertices is outside a plane created by
	// a side of the box.
	bool bHighX = true;
	bool bLowX = true;
	bool bHighY = true;
	bool bLowY = true;
	bool bHighZ = true;
	bool bLowZ = true;
	// Check i vertices in polygon is inside box.
	vector<unsigned int>::iterator iter = poly->m_vIndex.begin();
	vector<unsigned int>::iterator end = poly->m_vIndex.end();
	for ( ; iter != end; iter++) {
		Vertex3D vtx = poly->p_Shape3D->m_vVtxSrc[(*iter)];

		if ( (m_vtxSrc.x - m_fBoxSize) <= vtx.x ) bLowX = false;
		if ( vtx.x <= (m_vtxSrc.x + m_fBoxSize) ) bHighX = false;
		if ( (m_vtxSrc.y - m_fBoxSize) <= vtx.y ) bLowY = false;
		if ( vtx.y <= (m_vtxSrc.y + m_fBoxSize) ) bHighY = false;
		if ( (m_vtxSrc.z - m_fBoxSize) <= vtx.z ) bLowZ = false;
		if ( vtx.z <= (m_vtxSrc.z + m_fBoxSize) ) bHighZ = false;
	}
	EM_COUT("CollisionBounds::intersect() " << bLowX <<" "<< bHighX <<" "<<
		bLowY <<" "<< bHighY <<" "<< bLowZ <<" "<< bHighZ, 0);
	if ( bHighX || bLowX || bHighY || bLowY || bHighZ || bLowZ ) return 0;
	return 1;
}

/* Used by Group.
 */
void CollisionBounds::setParent(Group* g) {
	p_Parent = g;
}

/* Removes old configuration and adds all polygons to the bound
 * or the its children.
 */
void CollisionBounds::setShape3D(Shape3D* s, int level) {
	if (s == NULL) return;
	
	// Remove all old collision bounds.
	// TODO: Look up the name of the destroy function for vector.
	
	// Create new collision bounds
	this->split(level);

//	bool* abPolygon = (bool*) calloc(s->-m_vPolygon.size(), sizeof(bool));
	memset(abPolygon, false, sizeof(bool)*256);

	vector<Polygon*>::iterator iter = s->m_vPolygon.begin();
	vector<Polygon*>::iterator end = s->m_vPolygon.end();
	for (int a=0; iter != end; iter++, a++) {
		this->add((*iter), a, abPolygon);
	}

//	free(abPolygons);
	this->removeEmpty();

	this->printTree();
}

/* Returns 2 if box surround all vertices, returns 1 if box surround
 * at least one vertex, else returns 0.
 */
int CollisionBounds::surround(Polygon* poly) {
	if (poly == NULL) return 0;
	if (poly->p_Shape3D == NULL) return 0;
	if (poly->m_vIndex.size() == 0) return 0;
	bool bFull = true;
	bool bSurround  = false;
	// Check i vertices in polygon is inside box.
	vector<unsigned int>::iterator iter = poly->m_vIndex.begin();
	vector<unsigned int>::iterator end = poly->m_vIndex.end();
	for ( ; iter != end; iter++) {
		Vertex3D vtx = poly->p_Shape3D->m_vVtxSrc[(*iter)];
		float dx = m_vtxSrc.x - vtx.x;
		float dy = m_vtxSrc.y - vtx.y;
		float dz = m_vtxSrc.z - vtx.z;
		EM_COUT(dx <<" "<< dy <<" "<< dz <<endl, 0);
		if ( dx >= -m_fBoxSize && dx <= m_fBoxSize &&
				dy >= -m_fBoxSize && dy <= m_fBoxSize &&
				dz >= -m_fBoxSize && dz <= m_fBoxSize) {
			bSurround = true;
		} else {
			bFull = false;
		}
	}
	if (bFull) return 2; // -2-
	else if (bSurround) return 1;
	else return this->intersect(poly);
//	return 0;
}


void CollisionBounds::printTree(int level) {
/*	EM_COUT("T: ", 0);
	int a;
	for (a=0; a<level; a++) {
		EM_COUT("  ", 0);
	}
	EM_COUT("CollisionBounds " << this->iPolygons << " polygons" << endl, 0);
	for (a=0; a<this->iCollisionBounds; a++) {
		this->aCollisionBounds[a]->printTree(level+1);
	}*/
}

void CollisionBounds::removeEmpty() {
/*	for (int a=0; a < iCollisionBounds; a++) {
		CollisionBounds* nb = aCollisionBounds[a];
		nb->removeEmpty();
		if (nb->iCollisionBounds == 0 && nb->iPolygons == 0) {
			delete(nb);
			for (int b=a; b < this->iCollisionBounds-1; b++) {
				aCollisionBounds[b] = aCollisionBounds[b+1];
			}
			aCollisionBounds[iCollisionBounds] = NULL;
			iCollisionBounds--;
			a--;
			EM_COUT("CollisionBounds::removeEmpty()" << endl, 0);
		}
	}*/
}

void CollisionBounds::split(int level) {
	int a, b, c;
	if (level < 1) return;
	EM_COUT("CollisionBounds::split() "<< level << endl, 0);

	for (a=-1; a<2; a+=2) {
		for (b=-1; b<2; b+=2) {
			for (c=-1; c<2; c+=2) {
//				printf("CollisionBounds.split() %d %f %f\n", a, (float)fSize, (float)OFFSET);
				CollisionBounds* cb = new CollisionBounds( m_fBoxSize*0.5,
					m_vtxSrc.x + a*m_fBoxSize*0.5, m_vtxSrc.y + b*m_fBoxSize*0.5,
					m_vtxSrc.z + c*m_fBoxSize*0.5);
				cb->setParent(p_Parent);
				m_vCollisionBounds.push_back(cb);
			}
		}
	}
	vector<CollisionBounds*>::iterator iter = m_vCollisionBounds.begin();
	vector<CollisionBounds*>::iterator end = m_vCollisionBounds.end();
	for ( ; iter != end; iter++) {
		(*iter)->split(level-1);
	}
}


void CollisionBounds::transform(const Matrix & mtx) {
	vector<CollisionBounds*>::iterator iter = m_vCollisionBounds.begin();
	vector<CollisionBounds*>::iterator end = m_vCollisionBounds.end();
	for ( ; iter != end; iter++) {
		(*iter)->transform(mtx);
	}
	EMATH_APPLYMATRIX(mtx, m_vtxSrc, m_vtxTrans);
	//	EMath::applyMatrix(mtx, m_vtxSrc, m_vtxTrans);
	EM_COUT("CollisionBounds()::transform "<< m_vtxTrans.x <<" "<< m_vtxTrans.y <<" "<< m_vtxTrans.z, 0);
}

