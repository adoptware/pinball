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
		virtual void onCollision(const Vertex3D & vtxWall, const Vertex3D & vtxOwn, Group * g) {};
		virtual void onSignal(int signal, Group * sender) {};
		void onTick();
};

#endif // KEYROTBEHAVIOR_H
