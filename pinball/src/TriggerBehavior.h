/***************************************************************************
                          TriggerBehavior.h  -  description
                             -------------------
    begin                : Thu Jan 25 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef TRIGGERBEHAVIOR_H
#define TRIGGERBEHAVIOR_H

#include "Behavior.h"
#include "StateMachine.h"

class Light;

/**
  *@author Henrik Enqvist
  */

class TriggerBehavior : public Behavior  {
	public:
		TriggerBehavior(int aSignal, int uSignal, int signal, bool init=false);
		~TriggerBehavior();
		void onTick();
		void StdOnSignal();
		void StdOnCollision();
	private:
		int m_iActiveSignal;
		int m_iUnActiveSignal;
		int m_iSignal;
		int m_iTick;
		bool m_bActive;
		bool m_bInit;
};

#endif
