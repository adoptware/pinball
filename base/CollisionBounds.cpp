/***************************************************************************
                          CollisionBounds.cpp  -  description
                             -------------------
    begin                : Sun Mar 19 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "Private.h"
#include "CollisionBounds.h"
#include "Polygon.h"
#include "Shape3D.h"
#include "Group.h"

#define BOUNDS_OVERLAP 1.0f

/* */
CollisionBounds::CollisionBounds(float size, float x, float y, float z) {
	EM_COUT("CollisionBounds() size"<< size <<" pos "<< x <<" "<< y <<" "<< z << endl, 0);
	m_vtxSrc.x = x;
	m_vtxSrc.y = y;
	m_vtxSrc.z = z;
	m_vtxTrans.x = x;
	m_vtxTrans.y = y;
	m_vtxTrans.z = z;
	m_fBoxSize = size;
	m_fRadius = size*EM_SQRT_3*BOUNDS_OVERLAP;
	p_Parent = NULL;
	m_bHasShape3D = false;
}

CollisionBounds::~CollisionBounds() {
	// the shape and polygon are borrowed from the group
	// so the group will delete them
}

/* Adds a polygon to the collision bound.
 * If the the collision bound has children the polygon is
 * added to all of the children. If a leaf collision bounds
 * surrounds a polygon, there is no need to add it to
 * neighbouring bounds. This function returns true if
 * if a leaf fully surrounds a polygon. */
bool CollisionBounds::addSurround(Polygon* p) {
	EmAssert(p != NULL, "CollisionBounds::add polygon NULL");
	EM_COUT("CollisionBounds::add" << endl, 0);
	// add polygons only to leafs
	if (m_vCollisionBounds.size() > 0) {
		vector<CollisionBounds*>::iterator iter = m_vCollisionBounds.begin();
		vector<CollisionBounds*>::iterator end = m_vCollisionBounds.end();
		for (; iter != end; ++iter) {
			if ((*iter)->addSurround(p)) return true;
		}
	} else {
		// add surrounded polygon to bounds
		if (this->surround(p) == 2) {
			m_vPolygon.push_back(p);
			return true;
		}
	}
	return false;
}

void CollisionBounds::addIntersect(Polygon* p) {
	EmAssert(p != NULL, "CollisionBounds::add polygon NULL");
	EM_COUT("CollisionBounds::add" << endl, 0);
	// add polygons only to leafs
	if (m_vCollisionBounds.size() > 0) {
		vector<CollisionBounds*>::iterator iter = m_vCollisionBounds.begin();
		vector<CollisionBounds*>::iterator end = m_vCollisionBounds.end();
		for (; iter != end; ++iter) {
			(*iter)->addIntersect(p);
		}
	} else {
		// add intersection polygon to bounds
		if (this->intersect(p)) {
			m_vPolygon.push_back(p);
		}
	}
}

// TODO
/* Does a fast conservative polygon-box intersection test.
 * May return intersection == true even if polygon-box does not intersect. */
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
	// Check if vertices in polygon is inside box.
	vector<int>::iterator iter = poly->m_vIndex.begin();
	vector<int>::iterator end = poly->m_vIndex.end();
	for ( ; iter != end; iter++) {
		Vertex3D vtx = poly->p_Shape3D->m_vVtxSrc[(*iter)];

		if ( (m_vtxSrc.x - m_fBoxSize*BOUNDS_OVERLAP) < vtx.x+0.01 ) bLowX = false;
		if ( vtx.x-0.01 < (m_vtxSrc.x + m_fBoxSize*BOUNDS_OVERLAP) ) bHighX = false;
		if ( (m_vtxSrc.y - m_fBoxSize*BOUNDS_OVERLAP) < vtx.y+0.01 ) bLowY = false;
		if ( vtx.y-0.01 < (m_vtxSrc.y + m_fBoxSize*BOUNDS_OVERLAP) ) bHighY = false;
		if ( (m_vtxSrc.z - m_fBoxSize*BOUNDS_OVERLAP) < vtx.z+0.01 ) bLowZ = false;
		if ( vtx.z-0.01 < (m_vtxSrc.z + m_fBoxSize*BOUNDS_OVERLAP) ) bHighZ = false;
	}
	EM_COUT("CollisionBounds::intersect() " << bLowX <<" "<< bHighX <<" "<<
		bLowY <<" "<< bHighY <<" "<< bLowZ <<" "<< bHighZ, 0);
	if ( bHighX || bLowX || bHighY || bLowY || bHighZ || bLowZ ) return 0;
	return 1;
}

