/***************************************************************************
                          Cylinder.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef CYLINDER_H
#define CYLINDER_H

#include "EMath.h"
#include "Shape3D.h"
/** A simple cylinder. @see Shape3D */
class Cylinder : public Shape3D {
	public:
		/** Cylinder(size, r, g, b, a). You must enable transparency to be able
		 * to use the alpha color. */
		Cylinder(float size, float r, float g, float b, float a, bool bTop = true);
};

#endif // SPHERE_H
