/***************************************************************************
                            Table.cpp -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
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
    cerr << "Try reinstalling the game or use the -data switch to specify the data directory" 
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

  // Reset pinball
  SendSignal(PBL_SIG_RESET_ALL, 0, engine, NULL);

  return 0;
}

string Table::getTableName()
{
  return m_sTableName;
}

string Table::getTableDataDirName()
{
  string datadir(Config::getInstance()->getDataSubDir());
  return datadir;
}

bool Table::isItHighScore(const int nNewScore)
{
  // The first score is the lowest
  multimap<int, string>::iterator it = m_mapHighScores.begin();

  int nScore = (*it).first;

  if (nNewScore > nScore)
    return true;

  return false;
}

bool Table::getHighScoresData(list<string>& listHighScores)
{
  int nScore = 0;
  string sName;

  char sScore[11];

  for (multimap<int, string>::iterator it = m_mapHighScores.begin();
       it != m_mapHighScores.end(); it++) {
    nScore = (*it).first;
    sName  = (*it).second;

    string sRow(25, ' ');

    sprintf(sScore, "%10d", nScore);

    sRow.replace(0, 10, sName);
    sRow.replace(12, 21, sScore);

    listHighScores.push_front(sRow);
  }

  return true;
}

void Table::saveNewHighScore(int nNewHighScore, const char * name)
{
  // Remove the first element, it's the lowest score,
  // a map is by always sorted.
  multimap<int, string>::iterator it = m_mapHighScores.begin();

  // Make shure that the new score is at least bigger then the lowest score
  int nScore = (*it).first;
  if (nNewHighScore > nScore)
  {
    m_mapHighScores.erase(it);

    m_mapHighScores.insert(pair<int, string>(nNewHighScore, name));
  }
}

#define HIGH_SCORES_FILENAME "/highscores"

bool Table::readHighScoresFile()
{
  // This is the current table's name
  if (m_sTableName.length() == 0)
  {
    cerr << "No current table name!" << endl;
    return false;
  }

  // Clear old high scores
  m_mapHighScores.clear();

  //string sFileName = string(Config::getInstance()->getDataSubDir()) +
		       HIGH_SCORES_FILENAME;

  string sFileName = string(EM_HIGHSCORE_DIR) +"/"+ m_sTableName +
		     HIGH_SCORES_FILENAME;
	
  ifstream file(sFileName.c_str());
  if (!file)
  {
    cerr << "Couldn't open high scores file: " << sFileName << endl;
    cerr << "Using default values!" << endl;
  
    for (int i=0; i<10; i++)
      m_mapHighScores.insert(pair<int, string>(0, "?"));

    return false;
  }

  cerr << "read HS file..." << endl;
	
  int nScore = 0;
  string sName;

  while (file)
  {
    file >> nScore;
    file >> sName;

    if (nScore == 0)
      continue;

    m_mapHighScores.insert(pair<int, string>(nScore, sName));

    // We only read 10 scores from the file!
    if (m_mapHighScores.size() >= 10)
      break;
  }

  // If we read less then 10 scores
  for (int i=m_mapHighScores.size(); i<10; i++)
    m_mapHighScores.insert(pair<int, string>(10 - i, "lia"));

  return true;
}

// NOTE!!!
// For now the high scores are saved in a file that is in directory
//  /usr/local/share/pinball/tux (for table tux, last dir is table's name)
// Problem: this file must be owned by user pinball with write access
//  to all (don't create a root file with write access to all...!).
// For now please create this file by hand if you want to save your scores:
// # su <- give root password
// # adduser pinball
// # cd /usr/local/share/pinball/tux
// # touch highscores
// # chown pinball:pinball highscores
// # chmod a+w highscores
// TODO: Find a way to safely write in a common file all high scores, this
//  method also should be FHS friendly...
//
//
// LAST CHANGES! Henrik changed this to dir EM_HIGHSCORE_DIR defined during
//                compilation...
//
bool Table::writeHighScoresFile()
{
  // This is the current table's name
  if (m_sTableName.length() == 0)
  {
    cerr << "No current table name! (the first time is normal...)" << endl;
    return false;
  }

  //string sFileName = string(Config::getInstance()->getDataSubDir()) +
		       HIGH_SCORES_FILENAME;

  string sFileName = string(EM_HIGHSCORE_DIR) +"/"+ m_sTableName +
		     HIGH_SCORES_FILENAME;

  ofstream file(sFileName.c_str());//, ios_base::out | ios_base::trunc);

  if (!file)
  {
    cerr << "Couldn't open high scores file: " << sFileName << endl;
    cerr << "Can't save high scores!" <<  endl;
    return false;
  }

  int nIndex = 1;
  int nScore = 0;

  string sName;

  for (multimap<int, string>::iterator it = m_mapHighScores.begin();
       it != m_mapHighScores.end(); it++)
  {
    nScore = (*it).first;
    sName  = (*it).second;

    file << nScore << " " << sName << endl;

    // We only write 10 scores to file! (for safety...)
    if (nIndex >= 10)
      break;

    nIndex++;
  }

  return true;
}
