/***************************************************************************
                          Cone.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "Cone.h"
#include "Polygon.h"

Cone::Cone(float fSize, float fR, float fG, float fB, float fA) : Shape3D(8+1, 8+1) {
	Polygon3D* p;
	// Top vertex.
	this->add(0, fSize/2, 0);
	// Ring of vertices.
	for (float a=0; a<8; a+=1)	{
		this->add( EMath::emSin(a/8)*fSize/2, -fSize/2 , EMath::emCos(a/8)*fSize/2,
							 fR, fG, fB, fA, 0.0f, 0.0f);
	}	
	// The ring of polygons.
	for (int a=1; a<8; a+=1)	{
		p = new Polygon3D(this, 3);
		p->add(0);
		p->add(a+1);
		p->add(a);
		this->add(p);
	}
	// the last one
	p = new Polygon3D(this, 3);
	p->add(0);
	p->add(1);
	p->add(8);
	this->add(p);
	// The bottom of the cone.
	p = new Polygon3D(this, 8);
	p->add(1);
	p->add(2);
	p->add(3);
	p->add(4);
	p->add(5);
	p->add(6);
	p->add(7);
	p->add(8);
	this->add(p);
	
	this->countNormals();
}
