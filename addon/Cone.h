/***************************************************************************
                          Cone.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef CONE_H
#define CONE_H

#include "Private.h"
#include "EMath.h"
#include "Shape3D.h"

/**
 * <p>Class Cone is a simple way to create a Shape3D that looks
 * like a cone.</p>
 * @see Shape3D
 */
class Cone : public Shape3D {
	public:
		/** <p>Cone(size, r, g, b, a). Color given by r, g and b. Alpha
		 * (a) not implemented yet.</p> */
		Cone(float size, float a, float r, float g, float b);
};

#endif // CONE_H
