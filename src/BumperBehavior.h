/***************************************************************************
                          BumperBehavior.h  -  description
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

/** A behavior for the pinball game. */
class BumperBehavior : public Behavior {
 public:
	BumperBehavior();
	~BumperBehavior();
	void onTick();
	void StdOnSignal() {};
	void StdOnCollision();
	inline void setSound(int s) { m_iSound = s; };
	inline int getSound() { return m_iSound; };
	inline void setScore(int s) { m_iScore = s; };
	inline int getScore() { return m_iScore; };
 private:
	int m_iLightCounter;
	int m_iSound;
	int m_iScore;
};

#endif // BounceBehavior
