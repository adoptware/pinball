/***************************************************************************
                          Polygon.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <cstdlib>
#include <algorithm>

#include "Private.h"
#include "Polygon.h"
#include "Shape3D.h"

Polygon::Polygon(Shape3D* s, int size) {
	EmAssert(s != NULL, "Polygon::Polygon shape NULL in constructor");
	m_iProperties = 0;
	m_nmlSrc.x = 0;
	m_nmlSrc.y = 1;
	m_nmlSrc.z = 0;
	m_nmlTrans.x = 0;
	m_nmlTrans.y = 1;
	m_nmlTrans.z = 0;
	m_colFlatLight.r = 0;
	m_colFlatLight.g = 0;
	m_colFlatLight.b = 0;
	m_colFlatLight.a = 0;
	
	m_vIndex.reserve(size);
	m_vIndex.clear();
	p_Shape3D = s;
}

Polygon::~Polygon() {
}

void Polygon::setProperty(int p) {
	m_iProperties |= p;
}

void Polygon::unsetProperty(int p) {
	m_iProperties -= (m_iProperties & p);
}

int Polygon::getProperties() {
	return m_iProperties;
}

void Polygon::setColor(float r, float g, float b, float a) {
	EmAssert(p_Shape3D != NULL, "Polygon::setColor shape NULL");
	vector<int>::iterator iter = m_vIndex.begin();
	vector<int>::iterator end = m_vIndex.end();
	for (; iter != end; ++iter) {
		p_Shape3D->setColor((*iter), r, g, b, a);
	}
}

void Polygon::decrement(int shindex) {
	if (shindex < 0) return;
	vector<int>::iterator iter = m_vIndex.begin();
	vector<int>::iterator end = m_vIndex.end();
	for (; iter != end; ++iter) {
 		if ((*iter) > shindex) --(*iter);
	}
}

bool Polygon::connected(int shindexA, int shindexB) {
	if (this->includes(shindexA) == -1) return false;
	if (this->includes(shindexB) == -1) return false;
	if (this->includes(shindexA) == (this->includes(shindexB) + 1)) return true;
	if (this->includes(shindexB) == (this->includes(shindexA) + 1)) return true;
	if (this->includes(shindexA) == ((signed)m_vIndex.size() - 1) && this->includes(shindexB) == 0) return true;
	if (this->includes(shindexB) == ((signed)m_vIndex.size() - 1) && this->includes(shindexA) == 0) return true;
	return false;
}

int Polygon::getIndex(int polyindex) {
	if (polyindex < 0 || polyindex >= (signed)m_vIndex.size()) {
		return -1;
	}
	return m_vIndex[polyindex];
}

int  Polygon::getIndexSize() {
	return m_vIndex.size();
}

Color * Polygon::getColor(int index) {
	EmAssert(p_Shape3D != NULL, "Polygon::getColor shape NULL");
	if (index < 0 || index >= (signed)m_vIndex.size()) {
		return NULL;
	}
	return p_Shape3D->getColor(m_vIndex[index]);
}

void Polygon::setColor(int index, float r, float g, float b, float a) {
	EmAssert(p_Shape3D != NULL, "Polygon::setColor shape NULL");
	if (index < 0 || index >= (signed)m_vIndex.size()) {
		return;
	}
	p_Shape3D->setColor(m_vIndex[index], r, g, b, a);
}

TexCoord * Polygon::getTexCoord(int index) {
	EmAssert(p_Shape3D != NULL, "Polygon::getTexCoord shape NULL");
	if (index < 0 || index >= (signed)m_vIndex.size()) {
		return NULL;
	}
	return p_Shape3D->getTexCoord(m_vIndex[index]);
}

void Polygon::add(int index) {
	EmAssert(p_Shape3D != NULL, "Polygon::add shape NULL");
	if (index < 0 || index >= (signed)p_Shape3D->m_vVtxSrc.size()) return;
	m_vIndex.push_back(index);
	if (m_vIndex.size() == 3) this->countNormal();
}

void Polygon::countNormal() {
  Vertex3D vtxA, vtxB;
  Vertex3D vtx0, vtx1, vtx2;

	if (m_vIndex.size() < 3 || p_Shape3D == NULL) return;

  vtx0 = p_Shape3D->m_vVtxSrc[m_vIndex[0]];
  vtx1 = p_Shape3D->m_vVtxSrc[m_vIndex[1]];
  vtx2 = p_Shape3D->m_vVtxSrc[m_vIndex[2]];

	vtxA.x = vtx2.x - vtx0.x;
	vtxA.y = vtx2.y - vtx0.y;
	vtxA.z = vtx2.z - vtx0.z;
	vtxB.x = vtx1.x - vtx0.x;
	vtxB.y = vtx1.y - vtx0.y;
	vtxB.z = vtx1.z - vtx0.z;

	EMath::crossProduct(vtxA, vtxB, m_nmlSrc);
	EMath::normalizeVector(m_nmlSrc);
}

int Polygon::includes(int index) {
	if (p_Shape3D == NULL) return -1;
	vector<int>::iterator iter = m_vIndex.begin();
	vector<int>::iterator end = m_vIndex.end();
	for (int a=0 ; iter != end; ++iter, ++a) {
		if ((*iter) == index) return a;
	}
	return -1;
}

void Polygon::moveUp(int polyindex) {
	if (polyindex <= 0 || polyindex > (signed)m_vIndex.size()) return;
	int tmp = m_vIndex[polyindex];
	m_vIndex[polyindex] = m_vIndex[polyindex-1];
	m_vIndex[polyindex-1] = tmp;
}

void Polygon::moveDown(int polyindex) {
	if (polyindex < 0 || polyindex > (signed)m_vIndex.size()-1) return;
	int tmp = m_vIndex[polyindex];
	m_vIndex[polyindex] = m_vIndex[polyindex+1];
	m_vIndex[polyindex+1] = tmp;
}
