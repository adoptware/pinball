/***************************************************************************
                          Cube.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef CUBE_H
#define CUBE_H

#include "Shape3D.h"
/** A simple cube. @see Shape3D */
class Cube : public Shape3D {
 public:
	/** Cube(size, r, g, b, a). You must enable transaparency to be able to
	 * use the alpha color. */
	Cube(float size, float r, float g, float b, float a);
	/** Cube(size, textureAppliedToEverySide). */
	Cube(float, EmTexture*);
};

#endif // CUBE_H
