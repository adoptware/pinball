/***************************************************************************
                            Table.h -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef TABLE_H
#define TABLE_H

#include <string>

class Score;
class Engine;
class BallGroup;

#define PBL_DEAD 0
#define PBL_ACTIVE 1
#define PBL_LOCKED 2

#define MAX_BALL 3

class Table {
 protected:
  Table();
 public:
  ~Table();
  static Table *  getInstance();
  /** Frees any previous table and loads a new one */
  int loadLevel(Engine * engine, const char * subdir);
  BallGroup * getBall(int b);
  inline Score * getScore() { return p_Score; };
  /** First ball is 0, second 1, etc. */
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

 private:
  void clear(Engine * engine);

  static Table * p_Table;
  int m_iBall;
  int m_aBall[MAX_BALL];
  BallGroup * p_Ball[MAX_BALL];
  Score * p_Score;
  string m_sTableName;
};

#endif // TABLE_H

