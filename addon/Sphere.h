/***************************************************************************
                          Sphere.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef SPHERE_H
#define SPHERE_H

#include "Shape3D.h"

/** The BigSphere class is a nicer than this sphere. @see BigSphere */
class Sphere : public Shape3D {
 public:
	Sphere(float fSize, float fR, float fG, float fB, float fA);
};

#endif // SPHERE_H
