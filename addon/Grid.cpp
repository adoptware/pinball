/***************************************************************************
                          Grid.cpp  -  description
                             -------------------
    begin                : Tue Feb 8 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "Grid.h"
#include "Polygon.h"

Grid::Grid(EmTexture* texture, float sizex, float sizey, int step, float bmStep,
		float fR, float fG, float fB, float fA) : Shape3D((step+1)*(step+1), step*step) {
	Polygon * poly;

	sizex /= step;
	sizey /= step;
	
	for (int a=0; a<=step; a++) {
		for (int b=0; b<=step; b++) {
			this->add( a*sizex - step*sizex*0.5, 0, -b*sizey + step*sizey*0.5);
		}
	}

	for (int a=0; a<step; a++) {
		for (int b=0; b<step; b++) {	
			poly = new Polygon(this, 4);
			poly->add(a + b*(step+1),					a*bmStep, b*bmStep, 				fR, fG, fB, fA);
			poly->add((a+1) + b*(step+1),			(a+1)*bmStep, b*bmStep, 		fR, fG, fB, fA);
			poly->add((a+1) + (b+1)*(step+1),	(a+1)*bmStep, (b+1)*bmStep, fR, fG, fB, fA);
			poly->add(a + (b+1)*(step+1),			a*bmStep, (b+1)*bmStep, 		fR, fG, fB, fA);
			this->add(poly);
		}
	}

	this->setTexture(texture);
	this->countNormals();
}

