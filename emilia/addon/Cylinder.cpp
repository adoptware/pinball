/***************************************************************************
                          Cylinder.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "Cylinder.h"
#include "Polygon.h"

Cylinder::Cylinder(float fSize, float fR, float fG, float fB, float fA, bool bTop) : Shape3D(8+8, 8+2) {
	Polygon* p;
	// First ring of vertices.
	for (float a=0; a<8; a+=1) {
		this->add(EMath::emSin(a/8)*fSize/2, (float)fSize/2 , EMath::emCos(a/8)*fSize/2);
	}	
	// Second ring.
	for (float a=0; a<8; a+=1) {
		this->add(EMath::emSin(a/8)*fSize/2, (float)-fSize/2, EMath::emCos(a/8)*fSize/2);
	}
	// Add vertices.
	// The ring of polygons.
	for (int a=0; a<7; a++) {
		p = new Polygon(this, 4);
		p->add(a,   0, 0, fA, fR, fG, fB);
		p->add(a+1, 0, 0, fA, fR, fG, fB);
		p->add(a+9, 0, 0, fA, fR, fG, fB);
		p->add(a+8, 0, 0, fA, fR, fG, fB);
		this->add(p);
	}

	p = new Polygon(this, 4);
	p->add(7,  0, 0, fA, fR, fG, fB);;
	p->add(0,  0, 0, fA, fR, fG, fB);
	p->add(8,  0, 0, fA, fR, fG, fB);
	p->add(15, 0, 0, fA, fR, fG, fB);
	this->add(p);
	// Top
	if (bTop) {
		p = new Polygon(this, 8);
		p->add(7,  0, 0, fA, fR, fG, fB);
		p->add(6,  0, 0, fA, fR, fG, fB);
		p->add(5,  0, 0, fA, fR, fG, fB);
		p->add(4,  0, 0, fA, fR, fG, fB);
		p->add(3,  0, 0, fA, fR, fG, fB);
		p->add(2,  0, 0, fA, fR, fG, fB);
		p->add(1,  0, 0, fA, fR, fG, fB);
		p->add(0,  0, 0, fA, fR, fG, fB);
		this->add(p);
		// Bottom
		p = new Polygon(this, 8);
		p->add(8,  0, 0, fA, fR, fG, fB);
		p->add(9,  0, 0, fA, fR, fG, fB);
		p->add(10, 0, 0, fA, fR, fG, fB);
		p->add(11, 0, 0, fA, fR, fG, fB);
		p->add(12, 0, 0, fA, fR, fG, fB);
		p->add(13, 0, 0, fA, fR, fG, fB);
		p->add(14, 0, 0, fA, fR, fG, fB);
		p->add(15, 0, 0, fA, fR, fG, fB);
		this->add(p);
	}

	this->countNormals();
}
