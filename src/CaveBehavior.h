/***************************************************************************
                          BouBeh.h  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef CAVEBEHAVIOR_E
#define CAVEBEHAVIOR_E

#include "Behavior.h"
#include "EMath.h"
#include "Light.h"
#include "StateMachine.h"

/** A behavior for the pinball demo. */

class CaveBehavior : public Behavior {
	public:
		CaveBehavior();
		~CaveBehavior();
		void onTick();
		void StdOnSignal() {};
		void StdOnCollision();
		int getBall() { return m_iBall; };
	private:
		int m_iBall;
		int m_iLockCounter;
		bool m_bLock;
};

#endif // CAVEBEHAVIOR_H
