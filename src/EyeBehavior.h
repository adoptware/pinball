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

class Score;

/**
 * <p>A Behavior for the pinball demo.</p>
 */
class EyeBehavior : public Behavior {
	public:
		EyeBehavior(Group* g1, Group* g2, Group* g3);
		~EyeBehavior();
		void onTick();
		void StdOnSignal() {};
		void StdOnCollision() {};
	private:
		Group* p_gBall1;
		Group* p_gBall2;
		Group* p_gBall3;
		Score* p_Score;
};

#endif // EYEBEHAVIOR_H
