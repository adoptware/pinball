/***************************************************************************
                          KeyBeh.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef ARMBEHAVIOR_H
#define ARMBEHAVIOR_H

#include "Behavior.h"
#include "StateMachine.h"

/** A Behavior for the pinball demo. */
class ArmBehavior : public Behavior {
 public:
	ArmBehavior(bool right = true);
	~ArmBehavior();
	void onTick();
	void StdOnSignal() {};
	void StdOnCollision() {};
	void setSound(int s) { m_iSound = s; };
 private:
	bool m_bRight;
	int m_iCount;
	bool m_bOn;
	int m_iSound;
};

#endif // ARMBEHAVIOR_H
