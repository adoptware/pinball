//#ident "$Id: Table.cpp,v 1.17 2003/06/13 13:39:48 rzr Exp $"
/***************************************************************************
                            Table.cpp -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com

    modifs
    24-05-2003           : pnf - Solved problem while reading highscore
                                  table when a name with more then one
                                  word exists.

***************************************************************************/

#include "Private.h"
#include "Table.h"
#include "Engine.h"
#include "Config.h"
#include "Loader.h"
#include "BallGroup.h"
#include "Pinball.h"
#include "Score.h"
#include "Camera.h"
#include "Light.h"
#include "EyeBehavior.h"
#include "SoundUtil.h"
#include "BounceBehavior.h"

#include <iostream>
#include <fstream>

#include <cstdio>

/*******************************************************
 * Singleton suff
 ******************************************************/
Table * Table::p_Table = NULL;

Table::Table() {
  this->clear(NULL);
};

Table::~Table() {
  p_Table = NULL;
};

Table * Table::getInstance() {
  if (p_Table == NULL) {
    p_Table = new Table();
  }
  return p_Table;
}

BallGroup * Table::getBall(int ball) {
  if (ball < 0 || ball >= MAX_BALL) return NULL;
  return p_Ball[ball];
}

void Table::clear(Engine * engine) {
  if (engine != NULL) engine->clear();
  m_sTableName = string("");
  p_Score = NULL;
  m_iBall = 0;
  for (int a=0; a<MAX_BALL; ++a) {
    p_Ball[a] = NULL;
    m_aBall[a] = PBL_DEAD;
  }
}

void Table::activateBall(int ball, float x, float y, float z) {
  if (ball < 0 || ball >= MAX_BALL) return;
  m_aBall[ball] = PBL_ACTIVE;
  if (p_Ball[ball] != NULL) {
    Behavior * beh = p_Ball[ball]->getBehavior();
    EmAssert(beh != NULL, "Table::activateBall behavior NULL");
    EmAssert(beh->getType() == PBL_TYPE_BOUNCEBEH,
             "Table::activateBall behavior not bouncebehavior");
    ((BounceBehavior*)beh)->activateBall(x, y, z);
  }
}

void Table::unActivateBall(int ball) {
  if (ball < 0 || ball >= MAX_BALL) return;
  m_aBall[ball] = PBL_DEAD;
}

bool Table::isBallActive(int ball) {
  if (ball < 0 || ball >= MAX_BALL) return false;
  return (m_aBall[ball] == PBL_ACTIVE);
}

bool Table::isBallDead(int ball) {
  if (ball < 0 || ball >= MAX_BALL) return true;
  return (m_aBall[ball] == PBL_DEAD);
}

/** The bounce behavior calls this method when the ball is locked */
void Table::lockBall(int ball) {
  if (ball < 0 || ball >= MAX_BALL) return;
  m_aBall[ball] = PBL_LOCKED;
}

void Table::unLockBall(int ball) {
  if (ball < 0 || ball >= MAX_BALL) return;
  if (m_aBall[ball] == PBL_LOCKED) m_aBall[ball] = PBL_ACTIVE;
}

bool Table::isBallLocked(int ball) {
  if (ball < 0 || ball >= MAX_BALL) return false;
  return (m_aBall[ball] == PBL_LOCKED);
}

int Table::active() {
  int active = 0;
  for (int a=0; a<MAX_BALL; ++a) {
    if (m_aBall[a] == PBL_ACTIVE) ++active;
  }
  return active;
}

int Table::locked() {
  int locked = 0;
  for (int a=0; a<MAX_BALL; ++a) {
    if (m_aBall[a] == PBL_LOCKED) ++locked;
  }
  return locked;
}

// Called to load a new table
int Table::loadLevel(Engine * engine, const char * subdir) {
  // Clear old engine objects
  this->clear(engine);
  m_sTableName = string(subdir);

  // Load from file
  Config::getInstance()->setSubDir(subdir);
  string datadir(Config::getInstance()->getDataSubDir());
  string filename = datadir + string("/pinball.pbl");
  Loader::getInstance()->clearSignalVariable();
  if (Loader::getInstance()->loadFile(filename.c_str(), engine) != 0) {
    cerr << "Error loading level: " << m_sTableName << endl;
    cerr << "Try reinstalling the game"
         << endl;
    this->clear(engine);
    return -1;
  }
  p_Score = new Score();
  engine->setBehavior(p_Score);
  // Add a camera.
  Group* groupCR = new Group();
  Group* groupCT = new Group();
  Camera* camera = new Camera();
  engine->add(groupCT);
  groupCT->add(groupCR);
  groupCR->setCamera(camera);

  Light* cl = new Light(1, 0.05f, 0, 1,1,1);
  cl->unsetProperty(EM_USE_DIFFUSE);
  groupCR->setLight(cl);
  engine->addLight(cl);

  engine->setEngineCamera(groupCR);

  p_Ball[0] = new BallGroup(1, 0, 0,  0);
  p_Ball[1] = new BallGroup(0, 1, 0,  1);
  p_Ball[2] = new BallGroup(0, 0, 1,  2);
  engine->add(p_Ball[0]);
  engine->add(p_Ball[1]);
  engine->add(p_Ball[2]);

  EyeBehavior* eyebeh = new EyeBehavior();
  filename = datadir + string("/nudge.wav");
  eyebeh->setSound(SoundUtil::getInstance()->loadSample(filename.c_str()));
  groupCT->setBehavior(eyebeh);
  groupCT->setTransform(TX2, TY2, TZ2, RX2, RY2, RZ2);

  // Reset pinball
  SendSignal(PBL_SIG_RESET_ALL, 0, engine, NULL);

  return 0;
}

