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

void Table::activateBall(int ball) {
  if (ball < 0 || ball >= MAX_BALL) return;
  m_aBall[ball] = PBL_ACTIVE;
  if (p_Ball[ball] != NULL) {
    Behavior * beh = p_Ball[ball]->getBehavior();
    EmAssert(beh != NULL, "Table::activateBall behavior NULL");
    EmAssert(beh->getType() == PBL_TYPE_BOUNCEBEH, 
	     "Table::activateBall behavior not bouncebehavior");
    ((BounceBehavior*)beh)->activateBall();
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

// pnf
string Table::getTableName()
{
  return m_sTableName;
}

// pnf
string Table::getTableDataDirName()
{
  string datadir(Config::getInstance()->getDataSubDir());

  return datadir;
}
