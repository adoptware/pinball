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

class Light;
class Group;

/**
  *@author Henrik Enqvist
  */

class SwitchBehavior : public Behavior  {
	public:
		SwitchBehavior(int aSignal, int uSignal, bool init=false);
		~SwitchBehavior();
		void StdOnSignal();
	private:
		int m_iActiveSignal;
		int m_iUnActiveSignal;
		bool m_bInit;
};

#endif // SWITCHBEHAVIOR_H
