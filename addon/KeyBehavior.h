/***************************************************************************
                          KeyBehavior.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef KEYBEHAVIOR_H
#define KEYBEHAVIOR_H

#include "Behavior.h"

/** A Behavior that lets the users move the group with A, S, D, Q, W, E keys.
 * Simply add a KeyBehavior object to a group to be able to move it.  */
class KeyBehavior : public Behavior {
	public:
		KeyBehavior();
		~KeyBehavior();
		void onTick();
		virtual void onCollision(const Vertex3D &, const Vertex3D &, Group *) {};
		virtual void onSignal(int, Group *) {};
	private:
		float m_x, m_y, m_z;
};

#endif // KEYBEHAVIOR_H
