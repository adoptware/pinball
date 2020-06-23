//#ident "$Id: Cylinder.cpp,v 1.7 2003/05/12 12:17:58 rzr Exp $"
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

Cylinder::Cylinder(float fSize, int sides, float fR, float fG, float fB, float fA, bool bTop) : Shape3D(sides+sides, sides+2) {
	if (sides < 3) sides = 3;
	Polygon3D * p;
	// First ring of vertices.
	{ for (float a=0; a<sides; ++a) {
		this->add(EMath::emSin(a/sides)*fSize/2, (float)fSize/2 , EMath::emCos(a/sides)*fSize/2,
							fR, fG, fB, fA, 0.0f, 0.0f);
	} }	
	// Second ring.
	{ for (float a=0; a<sides; ++a) {
		this->add(EMath::emSin(a/sides)*fSize/2, (float)-fSize/2, EMath::emCos(a/sides)*fSize/2,
							fR, fG, fB, fA, 0.0f, 0.0f);
	} }
	// Add vertices.
	// The ring of polygons.
	{ for (int a=0; a<sides-1; ++a) {
		p = new Polygon3D(this, 4);
		p->add(a);
		p->add(a+1);
		p->add(a+sides+1);
		p->add(a+sides);
		this->add(p);
	} }

	p = new Polygon3D(this, 4);
	p->add(sides-1);
	p->add(0);
	p->add(sides);
	p->add(sides+sides-1);
	this->add(p);
	// Top
	if (bTop) {
		p = new Polygon3D(this, sides);
		for (int a=sides-1; a>=0; --a) {
			p->add(a);
		}
		this->add(p);
		// Bottom
		p = new Polygon3D(this, sides);
		{ for (int a=sides; a<sides+sides; ++a) {
			p->add(a);
		} }
		this->add(p);
	}

	this->countNormals();
}
