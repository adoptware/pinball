/***************************************************************************
                          KeyRBeh.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef KEYROTBEHAVIOR_H
#define KEYROTBEHAVIOR_H

#include "Private.h"

#include "Behavior.h"
/**
 * <p>A Behavior that lets the users rotate the group with UP, DOWN
 * LEFT, RIGHT, PGUP, PGDOWN keys.Simply add a KeyRotationBehavior
 * object to a group to be able to rotate it.</p>
 */
class KeyRotBehavior : public Behavior
{
	public:
		KeyRotBehavior();
		~KeyRotBehavior();
		virtual void onCollision(const Vertex3D & vtxWall, const Vertex3D & vtxOwn, Group * g) {};
		virtual void onSignal(int signal, Group * sender) {};
		void onTick();
	private:
		float m_x, m_y, m_z;
};

#endif // KEYROTBEHAVIOR_H
