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

/**
 * <p>A behavior for the pinball demo.</p>
 */

class CaveBehavior : public Behavior {
	public:
		CaveBehavior();
		~CaveBehavior();
		void onTick();
		void StdOnSignal() {};
		void StdOnCollision();
	private:
		int m_iLockCounter;
		bool m_bLock;
};

#endif // CAVEBEHAVIOR_H
