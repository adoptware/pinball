/***************************************************************************
                          OctTree.cpp  -  description
                             -------------------
    begin                : Sun Mar 19 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "OctTree.h"
#include "Group.h"
#include "CollisionBounds.h"


OctTree::OctTree(int depth, float size, float x=0.0f, float y=0.0f, float z=0.0f) {
//	m_fR = (float)(rand())/RAND_MAX;
//	m_fG = (float)(rand())/RAND_MAX;
//	m_fB = (float)(rand())/RAND_MAX;
	EmAssert(depth > 0, "Depth must be larger than 0");

	EM_COUT("OctTree() size"<< size <<" pos "<< x <<" "<< y <<" "<< z << endl, 0);
	m_vtx.x = x;
	m_vtx.y = y;
	m_vtx.z = z;
	m_fSize = size;
	if (depth > 1) {
		m_vOctTree.reserve(8);
	}

	this->split(depth-1);
}

OctTree::~OctTree() {
}

void OctTree::split(int depth) {
	if (depth < 1) return;
	EM_COUT("OctTree::split() "<< depth << endl, 0);

	// TODO: Speed things up by allocating all octtrees as an array
	// at the top octtree instead of allocating them one by one
	for (int a=-1; a<2; a+=2) {
		for (int b=-1; b<2; b+=2) {
			for (int c=-1; c<2; c+=2) {
				OctTree* ot = new OctTree( depth, m_fSize*0.5,
					m_vtx.x + a*m_fSize*0.5, m_vtx.y + b*m_fSize*0.5, m_vtx.z + c*m_fSize*0.5);
				m_vOctTree.push_back(ot);
			}
		}
	}
}

void OctTree::clear() {
	m_vGroup.clear();
	
	vector<OctTree*>::iterator iter = m_vOctTree.begin();
	vector<OctTree*>::iterator end = m_vOctTree.end();
	for (; iter != end; iter++) {
		(*iter)->clear();
	}
}

bool OctTree::insertGroup(Group * g) {
	EmAssert(g != NULL, "OctTree::insestGroup() group NULL");
	EmAssert(g->p_CollisionBounds, "OctTree::insertGroup() collisionbounds NULL");

	if (this->surround(g->p_CollisionBounds)) {
		if (m_vOctTree.size() > 0) {
			// try to add the group to a child, exit if it was added
			vector<OctTree*>::iterator iter = m_vOctTree.begin();
			vector<OctTree*>::iterator end = m_vOctTree.end();
   		for ( ; iter != end; iter++) {
				if ((*iter)->insertGroup(g)) {
					return true;
				}
			}
		}
		// else add it in this octtree
		m_vGroup.push_back(g);
		EM_COUT("OctTree() size " << m_vGroup.size(), 0);
		return true;
	}
	return false;
}

bool OctTree::surround(CollisionBounds * cb) {
	EmAssert(cb != NULL, "OctTree::surround() collisionbounds NULL");

	// observe factor , loose octtrees if over 1.
#define FACTOR 1
	if (cb->m_vtxTrans.x + cb->m_fRadius < m_vtx.x + m_fSize*FACTOR &&
			cb->m_vtxTrans.x - cb->m_fRadius > m_vtx.x - m_fSize*FACTOR &&
			cb->m_vtxTrans.y + cb->m_fRadius < m_vtx.y + m_fSize*FACTOR &&
			cb->m_vtxTrans.y - cb->m_fRadius > m_vtx.y - m_fSize*FACTOR &&
			cb->m_vtxTrans.z + cb->m_fRadius < m_vtx.z + m_fSize*FACTOR &&
			cb->m_vtxTrans.z - cb->m_fRadius > m_vtx.z - m_fSize*FACTOR) {
		return true;
	}
	return false;
}

bool OctTree::collide(CollisionBounds * cb) {
	EmAssert(cb != NULL, "OctTree::surround() collisionbounds NULL");

	// observe factor , loose octtrees if over 1.
	if (cb->m_vtxTrans.x - cb->m_fRadius < m_vtx.x + m_fSize*FACTOR &&
			cb->m_vtxTrans.x + cb->m_fRadius > m_vtx.x - m_fSize*FACTOR &&
			cb->m_vtxTrans.y - cb->m_fRadius < m_vtx.y + m_fSize*FACTOR &&
			cb->m_vtxTrans.y + cb->m_fRadius > m_vtx.y - m_fSize*FACTOR &&
			cb->m_vtxTrans.z - cb->m_fRadius < m_vtx.z + m_fSize*FACTOR &&
			cb->m_vtxTrans.z + cb->m_fRadius > m_vtx.z - m_fSize*FACTOR) {
		return true;
	}
#undef FACTOR
	
	return false;
}

void OctTree::printTree(int depth=0) {
	for (int a=0; a<depth; a++) {
		cerr << "  ";
	}
	cerr << "T: " << m_vGroup.size() << endl;

	vector<OctTree*>::iterator iter = m_vOctTree.begin();
	vector<OctTree*>::iterator end = m_vOctTree.end();
	for (; iter != end; iter++) {
		(*iter)->printTree(depth+1);
	}
}

/*
void OctTree::removeEmpty() {
	for (int a=0; a < iOctTree; a++) {
		OctTree* nb = aOctTree[a];
		nb->removeEmpty();
		if (nb->iOctTree == 0 && nb->iPolygons == 0) {
			delete(nb);
			for (int b=a; b < this->iOctTree-1; b++) {
				aOctTree[b] = aOctTree[b+1];
			}
			aOctTree[iOctTree] = NULL;
			iOctTree--;
			a--;
			EM_COUT("OctTree::removeEmpty()" << endl, 0);
		}
	}
}
*/
