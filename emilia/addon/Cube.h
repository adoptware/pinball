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
/**
 * <p>Class Cube is a simple way to create a Shape3D that looks
 * like a Cube.</p>
 * @see Shape3D
 */
class Cube : public Shape3D
{
	public:
		/** <p>Cube(size,r, g, b).</p> */
		Cube(float size, float r, float g, float b, float a);
		/** <p>Cube(size, textureAppliedToEverySide). Texture size 32x32.</p> < */
		Cube(float, EmTexture*);
};

#endif // CUBE_H
