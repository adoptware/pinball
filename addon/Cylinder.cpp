/***************************************************************************
                          Cylinder.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "Cylinder.h"
#include "Polygon.h"

Cylinder::Cylinder(float fSize, float fR, float fG, float fB, float fA, bool bTop) : Shape3D(8+8, 8+2) {
	Polygon* p;
	// First ring of vertices.
	{ for (float a=0; a<8; a+=1) {
		this->add(EMath::emSin(a/8)*fSize/2, (float)fSize/2 , EMath::emCos(a/8)*fSize/2,
							fR, fG, fB, fA, 0.0f, 0.0f);
	} }	
	// Second ring.
	{ for (float a=0; a<8; a+=1) {
		this->add(EMath::emSin(a/8)*fSize/2, (float)-fSize/2, EMath::emCos(a/8)*fSize/2,
							fR, fG, fB, fA, 0.0f, 0.0f);
	} }
	// Add vertices.
	// The ring of polygons.
	{ for (int a=0; a<7; a++) {
		p = new Polygon(this, 4);
		p->add(a);
		p->add(a+1);
		p->add(a+9);
		p->add(a+8);
		this->add(p);
	} }

	p = new Polygon(this, 4);
	p->add(7);
	p->add(0);
	p->add(8);
	p->add(15);
	this->add(p);
	// Top
	if (bTop) {
		p = new Polygon(this, 8);
		p->add(7);
		p->add(6);
		p->add(5);
		p->add(4);
		p->add(3);
		p->add(2);
		p->add(1);
		p->add(0);
		this->add(p);
		// Bottom
		p = new Polygon(this, 8);
		p->add(8);
		p->add(9);
		p->add(10);
		p->add(11);
		p->add(12);
		p->add(13);
		p->add(14);
		p->add(15);
		this->add(p);
	}

	this->countNormals();
}
