/***************************************************************************
                          Shape3D.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Private.h"
#include "Shape3D.h"
#include "Polygon.h"

Shape3D::Shape3D(int v, int p) {
	m_iProperties = 0;
	p_Parent = NULL;
	m_vPolygon.reserve(p);

	m_vVtxSrc.reserve(v);
	m_vVtxTrans.reserve(v);
	m_vVtxAlign.reserve(v);

	m_vNmlSrc.reserve(v);
	m_vNmlTrans.reserve(v);
	m_vNmlAlign.reserve(v);

	m_vLight.reserve(v);
	m_vSpecular.reserve(v);

	m_vColor.reserve(v);
	m_vLitColor.reserve(v);
	m_vTexCoord.reserve(v);

	m_Texture = NULL;
}

Shape3D::~Shape3D() {
	vector<Polygon*>::iterator iter = m_vPolygon.begin();
	vector<Polygon*>::iterator end = m_vPolygon.end();
	for ( ; iter != end; iter++) {
		delete (*iter);
	}
	m_vPolygon.clear();
}

void Shape3D::setPolygonProperty(int p) {
	vector<Polygon*>::iterator iter = m_vPolygon.begin();
	vector<Polygon*>::iterator end = m_vPolygon.end();
	for ( ; iter != end; iter++) {
		(*iter)->setProperty(p);
	}
}

void Shape3D::unsetPolygonProperty(int p) {
	vector<Polygon*>::iterator iter = m_vPolygon.begin();
	vector<Polygon*>::iterator end = m_vPolygon.end();
	for ( ; iter != end; iter++) {
		(*iter)->unsetProperty(p);
	}
}

void Shape3D::setProperty(int p) {
	m_iProperties |= p;
}

void Shape3D::unsetProperty(int p) {
	m_iProperties -= (m_iProperties & p);
}

void Shape3D::setParent(Group* p) {
	p_Parent = p;
}

void Shape3D::setTexture(EmTexture* tex) {
	m_Texture = tex;
}

/*
 * Adds a vertex to this shape. The index of the vertices will be the
 * same as the order they are added.
 */
