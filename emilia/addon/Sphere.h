/***************************************************************************
                          Sphere.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef SPHERE_H
#define SPHERE_H

#include "Private.h"

#include "Shape3D.h"

/**
 *
 */
class Sphere : public Shape3D {
	public:
		Sphere(float fSize, float fR, float fG, float fB, float fA);
//		Sphere(float, EmBitmap*);
};

#endif // SPHERE_H
