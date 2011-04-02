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

Polygon3D::Polygon3D(Shape3D* s, int size) {
	EmAssert(s != NULL, "Polygon3D::Polygon3D shape NULL in constructor");
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

Polygon3D::~Polygon3D() {
}

void Polygon3D::copy(Polygon3D * poly) {
	if (poly == NULL) return;
	m_nmlSrc = poly->m_nmlSrc;
	m_nmlTrans = poly->m_nmlTrans;
	m_colFlatLight = poly->m_colFlatLight;
	m_vIndex.clear();
	vector<int>::iterator iter = poly->m_vIndex.begin();
	vector<int>::iterator end = poly->m_vIndex.end();
	for (; iter != end; ++iter) {
		m_vIndex.push_back(*iter);
	}
}

void Polygon3D::setProperty(int p) {
	m_iProperties |= p;
}

void Polygon3D::unsetProperty(int p) {
	m_iProperties -= (m_iProperties & p);
}

int Polygon3D::getProperties() {
	return m_iProperties;
}

void Polygon3D::setColor(float r, float g, float b, float a) {
	EmAssert(p_Shape3D != NULL, "Polygon3D::setColor shape NULL");
	vector<int>::iterator iter = m_vIndex.begin();
	vector<int>::iterator end = m_vIndex.end();
	for (; iter != end; ++iter) {
		p_Shape3D->setColor((*iter), r, g, b, a);
	}
}

void Polygon3D::decrement(int shindex) {
	if (shindex < 0) return;
	vector<int>::iterator iter = m_vIndex.begin();
	vector<int>::iterator end = m_vIndex.end();
	for (; iter != end; ++iter) {
 		if ((*iter) > shindex) --(*iter);
	}
}

bool Polygon3D::connected(int shindexA, int shindexB) {
	if (this->includes(shindexA) == -1) return false;
	if (this->includes(shindexB) == -1) return false;
	if (this->includes(shindexA) == (this->includes(shindexB) + 1)) return true;
	if (this->includes(shindexB) == (this->includes(shindexA) + 1)) return true;
	if (this->includes(shindexA) == ((signed)m_vIndex.size() - 1) && this->includes(shindexB) == 0) return true;
	if (this->includes(shindexB) == ((signed)m_vIndex.size() - 1) && this->includes(shindexA) == 0) return true;
	return false;
}

int Polygon3D::getIndex(int polyindex) {
	if (polyindex < 0 || polyindex >= (signed)m_vIndex.size()) {
		return -1;
	}
	return m_vIndex[polyindex];
}

int  Polygon3D::getIndexSize() {
	return m_vIndex.size();
}

Color * Polygon3D::getColor(int index) {
	EmAssert(p_Shape3D != NULL, "Polygon3D::getColor shape NULL");
	if (index < 0 || index >= (signed)m_vIndex.size()) {
		return NULL;
	}
	return p_Shape3D->getColor(m_vIndex[index]);
}

void Polygon3D::setColor(int index, float r, float g, float b, float a) {
	EmAssert(p_Shape3D != NULL, "Polygon3D::setColor shape NULL");
	if (index < 0 || index >= (signed)m_vIndex.size()) {
		return;
	}
	p_Shape3D->setColor(m_vIndex[index], r, g, b, a);
}

TexCoord * Polygon3D::getTexCoord(int index) {
	EmAssert(p_Shape3D != NULL, "Polygon3D::getTexCoord shape NULL");
	if (index < 0 || index >= (signed)m_vIndex.size()) {
		return NULL;
	}
	return p_Shape3D->getTexCoord(m_vIndex[index]);
}

void Polygon3D::add(int index) {
	EmAssert(p_Shape3D != NULL, "Polygon3D::add shape NULL");
	if (index < 0 || index >= (signed)p_Shape3D->m_vVtxSrc.size()) return;
	m_vIndex.push_back(index);
	if (m_vIndex.size() == 3) this->countNormal();
}

void Polygon3D::countNormal() {
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

int Polygon3D::includes(int index) {
	if (p_Shape3D == NULL) return -1;
	vector<int>::iterator iter = m_vIndex.begin();
	vector<int>::iterator end = m_vIndex.end();
	for (int a=0 ; iter != end; ++iter, ++a) {
		if ((*iter) == index) return a;
	}
	return -1;
}

void Polygon3D::moveUp(int polyindex) {
	if (polyindex <= 0 || polyindex > (signed)m_vIndex.size()) return;
	int tmp = m_vIndex[polyindex];
	m_vIndex[polyindex] = m_vIndex[polyindex-1];
	m_vIndex[polyindex-1] = tmp;
}

void Polygon3D::moveDown(int polyindex) {
	if (polyindex < 0 || polyindex > (signed)m_vIndex.size()-1) return;
	int tmp = m_vIndex[polyindex];
	m_vIndex[polyindex] = m_vIndex[polyindex+1];
	m_vIndex[polyindex+1] = tmp;
}
