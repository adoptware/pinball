/***************************************************************************
                          Score.h  -  description
                             -------------------
    begin                : Fri Jan 26 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef SCORE_H
#define SCORE_H

#include "Private.h"
#include "Behavior.h"
#include "StateMachine.h"

class EmFont;

/**
  *@author Henrik Enqvist
  */

class Score : public Behavior  {
	public:
		Score();
		~Score();
		void onTick();
		void StdOnSignal();
		void StdOnCollision() {};
		int getScore() {return m_iScore;};
		void empty();
		void draw();
	private:
		bool m_baAliveBalls[4];
		int m_iActiveBalls;
		int m_iBallsLeft;
		int m_iScore;
		int m_iBumps;
		int m_aMission[8];
		EmSound m_aSample[256];
		bool m_aTargets[2];
		bool m_bExtraBall;
		EmFont * m_Font;
};

#endif // SCORE_H
