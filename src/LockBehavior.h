/***************************************************************************
                          BouBeh.h  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef LOCKBEHAVIOR_H
#define LOCKBEHAVIOR_H

#include "Behavior.h"
#include "EMath.h"
#include "StateMachine.h"

class Light;

/**
 * <p>A behavior for the pinball demo.</p>
 */

class LockBehavior : public Behavior {
	public:
		LockBehavior(int as, int us, int ls, bool lock = false);
		~LockBehavior();
		void StdOnSignal();
		void StdOnCollision();
		void onTick() {};
		int m_iBall;
	private:
		int m_iActiveSignal;
		int m_iUnActiveSignal;
		int m_iLockSignal;
		bool m_bActive;
};

#endif // LOCKBEHAVIOR_H
