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
		void clear();
		void draw();
	private:
		EmSound m_aSample[256];
		char* m_Text1;
		char* m_Text2;
		bool m_baAliveBall[4];
		int m_iActiveBalls;
		int m_iBallsLeft;
		int m_iScore;
		int m_iBumps;
		int m_aMission[8];
		bool m_aTarget[2];
		bool m_aLinux[5];
		bool m_bExtraBall;
		EmFont * m_Font;
};

#endif // SCORE_H
