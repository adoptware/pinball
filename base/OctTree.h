/***************************************************************************
                          OctTree.h  -  description
                             -------------------
    begin                : Sun Mar 19 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef OCTTREE_H
#define OCTTREE_H

#include <vector>

#include "EMath.h"
#include "Node.h"

class Group;
class CollisionBounds;

/** An global octtree used for speeding up collision detection and
 * hidden surface test. */
class OctTree {
 public:
	OctTree(int level, float size, float x=0.0f, float y=0.0f, float z=0.0f);
	~OctTree();
	
 private:
	friend class CollisionVisitor;
	
	void split(int depth);
	bool surround(CollisionBounds * cb);
	bool collide(CollisionBounds * cb);
	bool insertGroup(Group * g);
	void clear();
	void printTree(int depth=0);
	
	float m_fSize;
	Vertex3D m_vtx;
	vector<OctTree*> m_vOctTree;
	vector<Group*> m_vGroup;
};

#endif // OCTTREE_H
