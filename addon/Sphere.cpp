//#ident "$Id: Sphere.cpp,v 1.6 2003/05/12 12:17:58 rzr Exp $"
/***************************************************************************
                          Sphere.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "Sphere.h"
#include "Polygon.h"

Sphere::Sphere(float fSize, float fR, float fG, float fB, float fA) : Shape3D(3*8+2, 5*8) {
	Polygon3D * p;
	// Top and bottom vertex.
	this->add(0, fSize, 0);
	this->add(0, -fSize, 0);
	// First ring of vertices.
	{ for (float a=0; a<8; a+=1) {
		this->add(EM_SIN_45*EMath::emSin(a/8)*fSize, EM_SIN_45*fSize, EM_SIN_45*EMath::emCos(a/8)*fSize,
							fR, fG, fB, fA, 0.0f, 0.0f);
	} }
	// Second ring.
	{ for (float a=0; a<8; a+=1) {
		this->add(EMath::emSin(a/8)*fSize, 0 ,EMath::emCos(a/8)*fSize,
							fR, fG, fB, fA, 0.0f, 0.0f);
	} }
	// Third ring.
	{ for (float a=0; a<8; a+=1) {
		this->add(EM_SIN_45*EMath::emSin(a/8)*fSize, -EM_SIN_45*fSize, EM_SIN_45*EMath::emCos(a/8)*fSize,
							fR, fG, fB, fA, 0.0f, 0.0f);

	} }
	// First ring of polygons.
	{ for (int a=2; a<9; a++) {
		p = new Polygon3D(this, 3);
		p->add(0);
		p->add(a+1);
		p->add(a);
		this->add(p);
	} }

	p = new Polygon3D(this, 3);
	p->add(0);
	p->add(2);
	p->add(9);
	this->add(p);
	// Second ring.
	{ for (int a=2; a<9; a++) {
		p = new Polygon3D(this, 4);
		p->add(a);
		p->add(a+1);
		p->add(a+9);
		p->add(a+8);
		this->add(p);
	} }

	p = new Polygon3D(this, 4);
	p->add(9);
	p->add(2);
	p->add(10);
	p->add(17);
	this->add(p);
	// Third ring.
	{ for (int a=10; a<17; a++)	{
		p = new Polygon3D(this, 4);
		p->add(a);
		p->add(a+1);
		p->add(a+9);
		p->add(a+8);
		this->add(p);
	} }

	p = new Polygon3D(this, 4);
	p->add(17);
	p->add(10);
	p->add(18);
	p->add(25);
	this->add(p);
	// Fourth ring.
	{ for (int a=18; a<25; a++)	{
		p = new Polygon3D(this, 3);
		p->add(a);
		p->add(a+1);
		p->add(1);
		this->add(p);
	} }

	p = new Polygon3D(this, 3);
	p->add(25);
	p->add(18);
	p->add(1);
	this->add(p);

	this->countNormals();
}
