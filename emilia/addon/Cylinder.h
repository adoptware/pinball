/***************************************************************************
                          Cylinder.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef CYLINDER_H
#define CYLINDER_H

#include "Private.h"
#include "EMath.h"
#include "Shape3D.h"
/**
 * <p>Class Cylinder is a simple way to create a Shape3D that looks
 * like a Cylinder.</p>
 * @see Shape3D
 */
class Cylinder : public Shape3D {
	public:
		/** <p>Cylinder(size, r, g, b, a).</p> */
		Cylinder(float size, float r, float g, float b, float a, bool bTop = true);
};

#endif // SPHERE_H
