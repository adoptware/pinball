/***************************************************************************
                   ation, Inc.
#
# This proription
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef BUMPERBEHAVIOR_E
#define BUMPERBEHAVIOR_E

#include "Behavior.h"
#include "EMath.h"
#include "Light.h"
#include "StateMachine.h"

/**
 * <p>A behavior for the pinball demo.</p>
 */

class BumperBehavior : public Behavior {
	public:
		BumperBehavior();
		~BumperBehavior();
		void onTick();
		void StdOnSignal() {};
		void StdOnCollision();
		void setSound(int s) { m_iSound = s; };
	private:
		int m_iLightCounter;
		int m_iSound;
};

#endif // BounceBehavior
