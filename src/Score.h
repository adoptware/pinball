/***************************************************************************
                          Score.h  -  description
                             -------------------
    begin                : Fri Jan 26 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef SCORE_H
#define SCORE_H

#include "Behavior.h"
#include "StateMachine.h"

#define PBL_DEAD 0
#define PBL_ALIVE 1
#define PBL_LOCKED 2

class EmFont;

/** @author Henrik Enqvist */
class Score : public Behavior  {
 protected:
	Score();
 public:
	~Score();
	static Score* getInstance();
	void onTick();
	void StdOnSignal();
	void StdOnCollision() {};
	int getScore() { return m_iScore; };
	void addScore(int s);
	void clear();
	void draw();
	void lockBall(int ball);
	int locked();
	int alive();
	void unLockBall(int ball);
	bool isBallActive(int ball);
	void setShowFPS(bool fps) { m_bShowFPS = fps; };
	bool getShowFPS() { return m_bShowFPS; };
 private:
	char* m_Text1;
	char* m_Text2;
	char* m_Text3;
	char* m_Text4;
	int m_aAliveBall[4];
	bool m_bLaunch;
	bool m_bShowFPS;
	int m_iBall;
	int m_iScore;
	int m_iBumps;
	//	int m_aMission[8];
	int m_iFactor;
/* 	bool m_aTarget[2]; */
/* 	bool m_aLinux[5]; */
/* 	bool m_aBoot[4]; */
/* 	bool m_aTux[3]; */
	bool m_bExtraBall;
	EmFont * m_Font;

	static Score* p_Score;
};

#endif // SCORE_H