string Table::getTableName() const {
  return m_sTableName;
}

string Table::getTableDataDirName() const {
  string datadir(Config::getInstance()->getDataSubDir());
  return datadir;
}

bool Table::isItHighScore(const int nNewScore) const {
  // The first score is the lowest
  multimap<int, string>::const_iterator iter = m_mapHighScores.begin();
  int nScore = (*iter).first;
  if (nNewScore > nScore)  return true;
  return false;
}

bool Table::getHighScoresData(list<string>& listHighScores) const {
  // Sorted in ascending order, we begin by the last one and
  //  decrement iterator
  multimap<int, string>::const_reverse_iterator iter = m_mapHighScores.rbegin();
  multimap<int, string>::const_reverse_iterator end = m_mapHighScores.rend();
  for (int index=0;
       iter != end && index < EM_MAX_HIGHSCORES;
       ++iter, ++index) {
    char sScore[11];
    string sRow;
    int nScore = (*iter).first;
    string sName  = (*iter).second;
    // Max len of name is 12
    int nAdjust = 13 - sName.length();
    if (nAdjust > 0) {
      sName.append(nAdjust, ' ');
    }
    sprintf(sScore, "%10d", nScore);
    sRow = sName;
    sRow += sScore;
    listHighScores.push_back(sRow);
  }
  return true;
}

void Table::saveNewHighScore(int nNewHighScore, const char * name) {
  // Remove the first element, it's the lowest score,
  // a map is by always sorted.
  multimap<int, string>::iterator iter = m_mapHighScores.begin();

  // Make shure that the new score is at least bigger then the lowest score
  int nScore = (*iter).first;
  if (nNewHighScore > nScore)  {
    m_mapHighScores.erase(iter);
    m_mapHighScores.insert(pair<int, string>(nNewHighScore, name));
  }
}

#define HIGH_SCORES_FILENAME "highscores" //!rzr: removed /

bool Table::readHighScoresFile() {
  // This is the current table's name
  if (m_sTableName.length() == 0) {
    cerr << "No current table name!" << endl;
    return false;
  }
  // Clear old high scores
  m_mapHighScores.clear();

  char *home = getenv("HOME");
  string sFileName = string(home? home:".") + "/.config/emilia/" + m_sTableName +
    ".hiscore";

  ifstream file(sFileName.c_str());
  if (!file) {
    cerr << "Couldn't open high scores file: " << sFileName << endl;
    cerr << "Using default values!" << endl;
    for (int i=0; i<EM_MAX_HIGHSCORES; i++) {
      m_mapHighScores.insert(pair<int, string>(0, "emilia"));
    }
    return false;
  }

  const int BUF_SIZE = 255;
  char   sBuf[BUF_SIZE];
  char*  pSpacePos      = NULL;
  int    nScore         = 0;
  string sName;
  // Read all lines from file, this way we can append several
  //  highscores files (from backups or other) and after being
  //  sorted we'll only keep the top X scores
  while (file) {
    memset(sBuf, 0, BUF_SIZE);
    file.getline(sBuf, BUF_SIZE);
    pSpacePos = strchr(sBuf, ' ');
    if (pSpacePos != NULL) {
      sName  = "";
      nScore = 0;
      *pSpacePos = '\0';
      nScore = atoi(sBuf);
      sName = pSpacePos + 1;

      if (nScore == 0) {
        continue;
      }
      if (sName.length() == 0) {
        sName = "emilia";
      }
      m_mapHighScores.insert(pair<int, string>(nScore, sName));
    }
  }
  // If we read less then 10 scores create the rest with default values
  for (int i=m_mapHighScores.size(); i<EM_MAX_HIGHSCORES; i++) {
    m_mapHighScores.insert(pair<int, string>(0, "emilia"));
  }
  return true;
}

// The file must be writable by all users...
bool Table::writeHighScoresFile() {
  // This is the current table's name
  if (m_sTableName.length() == 0) {
    cerr << "No current table name! (the first time is normal...)" << endl;
    return false;
  }

  char *home = getenv("HOME");
  string sFileName = string(home? home:".") + "/.config/emilia/" + m_sTableName +
    ".hiscore";

  ofstream file(sFileName.c_str());//, ios_base::out | ios_base::trunc);
  if (!file) {
    cerr << "Couldn't open high scores file: " << sFileName << endl;
    cerr << "Can't save high scores!" <<  endl;
    return false;
  }

  multimap<int, string>::reverse_iterator iter = m_mapHighScores.rbegin();
  multimap<int, string>::reverse_iterator begin = m_mapHighScores.rend();
  for (int index=0;
       index < EM_MAX_HIGHSCORES && iter != begin;
       ++iter, ++index) {
    int nScore = (*iter).first;
    string sName  = (*iter).second;
    file << nScore << " " << sName << endl;
  }

  return true;
}
//EOF: $Id: Table.cpp,v 1.17 2003/06/13 13:39:48 rzr Exp $