int Shape3D::add(float x, float y, float z) {
	this->add(x, y, z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	return m_vVtxSrc.size() - 1;
}

int Shape3D::add(float x, float y, float z, float r, float g, float b, float a, float u, float v) {
	EM_COUT("Shape3D::add() "<< x <<" "<< y <<" "<< z, 0);

	Vertex3D vtx;
	vtx.x = x;
	vtx.y = y;
	vtx.z = z;
	m_vVtxSrc.push_back(vtx);
	// Add dummy values to trans and align, i.e. allocate space
	m_vVtxTrans.push_back(vtx);
	m_vVtxAlign.push_back(vtx);
	// Add dummy values to normals, i.e. allocate space
	m_vNmlSrc.push_back(vtx);
	m_vNmlTrans.push_back(vtx);
	m_vNmlAlign.push_back(vtx);

	Color color = {1.0f, 1.0f, 1.0f, 1.0f};
	m_vLight.push_back(color);
	m_vSpecular.push_back(color);

	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	m_vColor.push_back(color);
	m_vLitColor.push_back(color);
	TexCoord texcoord;
	texcoord.u = u;
	texcoord.v = v;
	m_vTexCoord.push_back(texcoord);

	return m_vVtxSrc.size() - 1;
}

int Shape3D::addAt(int index, float x, float y, float z,
									 float r, float g, float b, float a, float u, float v) {
	if (index < 0 || index >= (signed)m_vVtxSrc.size()) {
		return this->add(x, y, z, r, g, b, a, u, v);
	}

	Vertex3D vtx;
	vtx.x = x;
	vtx.y = y;
	vtx.z = z;
	m_vVtxSrc.insert(m_vVtxSrc.begin()+index, vtx);
	// Add dummy values to trans and align, i.e. allocate space
	m_vVtxTrans.insert(m_vVtxTrans.begin()+index, vtx);
	m_vVtxAlign.insert(m_vVtxAlign.begin()+index, vtx);
	// Add dummy values to normals, i.e. allocate space
	m_vNmlSrc.insert(m_vNmlSrc.begin()+index, vtx);
	m_vNmlTrans.insert(m_vNmlTrans.begin()+index, vtx);
	m_vNmlAlign.insert(m_vNmlAlign.begin()+index, vtx);

	Color color = {1.0f, 1.0f, 1.0f, 1.0f};
	m_vLight.insert(m_vLight.begin()+index, color);
	m_vSpecular.insert(m_vSpecular.begin()+index, color);

	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	m_vColor.insert(m_vColor.begin()+index, color);
	m_vLitColor.insert(m_vLitColor.begin()+index, color);
	TexCoord texcoord;
	texcoord.u = u;
	texcoord.v = v;
	m_vTexCoord.insert(m_vTexCoord.begin()+index, texcoord);

	return index;
}



Vertex3D * Shape3D::getVertex3D(int index) {
	if ( index < 0 || index >= (signed)m_vVtxSrc.size() ) {
		return NULL;
	}
	return &(m_vVtxSrc[index]);
}

int Shape3D::getVertex3DSize() {
	return m_vVtxSrc.size();
}

int Shape3D::getVertex3DIndex(Vertex3D * vtx) {
	if (vtx == NULL) return -1;
	vector<Vertex3D>::iterator iter = m_vVtxSrc.begin();
	vector<Vertex3D>::iterator end = m_vVtxSrc.end();
	for (int a=0; iter != end; iter++, a++) {
		if (&(*iter) == vtx) return a;
	}
	return -1;
}

int Shape3D::getVertex3DIndex(TexCoord * tex) {
	if (tex == NULL) return -1;
	vector<TexCoord>::iterator iter = m_vTexCoord.begin();
	vector<TexCoord>::iterator end = m_vTexCoord.end();
	for (int a=0; iter != end; iter++, a++) {
		if (&(*iter) == tex) return a;
	}
	return -1;
}

Color * Shape3D::getColor(int index) {
  if (index < 0 || index >= (signed)m_vColor.size()) {
    return NULL;
  }
  return &(m_vColor[index]);
}

void Shape3D::setColor(int index, float r, float g, float b, float a) {
  if (index < 0 || index >= (signed)m_vColor.size()) {
    return;
  }
  m_vColor[index].r = EM_MAX(EM_MIN(r, 1.0f), 0.0f);
  m_vColor[index].g = EM_MAX(EM_MIN(g, 1.0f), 0.0f);
  m_vColor[index].b = EM_MAX(EM_MIN(b, 1.0f), 0.0f);
  m_vColor[index].a = EM_MAX(EM_MIN(a, 1.0f), 0.0f);
}

TexCoord * Shape3D::getTexCoord(int index) {
	if (index < 0 || index >= (signed)m_vTexCoord.size()) {
		return NULL;
	}
	return &(m_vTexCoord[index]);
}

void Shape3D::setTexCoord(int index, float u, float v) {
	if (index < 0 || index >= (signed)m_vTexCoord.size()) {
		return;
	}
	m_vTexCoord[index].u = u;
	m_vTexCoord[index].v = v;
}

bool Shape3D::removeLooseVertex3D(int vtxindex) {
	if (vtxindex < 0 || vtxindex >= (signed)m_vVtxSrc.size()) return false;
	// check that the vertex is not included in any polygon
	vector<Polygon*>::iterator polyiter = m_vPolygon.begin();
	vector<Polygon*>::iterator polyend = m_vPolygon.end();
	for (; polyiter != polyend; ++polyiter) {
		if ((*polyiter)->includes(vtxindex) >= 0) return false;
	}

 	m_vVtxSrc.erase(m_vVtxSrc.begin()+vtxindex);
 	m_vVtxTrans.erase(m_vVtxTrans.begin()+vtxindex);
 	m_vVtxAlign.erase(m_vVtxAlign.begin()+vtxindex);
 	m_vNmlSrc.erase(m_vNmlSrc.begin()+vtxindex);
 	m_vNmlTrans.erase(m_vNmlTrans.begin()+vtxindex);
 	m_vNmlAlign.erase(m_vNmlAlign.begin()+vtxindex);
	
	// decrement each index above vtxindex
	polyiter = m_vPolygon.begin();
	polyend = m_vPolygon.end();
	for (; polyiter != polyend; ++polyiter) {
		(*polyiter)->decrement(vtxindex);
	}
	return true;
}

Polygon * Shape3D::getPolygon(int index) {
	if ( index < 0 || index >= (signed)m_vPolygon.size() ) {
		return NULL;
	}
	return m_vPolygon[index];
}

int Shape3D::getPolygonSize() {
	return m_vPolygon.size();
}

int Shape3D::getPolygonIndex(Polygon * poly) {
	if (poly == NULL) return -1;
	vector<Polygon*>::iterator iter = m_vPolygon.begin();
	vector<Polygon*>::iterator end = m_vPolygon.end();
	for (int a=0; iter != end; iter++, a++) {
		if ((*iter) == poly) return a;
	}
	return -1;
}

void Shape3D::removePolygon(Polygon * poly) {
	if (poly == NULL) return;
	vector<Polygon*>::iterator iter = m_vPolygon.begin();
	vector<Polygon*>::iterator end = m_vPolygon.end();
	for (; iter != end; iter++) {
		if ((*iter) == poly) {
			m_vPolygon.erase(iter);
			return;
		}
	}
}

int Shape3D::find(float x, float y, float z, float diff) {
	vector<Vertex3D>::iterator iter = m_vVtxSrc.begin();
	vector<Vertex3D>::iterator end = m_vVtxSrc.end();
	for (int a=0; iter != end; iter++, a++) {
		if ((*iter).x < x+diff && (*iter).x > x-diff &&
				(*iter).y < y+diff && (*iter).y > y-diff &&
				(*iter).z < z+diff && (*iter).z > z-diff)
			return a;
	}
	return -1;
}

void Shape3D::add(Polygon* p) {
	if (p == NULL) return;
	m_vPolygon.push_back(p);
}

/* Sets all polygons to color c. */
void Shape3D::setColor(float r, float g, float b, float a) {
	vector<Polygon*>::iterator iter = m_vPolygon.begin();
	vector<Polygon*>::iterator end = m_vPolygon.end();
	
	for ( ; iter != end; iter++) {
		(*iter)->setColor(r,g,b,a);
	}
}

/* Counts normals for all vertices in Shape3D. */
void Shape3D::countNormals() {
	vector<Vertex3D>::iterator nmlIter = m_vNmlSrc.begin();
	vector<Vertex3D>::iterator nmlEnd = m_vNmlSrc.end();
	for (int a=0; nmlIter != nmlEnd; nmlIter++, a++) {
		(*nmlIter).x = 0;
		(*nmlIter).y = 0;
		(*nmlIter).z = 0;
		// Get the avrage of all normals in which the vertex resides.
		vector<Polygon*>::iterator polyIter = m_vPolygon.begin();
		vector<Polygon*>::iterator polyEnd = m_vPolygon.end();
		for (; polyIter != polyEnd; polyIter++) {
			if ((*polyIter)->includes(a) >= 0) {
				(*nmlIter).x += (*polyIter)->m_nmlSrc.x;
				(*nmlIter).y += (*polyIter)->m_nmlSrc.y;
				(*nmlIter).z += (*polyIter)->m_nmlSrc.z;
			}
		}
		if ( EM_ZERO((*nmlIter).x)	&&	
				EM_ZERO((*nmlIter).y) &&
				EM_ZERO((*nmlIter).z) ) {
			(*nmlIter).y = 1;
		}
	
		EMath::normalizeVector((*nmlIter));
	}
}

float Shape3D::getCollisionSize() {
	float size = 0;

	vector<Vertex3D>::iterator iter = m_vVtxSrc.begin();
	vector<Vertex3D>::iterator end = m_vVtxSrc.end();
	for ( ; iter != end; iter++) {
		if (EM_ABS((*iter).x) > size) size = EM_ABS((*iter).x);
		if (EM_ABS((*iter).y) > size) size = EM_ABS((*iter).y);
		if (EM_ABS((*iter).z) > size) size = EM_ABS((*iter).z);
	}

	return size;
}
