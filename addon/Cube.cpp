/***************************************************************************
                          Cube.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "Cube.h"
#include "Polygon.h"

Cube::Cube(float fSize, EmTexture* tex) : Shape3D(8, 6) {
	Polygon* p;
	float fA = 1.0f;
	float fR = 1.0f;
	float fG = 1.0f;
	float fB = 1.0f;

	this->add( fSize/2,  fSize/2, -fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add(-fSize/2,  fSize/2, -fSize/2,
						fR, fG, fB, fA, 0.0f, 0.0f);
	this->add( fSize/2, -fSize/2, -fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add(-fSize/2, -fSize/2, -fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add( fSize/2,  fSize/2,  fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add(-fSize/2,  fSize/2,  fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add( fSize/2, -fSize/2,  fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add(-fSize/2, -fSize/2,  fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);

	this->setTexture(tex);

	p = new Polygon(this, 4);
	p->add(0);
	p->add(1);
	p->add(3);
	p->add(2);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(4);
	p->add(0);
	p->add(2);
	p->add(6);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(5);
	p->add(4);
	p->add(6);
	p->add(7);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(1);
	p->add(5);
	p->add(7);
	p->add(3);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(0);
	p->add(4);
	p->add(5);
	p->add(1);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(2);
	p->add(3);
	p->add(7);
	p->add(6);
	this->add(p);

	this->countNormals();
}

Cube::Cube(float fSize, float fR, float fG, float fB, float fA) : Shape3D(8, 6) {
	Polygon* p;

	this->add( fSize/2,  fSize/2, -fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add(-fSize/2,  fSize/2, -fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add( fSize/2, -fSize/2, -fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add(-fSize/2, -fSize/2, -fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add( fSize/2,  fSize/2,  fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add(-fSize/2,  fSize/2,  fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add( fSize/2, -fSize/2,  fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);
	this->add(-fSize/2, -fSize/2,  fSize/2,
						fR, fG, fB, fA,	0.0f, 0.0f);

	p = new Polygon(this, 4);
	p->add(0);
	p->add(1);
	p->add(3);
	p->add(2);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(4);
	p->add(0);
	p->add(2);
	p->add(6);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(5);
	p->add(4);
	p->add(6);
	p->add(7);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(1);
	p->add(5);
	p->add(7);
	p->add(3);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(0);
	p->add(4);
	p->add(5);
	p->add(1);
	this->add(p);

	p = new Polygon(this, 4);
	p->add(2);
	p->add(3);
	p->add(7);
	p->add(6);
	this->add(p);

	this->countNormals();
}