void CollisionBounds::setParent(Group* g) {
	p_Parent = g;
}

void CollisionBounds::setShape3D(Shape3D* s, int level) {
	if (s == NULL) return;
	m_bHasShape3D = true;
	// Remove all old collision bounds.
	// TODO: Look up the name of the destroy function for vector.
	
	// Create new collision bounds
	this->split(level);

	vector<Polygon*>::iterator iter = s->m_vPolygon.begin();
	vector<Polygon*>::iterator end = s->m_vPolygon.end();
	for (int a=0; iter != end; iter++, a++) {
		if (!this->addSurround(*iter)) {
			this->addIntersect(*iter);
		}
	}
	this->removeEmpty();
	//	this->printTree();
}

/* Returns 2 if box surround all vertices, returns 1 if box surround
 * at least one vertex, else returns 0. */
int CollisionBounds::surround(Polygon* poly) {
	if (poly == NULL) return 0;
	if (poly->p_Shape3D == NULL) return 0;
	if (poly->m_vIndex.size() == 0) return 0;
	bool bFull = true;
	bool bSurround  = false;
	// Check if vertices in polygon is inside box.
	vector<int>::iterator iter = poly->m_vIndex.begin();
	vector<int>::iterator end = poly->m_vIndex.end();
	for ( ; iter != end; iter++) {
		Vertex3D vtx = poly->p_Shape3D->m_vVtxSrc[(*iter)];
		float dx = m_vtxSrc.x - vtx.x;
		float dy = m_vtxSrc.y - vtx.y;
		float dz = m_vtxSrc.z - vtx.z;
		EM_COUT(dx <<" "<< dy <<" "<< dz <<endl, 0);
		if ( dx >= -m_fBoxSize*BOUNDS_OVERLAP && dx <= m_fBoxSize*BOUNDS_OVERLAP &&
				dy >= -m_fBoxSize*BOUNDS_OVERLAP && dy <= m_fBoxSize*BOUNDS_OVERLAP &&
				dz >= -m_fBoxSize*BOUNDS_OVERLAP && dz <= m_fBoxSize*BOUNDS_OVERLAP) {
			bSurround = true;
		} else {
			bFull = false;
		}
	}
	if (bFull) return 2;
	else if (bSurround) return 1;
	return 0;
}


void CollisionBounds::printTree(int level) {
	EM_COUT("T: ", 0);
	int a;
	for (a=0; a<level; ++a) {
		EM_COUT("  ", 0);
	}
	EM_COUT("CollisionBounds " << this->m_vPolygon.size() << " polygons", 0);
	vector<CollisionBounds*>::iterator iter = m_vCollisionBounds.begin();
	vector<CollisionBounds*>::iterator end = m_vCollisionBounds.end();
	for (; iter != end; ++iter) {
		(*iter)->printTree(level+1);
	}
}

bool CollisionBounds::removeEmpty() {
	vector<CollisionBounds*>::iterator iter = m_vCollisionBounds.begin();
	vector<CollisionBounds*>::iterator end = m_vCollisionBounds.end();

	for (; iter != end; ++iter) {
		if ((*iter)->removeEmpty()) {
			CollisionBounds* cb = (*iter);
			m_vCollisionBounds.erase(iter);
			delete(cb);
			// an object is erased, I think we need re-initialize
			iter = m_vCollisionBounds.begin();
			end = m_vCollisionBounds.end();
			EM_COUT("CollisionBounds::removeEmpty - removed a bound", 0);
		}
	}
	return (m_vCollisionBounds.size() == 0 && m_vPolygon.size() == 0);
}

void CollisionBounds::split(int level) {
	int a, b, c;
	if (level <= 1) return;
	EM_COUT("CollisionBounds::split "<< level << endl, 0);

	for (a=-1; a<2; a+=2) {
		for (b=-1; b<2; b+=2) {
			for (c=-1; c<2; c+=2) {
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
	for ( ; iter != end; ++iter) {
		(*iter)->transform(mtx);
	}
	//EMATH_APPLYMATRIX(mtx, m_vtxSrc, m_vtxTrans);
	EMath::applyMatrix(mtx, m_vtxSrc, m_vtxTrans);
	EM_COUT("CollisionBounds::transform "<< m_vtxTrans.x <<" "<< m_vtxTrans.y <<" "<< m_vtxTrans.z, 0);
}

