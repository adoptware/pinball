/***************************************************************************
                          ColorBehavior.h  -  description
                             -------------------
    begin                : Tue Feb 15 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef COLORBEHAVIOR_H
#define COLORBEHAVIOR_H

#include "Behavior.h"
#include "EMath.h"
#include "StateMachine.h"

/** A simple behavior that changes the color of all polygons in the Shape3D
 * to blue and then when collision occurs all polygons are changed to red.
 * The collision example uses this to demonstrate when collision is
 * detected between two Shape3Ds. */
class ColorBehavior : public Behavior {
 public:
	ColorBehavior();
	~ColorBehavior();
	void onCollision(const Vertex3D & vtxWall, const Vertex3D & vtxOwn, Group * g);
	void onTick();
	void StdOnSignal();
};

#endif // COLORBEHAVIOR_H
