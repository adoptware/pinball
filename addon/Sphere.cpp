/***************************************************************************
                          Sphere.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by
    email                :
 ***************************************************************************/

#include "Sphere.h"
#include "Polygon.h"

Sphere::Sphere(float fSize, float fR, float fG, float fB, float fA) : Shape3D(3*8+2, 5*8) {
	Polygon* p;
	// Top and bottom vertex.
	this->add(0, fSize, 0);
	this->add(0, -fSize, 0);
	// First ring of vertices.
	for (float a=0; a<8; a+=1) {
		this->add(EM_SIN_45*EMath::emSin(a/8)*fSize,
							EM_SIN_45*fSize ,
							EM_SIN_45*EMath::emCos(a/8)*fSize);
	}
	// Second ring.
	for (float a=0; a<8; a+=1) {
		this->add(EMath::emSin(a/8)*fSize, 0 ,EMath::emCos(a/8)*fSize);
	}
	// Third ring.
	for (float a=0; a<8; a+=1) {
		this->add(EM_SIN_45*EMath::emSin(a/8)*fSize, -EM_SIN_45*fSize,
							EM_SIN_45*EMath::emCos(a/8)*fSize);
	}
	// First ring of polygons.
	for (int a=2; a<9; a++) {
		p = new Polygon(this, 3);
		p->add(0,  0, 0, fR, fG, fB, fA);
		p->add(a+1,0, 0, fR, fG, fB, fA);
		p->add(a,  0, 0, fR, fG, fB, fA);
		this->add(p);
	}

	p = new Polygon(this, 3);
	p->add(0,  0, 0, fR, fG, fB, fA);
	p->add(2,  0, 0, fR, fG, fB, fA);
	p->add(9,  0, 0, fR, fG, fB, fA);
	this->add(p);
	// Second ring.
	for (int a=2; a<9; a++) {
		p = new Polygon(this, 4);
		p->add(a,  0, 0, fR, fG, fB, fA);
		p->add(a+1,0, 0, fR, fG, fB, fA);
		p->add(a+9,0, 0, fR, fG, fB, fA);
		p->add(a+8,0, 0, fR, fG, fB, fA);
		this->add(p);
	}

	p = new Polygon(this, 4);
	p->add(9, 0, 0, fR, fG, fB, fA);
	p->add(2, 0, 0, fR, fG, fB, fA);
	p->add(10, 0, 0, fR, fG, fB, fA);
	p->add(17, 0, 0, fR, fG, fB, fA);
	this->add(p);
	// Third ring.
	for (int a=10; a<17; a++)	{
		p = new Polygon(this, 4);
		p->add(a,  0, 0, fR, fG, fB, fA);
		p->add(a+1,0, 0, fR, fG, fB, fA);
		p->add(a+9,0, 0, fR, fG, fB, fA);
		p->add(a+8,0, 0, fR, fG, fB, fA);
		this->add(p);
	}

	p = new Polygon(this, 4);
	p->add(17,  0, 0, fR, fG, fB, fA);
	p->add(10,  0, 0, fR, fG, fB, fA);
	p->add(18,  0, 0, fR, fG, fB, fA);
	p->add(25,  0, 0, fR, fG, fB, fA);
	this->add(p);
	// Fourth ring.
	for (int a=18; a<25; a++)	{
		p = new Polygon(this, 3);
		p->add(a,  0, 0, fR, fG, fB, fA);
		p->add(a+1,0, 0, fR, fG, fB, fA);
		p->add(1,  0, 0, fR, fG, fB, fA);
		this->add(p);
	}

	p = new Polygon(this, 3);
	p->add(25,  0, 0, fR, fG, fB, fA);
	p->add(18,  0, 0, fR, fG, fB, fA);
	p->add(1,   0, 0, fR, fG, fB, fA);
	this->add(p);

	this->countNormals();
}
