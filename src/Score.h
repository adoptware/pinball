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
#define PBL_ACTIVE 1
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
	void addScore(int s, bool multi = false);
	void clear();
	void draw();
	void setText1(const char * text);
	void setText2(const char * text);
	void setText3(const char * text);
	void setText4(const char * text);
	void clearText();
	void lockBall(int ball);
 	void unLockBall(int ball);
	bool isBallLocked(int ball);
	bool isBallDead(int ball);
	void activateBall(int ball);
	void unActivateBall(int ball);
  bool isBallActive(int ball);
 	int locked();
 	int active();
	int getCurrentBall() { return m_iBall; };
	void setCurrentBall(int ball) { m_iBall = ball; };
	void setShowFPS(bool fps) { m_bShowFPS = fps; };
	bool getShowFPS() { return m_bShowFPS; };
	void setMultiplier(int m) { m_iFactor = m; };
	int getMultiplier() { return m_iFactor; };
 private:
	char m_Text1[64];
	char m_Text2[64];
	char m_Text3[64];
	char m_Text4[64];
	bool m_bShowFPS;
	int m_iScore;
	//	int m_aMission[8];
	int m_iFactor;
/* 	bool m_aTarget[2]; */
/* 	bool m_aLinux[5]; */
/* 	bool m_aBoot[4]; */
/* 	bool m_aTux[3]; */
	EmFont * m_Font;
	int m_iBall;
	int m_aBall[4];

	static Score* p_Score;
};

#endif // SCORE_H
