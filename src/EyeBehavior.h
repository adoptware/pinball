/***************************************************************************
                          KeyBeh.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef EYEBEHAVIOR_H
#define EYEBEHAVIOR_H

#include "Behavior.h"
#include "StateMachine.h"

/**
 * <p>A Behavior for the pinball demo.</p>
 */
class EyeBehavior : public Behavior {
	public:
		EyeBehavior(Group * group);
		~EyeBehavior();
		void onTick();
		void StdOnSignal() {};
		void StdOnCollision() {};
	private:
		Group* m_gBall;
};

#endif // EYEBEHAVIOR_H
