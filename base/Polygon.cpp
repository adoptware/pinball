/***************************************************************************
                          Polygon.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include <stdlib.h>

#include "Polygon.h"
#include "Shape3D.h"

Polygon::Polygon(Shape3D* s, int size = 3) {
	m_iProperties = 0;
	m_nmlSrc.x = 0;
	m_nmlSrc.y = 1;
	m_nmlSrc.z = 0;
	m_nmlTrans.x = 0;
	m_nmlTrans.y = 1;
	m_nmlTrans.z = 0;
	m_vIndex.reserve(size);
	m_vColor.reserve(size);
	m_vTexCoord.reserve(size);
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
	vector<Color>::iterator iter = m_vColor.begin();
	vector<Color>::iterator end = m_vColor.end();
	
	for (; iter != end; iter++) {
		(*iter).r = r;
		(*iter).g = g;
		(*iter).b = b;
		(*iter).a = a;
	}
}

void Polygon::add(int index) {
	this->add(index, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}
void Polygon::add(int index, float u, float v, float r, float g, float b, float a) {
	TexCoord tex;
	Color color;
	tex.u = u;
	tex.v = v;
	color.a = a;
	color.r = r;
	color.g = g;
	color.b = b;
	m_vIndex.push_back(index);
	m_vTexCoord.push_back(tex);
	m_vColor.push_back(color);
	
	if (m_vIndex.size() == 3) this->countNormal();
}

void Polygon::countNormal() {
  Vertex3D vtxA, vtxB;
  Vertex3D vtx0, vtx1, vtx2;

	if (m_vIndex.size() < 3 || p_Shape3D == NULL) return;
	/*
  vtx0 = p_Shape3D->m_vVertexStruct[(m_vPolygonEdge[0].index)].vtxSrc;
  vtx1 = p_Shape3D->m_vVertexStruct[(m_vPolygonEdge[1].index)].vtxSrc;
  vtx2 = p_Shape3D->m_vVertexStruct[(m_vPolygonEdge[2].index)].vtxSrc;
	*/
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

bool Polygon::includes(int index) {
	if (p_Shape3D == NULL) return false;

	vector<unsigned int>::iterator iter = m_vIndex.begin();
	vector<unsigned int>::iterator end = m_vIndex.end();
	for ( ; iter != end; iter++ ) {
		if ((*iter) == index) return true;
	}
	
	return false;
}

