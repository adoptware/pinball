/***************************************************************************
                          SwitchBehavior.h  -  description
                             -------------------
    begin                : Thu Feb 8 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef SWITCHBEHAVIOR_H
#define SWITCHBEHAVIOR_H

#include "Behavior.h"
#include "StateMachine.h"

/** The switch behavior turns on the group on if the active signal is 
 * recieved. If the  */
class SwitchBehavior : public Behavior  {
	public:
		SwitchBehavior(int aSignal, int uSignal, bool init=false);
		~SwitchBehavior();
		void StdOnSignal();
		void StdOnCollision() {};
		void onTick() {};
	private:
		int m_iActiveSignal;
		int m_iUnActiveSignal;
		bool m_bInit;
};

#endif // SWITCHBEHAVIOR_H
