/***************************************************************************
                          Cone.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef CONE_H
#define CONE_H

#include "EMath.h"
#include "Shape3D.h"

/** A simple cone. @see Shape3D */
class Cone : public Shape3D {
 public:
	/** Cone(size, r, g, b, a). Color given by r, g, b and a. You must enable
	 * transparency to use the alpha color. */
	Cone(float size, float r, float g, float b, float a);
};

#endif // CONE_H
