/***************************************************************************
                          Cube.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "Cube.h"
#include "Polygon.h"

Cube::Cube(float fSize, EmTexture* tex) : Shape3D(8, 6) {
	Polygon* p;
	float fA = 1.0f;
	float fR = 1.0f;
	float fG = 1.0f;
	float fB = 1.0f;

	this->add( fSize/2,  fSize/2, -fSize/2);
	this->add(-fSize/2,  fSize/2, -fSize/2);
	this->add( fSize/2, -fSize/2, -fSize/2);
	this->add(-fSize/2, -fSize/2, -fSize/2);
	this->add( fSize/2,  fSize/2,  fSize/2);
	this->add(-fSize/2,  fSize/2,  fSize/2);
	this->add( fSize/2, -fSize/2,  fSize/2);
	this->add(-fSize/2, -fSize/2,  fSize/2);

	this->setTexture(tex);

	p = new Polygon(this, 4);
	p->add(0,  0, 0,  fA, fR, fG, fB);
	p->add(1,  1, 0,  fA, fR, fG, fB);
	p->add(3,  1, 1,  fA, fR, fG, fB);
	p->add(2,  0, 1,  fA, fR, fG, fB);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(4,  0, 0,  fA, fR, fG, fB);
	p->add(0,  1, 0,  fA, fR, fG, fB);
	p->add(2,  1, 1,  fA, fR, fG, fB);
	p->add(6,  0, 1,  fA, fR, fG, fB);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(5,  0, 0,  fA, fR, fG, fB);
	p->add(4,  1, 0,  fA, fR, fG, fB);
	p->add(6,  1, 1,  fA, fR, fG, fB);
	p->add(7,  0, 1,  fA, fR, fG, fB);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(1,  0, 0,  fA, fR, fG, fB);
	p->add(5,  1, 0,  fA, fR, fG, fB);
	p->add(7,  1, 1,  fA, fR, fG, fB);
	p->add(3,  0, 1,  fA, fR, fG, fB);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(0,  0, 0,  fA, fR, fG, fB);
	p->add(4,  1, 0,  fA, fR, fG, fB);
	p->add(5,  1, 1,  fA, fR, fG, fB);
	p->add(1,  0, 1,  fA, fR, fG, fB);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(2,  0, 0,  fA, fR, fG, fB);
	p->add(3,  1, 0,  fA, fR, fG, fB);
	p->add(7,  1, 1,  fA, fR, fG, fB);
	p->add(6,  0, 1,  fA, fR, fG, fB);
	this->add(p);

	this->countNormals();
}

Cube::Cube(float fSize, float fR, float fG, float fB, float fA) : Shape3D(8, 6) {
	Polygon* p;

	this->add( fSize/2,  fSize/2, -fSize/2);
	this->add(-fSize/2,  fSize/2, -fSize/2);
	this->add( fSize/2, -fSize/2, -fSize/2);
	this->add(-fSize/2, -fSize/2, -fSize/2);
	this->add( fSize/2,  fSize/2,  fSize/2);
	this->add(-fSize/2,  fSize/2,  fSize/2);
	this->add( fSize/2, -fSize/2,  fSize/2);
	this->add(-fSize/2, -fSize/2,  fSize/2);

	p = new Polygon(this, 4);
	p->add(0,	 0, 0, fA, fR, fG, fB);
	p->add(1,	 0, 0, fA, fR, fG, fB);
	p->add(3,	 0, 0, fA, fR, fG, fB);
	p->add(2,	 0, 0, fA, fR, fG, fB);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(4,	 0, 0, fA, fR, fG, fB);
	p->add(0,	 0, 0, fA, fR, fG, fB);
	p->add(2,	 0, 0, fA, fR, fG, fB);
	p->add(6,	 0, 0, fA, fR, fG, fB);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(5,	 0, 0, fA, fR, fG, fB);
	p->add(4,	 0, 0, fA, fR, fG, fB);
	p->add(6,	 0, 0, fA, fR, fG, fB);
	p->add(7,	 0, 0, fA, fR, fG, fB);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(1,	 0, 0, fA, fR, fG, fB);
	p->add(5,	 0, 0, fA, fR, fG, fB);
	p->add(7,	 0, 0, fA, fR, fG, fB);
	p->add(3,	 0, 0, fA, fR, fG, fB);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(0,	 0, 0, fA, fR, fG, fB);
	p->add(4,	 0, 0, fA, fR, fG, fB);
	p->add(5,	 0, 0, fA, fR, fG, fB);
	p->add(1,	 0, 0, fA, fR, fG, fB);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(2,	 0, 0, fA, fR, fG, fB);
	p->add(3,	 0, 0, fA, fR, fG, fB);
	p->add(7,	 0, 0, fA, fR, fG, fB);
	p->add(6,	 0, 0, fA, fR, fG, fB);
	this->add(p);

	this->countNormals();
}
