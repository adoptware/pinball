/***************************************************************************
                          Cone.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "Cone.h"
#include "Polygon.h"

Cone::Cone(float fSize, float fR, float fG, float fB, float fA) : Shape3D(8+1, 8+1) {
	Polygon* p;
	// Top vertex.
	this->add(0, fSize/2, 0);
	// Ring of vertices.
	for (float a=0; a<8; a+=1)	{
		this->add( EMath::emSin(a/8)*fSize/2, -fSize/2 , EMath::emCos(a/8)*fSize/2);
	}	
	// The ring of polygons.
	for (int a=1; a<8; a+=1)	{
		p = new Polygon(this, 3);
		p->add(0,  0, 0, fA, fR, fG, fB);
		p->add(a+1,0, 0, fA, fR, fG, fB);
		p->add(a,  0, 0, fA, fR, fG, fB);
		this->add(p);
	}
	
	p = new Polygon(this, 3);
	p->add(0,  0, 0, fA, fR, fG, fB);
	p->add(1,  0, 0, fA, fR, fG, fB);
	p->add(8,  0, 0, fA, fR, fG, fB);
	this->add(p);
	// The bottom of the cone.
	p = new Polygon(this, 8);
	p->add(1,  0, 0, fA, fR, fG, fB);
	p->add(2,  0, 0, fA, fR, fG, fB);
	p->add(3,  0, 0, fA, fR, fG, fB);
	p->add(4,  0, 0, fA, fR, fG, fB);
	p->add(5,  0, 0, fA, fR, fG, fB);
	p->add(6,  0, 0, fA, fR, fG, fB);
	p->add(7,  0, 0, fA, fR, fG, fB);
	p->add(8,  0, 0, fA, fR, fG, fB);
	this->add(p);
	
	this->countNormals();
}
