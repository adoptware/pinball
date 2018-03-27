//#ident "$Id: Table.h,v 1.9 2003/06/13 13:39:48 rzr Exp $"
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
#include <map>
#include <list>

class Score;
class Engine;
class BallGroup;

#define PBL_DEAD 0
#define PBL_ACTIVE 1
#define PBL_LOCKED 2

#define MAX_BALL 3

#define EM_MAX_HIGHSCORES 5  // !?! isn't it better 10 ...?

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
  /** starts a new balls and drops it x,y,z */
  void activateBall(int ball, float x, float y, float z);
  void unActivateBall(int ball);
  bool isBallActive(int ball);
  int locked();
  int active();
  int getCurrentBall() { return m_iBall; };
  void setCurrentBall(int ball) { m_iBall = ball; };

  /// Get the table name - pnf
  string getTableName() const;
  /// Get the tables data directory name - pnf
  string getTableDataDirName() const;
  /// Returns true if the argument value is a high score - pnf
  bool isItHighScore(const int nNewScore) const;
  /// Returns the high scores data in a string array
  bool getHighScoresData(list<string>& listHighScores) const;
  /// Save new high score in high scores table
  void saveNewHighScore(int nNewHighScore, const char * name);
  /// Read high scores from given file - pnf
  bool readHighScoresFile();
  /// Write high scores to a given file - pnf
  bool writeHighScoresFile();

  void clear(Engine * engine); //!rzr  put  as public for w32 resize bug

private:

  static Table * p_Table;
  int m_iBall;
  int m_aBall[MAX_BALL];
  BallGroup * p_Ball[MAX_BALL];
  Score * p_Score;
  string m_sTableName;

  // Keep the high scores for the current table
  multimap<int, string> m_mapHighScores;
};
#endif // TABLE_H
//EOF $Id: Table.h,v 1.9 2003/06/13 13:39:48 rzr Exp $
