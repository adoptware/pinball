/***************************************************************************
                          EyeBehavior.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
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
	void StdOnSignal();
	void StdOnCollision() { EmAssert(false, "EyeBehavior::StdOnCollision() not implemented"); };
	inline void setSound(int s) { m_iSound = s; };
 private:
	int m_iNudgeTick;
	int m_iNudgeType;
	int m_iTiltTick;
	float m_fXNudge;
	float m_fZNudge;
	Group* p_gBall1;
	Group* p_gBall2;
	Group* p_gBall3;
	Score* p_Score;
	int m_iSound;
};

#endif // EYEBEHAVIOR_H
