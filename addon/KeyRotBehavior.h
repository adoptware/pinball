/***************************************************************************
                          KeyRorBehavior.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef KEYROTBEHAVIOR_H
#define KEYROTBEHAVIOR_H

#include "Behavior.h"
/** A Behavior that lets the users rotate the group with UP, DOWN
 * LEFT, RIGHT, PGUP, PGDOWN keys.Simply add a KeyRotationBehavior
 * object to a group to be able to rotate it. */
class KeyRotBehavior : public Behavior
{
	public:
		KeyRotBehavior();
		~KeyRotBehavior();
		virtual void onCollision(const Vertex3D &, const Vertex3D &, Group *) {};
		virtual void onSignal(int, Group *) {};
		void onTick();
};

#endif // KEYROTBEHAVIOR_H
