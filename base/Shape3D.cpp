/***************************************************************************
                          Shape3D.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Shape3D.h"
#include "Polygon.h"

Shape3D::Shape3D(int v = 6, int p = 2) {
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

	m_Texture = NULL;
}

Shape3D::~Shape3D() {
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

	Color color = {1, 1, 1, 1};
	m_vLight.push_back(color);
	m_vSpecular.push_back(color);

	return m_vVtxSrc.size() - 1;
}

Vertex3D * Shape3D::getVertex3D(int index) {
	if ( index < 0 || (unsigned)index >= m_vVtxSrc.size() ) {
		return NULL;
	}
	return &(m_vVtxSrc[index]);
}

Polygon * Shape3D::getPolygon(int index) {
	if ( index < 0 || (unsigned)index >= m_vPolygon.size() ) {
		return NULL;
	}
	return m_vPolygon[index];
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
void Shape3D::setColor(float a, float r, float g, float b) {
	vector<Polygon*>::iterator iter = m_vPolygon.begin();
	vector<Polygon*>::iterator end = m_vPolygon.end();
	
	for ( ; iter != end; iter++) {
		(*iter)->setColor(a,r,g,b);
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
			if ((*polyIter)->includes(a)) {
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
