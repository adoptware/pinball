/***************************************************************************
                          Cube.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef CUBE_H
#define CUBE_H

#include "Private.h"

#include "Shape3D.h"
/** Class Cube is a simple way to create a Shape3D that looks
 * like a Cube.
 * @see Shape3D */
class Cube : public Shape3D {
 public:
	/** Cube(size, r, g, b, a) */
	Cube(float size, float r, float g, float b, float a);
	/** Cube(size, textureAppliedToEverySide). Texture size 32x32. */
	Cube(float, EmTexture*);
};

#endif // CUBE_H
