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

class EmFont;

/*
#define STATE_STOPPED    1
#define STATE_RUNNING    2
#define STATE_TILT       3
#define STATE_WARNING    4
*/

/** @author Henrik Enqvist */
class Score : public Behavior  {
 public:
  Score();
  ~Score();
  //static Score* getInstance();
  void onTick();
  void StdOnSignal();
  void StdOnCollision() {};
  int getScore() { return m_iScore; };
/*   void addScore(int s, bool multi = false); */
  void addScore(int s) { m_iScore += s; };
  void clear();
  void draw();
  void setText1(const char * text);
  void setText2(const char * text);
  void setText3(const char * text);
  void setText4(const char * text);
  void setInfoText(const char * text, int delay);
  void clearText();
  void setShowFPS(bool fps) { m_bShowFPS = fps; };
  bool getShowFPS() { return m_bShowFPS; };
/*   void setMultiplier(int m) { m_iFactor = m; }; */
/*   int getMultiplier() { return m_iFactor; }; */

  bool testForHighScore();

 private:
  char m_Text1[64];
  char m_Text2[64];
  char m_Text3[64];
  char m_Text4[64];
  char m_InfoText[64];
  int m_iInfoDelay;
  bool m_bShowFPS;
  int m_iScore;
  EmFont * m_Font;
  //static Score* p_Score;
};

#endif // SCORE_H
