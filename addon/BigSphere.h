/***************************************************************************
                          BigSphere.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef BIGSPHERE_H
#define BIGSPHERE_H

#include "Shape3D.h"

/** */
class BigSphere : public Shape3D {
	public:
		BigSphere(float fSize, int level, float fR, float fG, float fB, float fA);
};

#endif // SPHERE_H
