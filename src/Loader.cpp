//#ident "$Id: Loader.cpp,v 1.34 2003/07/16 20:02:04 rzr Exp $"
/***************************************************************************
                            Loader.cpp -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
***************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include "Private.h"
#include "Config.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

#include "Loader.h"
#include "Pinball.h"
#include "Keyboard.h"
#include "Menu.h"
#include "Engine.h"
#include "ArmBehavior.h"
#include "CollisionBounds.h"
#include "Camera.h"
#include "Light.h"
#include "Cube.h"
#include "BigSphere.h"
#include "Cylinder.h"
#include "BounceBehavior.h"
#include "KeyBehavior.h"
#include "Grid.h"
#include "KeyRotBehavior.h"
#include "Polygon.h"
#include "BumperBehavior.h"
#include "TextureUtil.h"
#include "SoundUtil.h"
#include "Score.h"
#include "StdAnimation.h"
#include "StateBehavior.h"
#include "Script.h"
#include "FakeModuleBehavior.h"
#include "PlungerBehavior.h"
#include "LoaderModule.h"
#ifdef RZR_PATCHES_3DS
#include "Obj3dsUtil.h" 
#endif

#define EmReadCmp(file_, ist_, str_, cmp_) \
 this->readNextToken(file_, ist_, str_); \
 if (str_ != cmp_) throw string("Parse error, unexpected token \'") + str_ + \
 string("\' expecting \'") + cmp_ + string("\'");

/******************************************************* 
 * Singleton suff 
 ******************************************************/
Loader * Loader::p_Loader = NULL;

Loader::Loader() {
  m_iNextSignal = LOADER_FIRSTSIGNAL;
  m_iNextVariable = LOADER_FIRSTVARIABLE;
  m_bModules = true;
  m_LoaderModule = 0;
#ifdef RZR_PATCHES_3DS
  m_Obj3dsUtil = 0;
#endif
}

Loader::~Loader() {
  delete m_LoaderModule;
#ifdef RZR_PATCHES_3DS
  delete m_Obj3dsUtil;
#endif
};

Loader * Loader::getInstance() {
  if (p_Loader == NULL) {
    p_Loader = new Loader();
  }
  return p_Loader;
}


/******************************************************* 
 * Reader functions, this wierd things is done to 
 * enable line counting
 ******************************************************/
void Loader::readNextToken(ifstream & file, istringstream & ist, string & str) {
  ist >> str;
  while(file && !ist) {
    string buf;
    getline(file, buf);
    ist.clear();
    ist.str(buf);
    ist >> str;
    ++m_iLineNumber;
  }
}

void Loader::readNextToken(ifstream & file, istringstream & ist, int & i) {
  ist >> i;
  while(file && !ist) {
    string buf;
    getline(file, buf);
    ist.clear();
    ist.str(buf);
    ist >> i;
    ++m_iLineNumber;
  }
}

void Loader::readNextToken(ifstream & file, istringstream & ist, float & f) {
  ist >> f;
  while(file && !ist) {
    string buf;
    getline(file, buf);
    ist.clear();
    ist.str(buf);
    ist >> f;
    ++m_iLineNumber;
  }
}

/******************************************************* 
 * Signal and variable to string conv 
 ******************************************************/
int Loader::getSignal(const char * signal) {
  // some defualt signals
  if (signal == NULL) return PBL_SIG_NULL;
  if (strncmp(signal, "null", 63) == 0) return PBL_SIG_NULL;
  if (strncmp(signal, "reset", 63) == 0) return PBL_SIG_RESET_ALL;
  if (strncmp(signal, "tilt", 63) == 0) return PBL_SIG_TILT;
  if (strncmp(signal, "game_over", 63) == 0) return PBL_SIG_GAME_OVER;
  if (strncmp(signal, "game_start", 63) == 0) return PBL_SIG_GAME_START;
  if (strncmp(signal, "game_pause", 63) == 0) return PBL_SIG_GAME_PAUSE;
  // other signals
  map<string, int>::iterator element = m_hSignalInt.find(string(signal));
  if (element != m_hSignalInt.end()) {
    return (*element).second;
  } else {
    //cerr << "*** insert signal " << signal << " " << m_iNextSignal << endl;
    m_hSignalInt.insert(pair<string, int>(string(signal), m_iNextSignal));
    m_hSignalString.insert(pair<int, string>(m_iNextSignal, string(signal)));
    ++m_iNextSignal;
    return m_iNextSignal-1;
  }	
}

int Loader::getVariable(const char * var) {
  if (var == NULL) return -1;
  map<string, int>::iterator element = m_hVariableInt.find(string(var));
  if (element != m_hVariableInt.end()) {
    return (*element).second;
  } else {
    //cerr << "*** insert variable " << var << " " << m_iNextVariable << endl;
    m_hVariableInt.insert(pair<string, int>(string(var), m_iNextVariable));
    m_hVariableString.insert(pair<int, string>(m_iNextVariable, string(var)));
    ++m_iNextVariable;
    return m_iNextVariable-1;
  }	
}

const char * Loader::getSignal(int signal) {
  // default signals
  if (signal < 0) return "null";
  switch (signal) {
  case PBL_SIG_NULL : return "null";
  case PBL_SIG_RESET_ALL : return "reset";
  case PBL_SIG_TILT : return "tilt";
  case PBL_SIG_GAME_OVER : return "game_over";
  case PBL_SIG_GAME_START : return "game_start";
  case PBL_SIG_GAME_PAUSE : return "game_pause";
  }
  // other signals
  map<int, string>::iterator element = m_hSignalString.find(signal);
  if (element != m_hSignalString.end()) {
    return (*element).second.c_str();
  } else {
    return NULL;
  }
}

const char * Loader::getVariable(int var) {
  map<int, string>::iterator element = m_hVariableString.find(var);
  if (element != m_hVariableString.end()) {
    return (*element).second.c_str();
  } else {
    return NULL;
  }
}

/* This on must be called each time we load a new level */
void Loader::clearSignalVariable() {
  m_iNextSignal = LOADER_FIRSTSIGNAL;
  m_iNextVariable = LOADER_FIRSTVARIABLE;
  m_hSignalInt.clear();
  m_hSignalString.clear();
  m_hVariableInt.clear();
  m_hVariableString.clear();
};

/******************************************************* 
 * Set properties for a group
 ******************************************************/

void Loader::loadProperties(ifstream & file, istringstream & ist, Group * group) {
  EM_COUT("Loader::loadProperties", 0);
  
  string str;
  
  EmReadCmp(file, ist, str, "{");
  
  this->readNextToken(file, ist, str);
  
  while (str != "}") {
    if (str == "transform_once") {
      group->setProperty(EM_GROUP_TRANSFORM_ONCE);
      EM_COUT("Loader::loadProperties - transform once", 0);
      
    } else if (str == "no_light") {
      group->setProperty(EM_GROUP_NO_LIGHT);
      EM_COUT("Loader::loadProperties - no light", 0);
      
    } else if (str == "light_once") {
      group->setProperty(EM_GROUP_LIGHT_ONCE);
      EM_COUT("Loader::loadProperties - light once", 0);
      
      // TODO we don't need this
    } else if (str == "no_signal") {
      group->setProperty(EM_GROUP_NO_SIGNAL);
      EM_COUT("Loader::loadProperties - no signal - deprecated", 0);
      
    } else if (str == "wall") {
      group->setUserProperty(PBL_WALLS);
      EM_COUT("Loader::loadProperties - wall", 0);
      
    } else if (str == "wall_one_way") {
      group->setUserProperty(PBL_WALLS_ONE);
      EM_COUT("Loader::loadProperties - wall_one_way", 0);
      
    } else if (str == "group_1") {
      //group->setUserProperty(PBL_GROUP1);
      group->setCollisionGroup(1);
      EM_COUT("Loader::loadProperties - group_1", 0);
      
    } else if (str == "alpha_test") {
      for (int a=0; a<group->getShape3DSize(); a++) {
	group->getShape3D(a)->setProperty(EM_SHAPE3D_ALPHATEST);
      }
      EM_COUT("Loader::loadProperties - alpha test", 0);
      
      /* I don't think we need this
	 } else if (str == "transparent") {
	 for (int a=0; a<group->getShape3DSize(); a++) {
	 Shape3D* shape = group->getShape3D(a);
	 shape->setProperty(EM_SHAPE3D_USE_TRANS);
	 vector<Polygon3D*>::iterator polyIter = shape->m_vPolygon.begin();
	 vector<Polygon3D*>::iterator polyEnd = shape->m_vPolygon.end();
	 for (; polyIter != polyEnd; polyIter++) {
	 (*polyIter)->setProperty(EM_POLY_TRANS);
	 }
	 }
	 EM_COUT("Loader::loadProperties - transparent", 0);
      */
    } else if (str == "collision") {
      Shape3D * shape = group->getShape3D(0);
      if (shape == NULL) {
	EM_COUT("Loader::loadProperties - no shape", 0); 
      } else {
	CollisionBounds* bounds = new CollisionBounds(shape->getCollisionSize());
	if (shape->getCollisionSize() > 16) {
	  bounds->setShape3D(shape, 4); //4
	} else if (shape->getCollisionSize() > 8) {
	  bounds->setShape3D(shape, 3); //4
	} else if (shape->getCollisionSize() > 4) {
	  bounds->setShape3D(shape, 2); //4
	} else if (shape->getCollisionSize() > 2) {
	  bounds->setShape3D(shape, 1); //4
	} else {
	  bounds->setShape3D(shape, 1);
	}				
	group->setCollisionBounds(bounds);
      }
      EM_COUT("Loader::loadProperties - collision", 0);
      
    } else {
      throw string("--UNKNOWN property in property block");
    }
    this->readNextToken(file, ist, str);
  }
}

/**************************************************************
 ** Behaviors
 **************************************************************/

void Loader::loadArmBehavior(ifstream & file, istringstream & ist, Group * group) {
  EM_COUT("Loader::loadArmBehavior", 0);

  string str;
  ArmBehavior* beh = NULL;
	
  EmReadCmp(file, ist, str, "{");
	
  this->readNextToken(file, ist, str);
  if (str == "right") {
    beh = new ArmBehavior(true);
  } else {
    beh = new ArmBehavior(false);
  }
  group->setBehavior(beh);	

  this->readNextToken(file, ist, str);
  if (str == "sound") {
    string soundname;
    this->readNextToken(file, ist, soundname);
    string filename = string(Config::getInstance()->getDataSubDir()) + "/" + soundname; 
    int sound = SoundUtil::getInstance()->loadSample(filename.c_str());
    beh->setSound(sound);
  } else if (str == "no_sound"){
  } else {
    throw string("No sound field in ArmBehavior");
  }

  EmReadCmp(file, ist, str, "}");
}

void Loader::loadAnimation(ifstream & file, istringstream & ist, Engine * e, 
                           Group * group, Behavior * beh) {
  EM_COUT("Loader::loadAnimation", 0);

  string str;

  EmAssert(beh != NULL, "Behavior NULL in loadAnimation");
  EmReadCmp(file, ist, str, "{");

  int type = EM_LIGHT;
  this->readNextToken(file, ist, str);
  if (str == "rotation") {
    type = EM_ROTATION;
  } else if (str == "translation") {
    type = EM_TRANSLATION;
  } else if (str == "light") {
    type = EM_LIGHT;
  } else {
    throw string("Expecting rotation, translation or light in anim field");
  }
  int count, step;
  this->readNextToken(file, ist, step);
  this->readNextToken(file, ist, count);
  StdAnimation* anim = new StdAnimation(step, type);

  for (; count > 0; count--) {
    float a, b, c;
    this->readNextToken(file, ist, a); 
    this->readNextToken(file, ist, b); 
    this->readNextToken(file, ist, c);
    cerr << "****** " << a <<" "<< b <<" "<< c << endl;
    if (count == 1) {
      anim->setEnd(a, b, c);
    } else {
      anim->add(a, b, c);
    }
  }
  group->setBehavior(anim);

  EmReadCmp(file, ist, str, "}");
}

void Loader::loadBehaviorLight(ifstream & file, istringstream & ist, 
                               Engine * engine, Group * group, Behavior * beh) {
  EM_COUT("Loader::loadBehaviorLight", 0);

  string str;
	
  EmAssert(beh != NULL, "Behavior NULL in loadBehaviorLight");
  EmReadCmp(file, ist, str, "{");

  float r, g, b, x, y, z;
  this->readNextToken(file, ist, r); 
  this->readNextToken(file, ist, g); 
  this->readNextToken(file, ist, b);
  this->readNextToken(file, ist, x); 
  this->readNextToken(file, ist, y); 
  this->readNextToken(file, ist, z);

  Light * light = new Light(1.0f, 0.0f, 0.1f, r, g, b);
  light->setOn(false);
  light->setProperty(EM_IGNORE_ANGLE_FULL);
  //light->setProperty(EM_IGNORE_DISTANCE);
  light->setProperty(EM_USE_BOUNDS);
  light->setBounds(10.0);
	
  Group * gl = new Group();
  gl->setName("#light");
  gl->addTranslation(x, y, z);
  gl->setLight(light);
  group->add(gl);
  engine->addLight(light);
	
  beh->setLight(light);

  this->loadMisc(file, ist, engine, gl, beh);
}

void Loader::loadBumperBehavior(ifstream & file, istringstream & ist, Engine * engine, Group * group) {
  EM_COUT("Loader::loadBumperBehavior", 0);
  
  string str;
  
  EmReadCmp(file, ist, str, "{");
  
  BumperBehavior* beh = new BumperBehavior();
  group->setBehavior(beh);
  
  if (this->cmpVersion(m_FileVersion, 0, 3, 0) >= 0) { // 0.3.0 and above
    float power;
    this->readNextToken(file, ist, power); 
    beh->setPower(power);
  }

  this->readNextToken(file, ist, str);
  if (str == "sound") {
    string soundname;
    this->readNextToken(file, ist, soundname);
    string filename = string(Config::getInstance()->getDataSubDir()) + "/" + soundname; 
    int sound = SoundUtil::getInstance()->loadSample(filename.c_str());
    beh->setSound(sound);
  } else if (str == "no_sound"){
  } else {
    throw string("No sound field in BumperBehavior");
  }
  group->setUserProperty(PBL_BUMPER);
  
  this->loadMisc(file, ist, engine, group, beh);
}

void Loader::loadPlungerBehavior(ifstream & file, istringstream & ist, Engine * engine, Group * group) {
  EM_COUT("Loader::loadPlungerBehavior", 0);
  
  string str;
  
  EmReadCmp(file, ist, str, "{");
  
  PlungerBehavior* beh = new PlungerBehavior();
  group->setBehavior(beh);
  
  this->readNextToken(file, ist, str);
  if (str == "sound") {
    string soundname;
    this->readNextToken(file, ist, soundname);
    string filename = string(Config::getInstance()->getDataSubDir()) + "/" + soundname; 
    int sound = SoundUtil::getInstance()->loadSample(filename.c_str());
    beh->setSound(sound);
  } else if (str == "no_sound"){
  } else {
    throw string("No sound field in PlungerBehavior");
  }
  group->setUserProperty(PBL_PLUNGER);
  
  this->loadMisc(file, ist, engine, group, beh);
}

void Loader::loadStateItem(ifstream & file, istringstream & ist, Engine * engine, Group * group, Behavior * beh) {
  EM_COUT("Loader::loadStateItem", 0);
  
  string str;
  
  EmAssert(beh != NULL, "Behavior NULL in loadBehaviorLight");
  EmAssert(beh->getType() == PBL_TYPE_STATEBEH, "Not StateBehavior");
  
  EmReadCmp(file, ist, str, "{");
  
  string asig;
  string csig;
  int cstate, delay, dstate;
  this->readNextToken(file, ist, asig); 
  this->readNextToken(file, ist, csig);
  this->readNextToken(file, ist, cstate);
  this->readNextToken(file, ist, dstate);
  this->readNextToken(file, ist, delay);
  StateItem* stateitem = new StateItem();
  stateitem->setActSig(this->getSignal(asig.c_str()));
  stateitem->setCollSig(this->getSignal(csig.c_str()));
  stateitem->setCollState(cstate);
  stateitem->setDelayState(dstate, delay);
  ((StateBehavior*)beh)->addStateItem(stateitem);
  
  this->readNextToken(file, ist, str);
  if (str == "move") {
    ((StateBehavior*)beh)->useMove(true);
    float tx, ty, tz, rx, ry, rz;
    int steps;
    this->readNextToken(file, ist, steps);
    this->readNextToken(file, ist, tx); 
    this->readNextToken(file, ist, ty); 
    this->readNextToken(file, ist, tz);
    this->readNextToken(file, ist, rx); 
    this->readNextToken(file, ist, ry); 
    this->readNextToken(file, ist, rz);
    stateitem->setMoveSteps(steps);
    stateitem->setTr(tx, ty, tz);
    stateitem->setRot(rx, ry, rz);
  } else if (str == "no_move") {
  } else {
    throw string("No move field in StateItem");
  }
  
  this->readNextToken(file, ist, str);
  if (str == "light") {
    stateitem->setLight(true);
  } else if (str == "no_light"){
    stateitem->setLight(false);
  } else {
    throw string("No light field in StateItem");
  }
  
  this->readNextToken(file, ist, str);
  if (str == "sound") {
    string soundname;
    this->readNextToken(file, ist, soundname);
    string filename = string(Config::getInstance()->getDataSubDir()) + "/" + soundname; 
    int sound = SoundUtil::getInstance()->loadSample(filename.c_str());
    stateitem->setSound(sound);
  } else if (str == "no_sound"){
  } else {
    throw string("No sound field in StateItem");
  }
  
  this->readNextToken(file, ist, str);
  if (str == "music") {
    string musicname;
    this->readNextToken(file, ist, musicname);
    string filename = string(Config::getInstance()->getDataSubDir()) + "/" + musicname; 
    int music = SoundUtil::getInstance()->loadMusic(filename.c_str());
    stateitem->setMusic(music);
  } else if (str == "no_music"){
  } else {
    throw string("No music field in StateItem");
  }
  
  // version hack
  if (this->cmpVersion(m_FileVersion, 0, 2 ,1) < 0) { // 0.2.0
    this->readNextToken(file, ist, str);
    if (str == "property") {
      int p;
      this->readNextToken(file, ist, p);
      stateitem->setUserProperty(p);
    } else if (str == "no_property") {
    } else {
      throw string("No user property field in StateItem"); 
    }
  } else { // 0.2.1 and above
    this->readNextToken(file, ist, str);
    if (str == "user_property") {
      int p;
      this->readNextToken(file, ist, p);
      stateitem->setUserProperty(p);
    } else if (str == "no_user_property") {
    } else {
      throw string("No user property field in StateItem");
    }
    
    this->readNextToken(file, ist, str);
    if (str == "shape_property") {
      int p;
      this->readNextToken(file, ist, p);
      stateitem->setShapeProperty(p);
    } else if (str == "no_shape_property") {
    } else {
      throw string("No shape property field in StateItem");
    }
  }
  
  this->readNextToken(file, ist, str);
  if (str == "texcoord") {
    ((StateBehavior*)beh)->useTexCoord(true);
    int count;
    this->readNextToken(file, ist, count);
    for (int a=0; a<count; ++a) {
      float u, v;
      this->readNextToken(file, ist, u); 
      this->readNextToken(file, ist, v);
      stateitem->addTexCoord(u, v);
    }
  } else if (str == "no_texcoord") {
    ((StateBehavior*)beh)->useTexCoord(false);
  } else {
    throw string("No texcoord field in StateItem");
  }
  // shape
  this->readNextToken(file, ist, str);
  if (str == "shape") {
    ((StateBehavior*)beh)->useShape(true);
    int count;
    this->readNextToken(file, ist, count);
    for (int a=0; a<count; ++a) {
      int s;
      this->readNextToken(file, ist, s);
      if (s == 1) {
	stateitem->addShapeEnable(a, true);
      } else {
	stateitem->addShapeEnable(a, false);
      }
    }
  } else if (str == "no_shape") {
    ((StateBehavior*)beh)->useShape(false);
  } else {
    throw string("No shape field in StateItem");
  }
	
  this->loadMisc(file, ist, engine, group, beh);
}

void Loader::loadStateBehavior(ifstream & file, istringstream & ist, Engine * engine, Group * group) {
  EM_COUT("state", 0);
	
  string str;

  EmReadCmp(file, ist, str, "{");
		
  StateBehavior* b = new StateBehavior();
  group->setBehavior(b);

  this->loadMisc(file, ist, engine, group, b);
}

/****************************************************************
 ** Script loading - deprecated
 ****************************************************************/

void Loader::loadScript(ifstream & file, istringstream & ist, Engine *, Group * group) {
  EM_COUT("Loader::loadScript", 0);
  EmAssert(group != NULL, "Group NULL in loadMisc");

  string str;
  this->clearSignalVariable();
  Script * script = new Script();

  EmReadCmp(file, ist, str, "{");

  this->readNextToken(file, ist, str);
  while (str != "}") {
    QueryItem * qi = new QueryItem();
    // read query
    if (str == "onsignal") {
      qi->setQuery(EM_SCRIPT_ONSIGNAL);
      int isig;
      this->readNextToken(file, ist, isig);
      for (; isig > 0; --isig) {
	int sig;
	this->readNextToken(file, ist, sig);
	qi->addQueryParm(sig);
      }
    } else {
      throw string("UNKNOWN in script query block ") + str;
    }
    // read action
    this->readNextToken(file, ist, str);
    if (str == "sendsignal") {
      qi->setAction(EM_SCRIPT_SENDSIGNAL);
      int sig, delay;
      this->readNextToken(file, ist, sig);
      this->readNextToken(file, ist, delay);
      qi->addActionParm(sig);
      qi->addActionParm(delay);
    } else if (str == "setvar") {
      qi->setAction(EM_SCRIPT_SETVAR);
      int id, val;
      this->readNextToken(file, ist, id);
      this->readNextToken(file, ist, val);
      qi->addActionParm(id);
      qi->addActionParm(val);
    } else {
      throw string("UNKNOWN in script action block ") + str;
    }
    // done
    script->addQueryItem(qi);
    this->readNextToken(file, ist, str);
  }
  group->setBehavior(script);
}

/****************************************************************
 ** Module (plugin) loading
 ****************************************************************/

void Loader::loadModule(ifstream & file, istringstream & ist, Engine *, Group * group) {
  EM_COUT("+Loader::loadModule", 1);
  EmAssert(group != NULL, "Group NULL in loadMisc");
  m_LoaderModule = LoaderModule::getInstance();
  string str;
  
  EmReadCmp(file, ist, str, "{");
  this->readNextToken(file, ist, str);
  //string filename = string(Config::getInstance()->getDataSubDir()) + "/" + str;
  string filename = string(EM_LIBDIR) + "/" + str;
  Behavior *beh=0;
  if (m_bModules) {
    beh = m_LoaderModule->read(filename); 
    if (beh == NULL) {
      throw string("Could not allocate behavior object");
    } else {
      group->setBehavior(beh);
    }
  } else {
    group->setBehavior(new FakeModuleBehavior(filename.c_str()));
  }
  EmReadCmp(file, ist, str, "}");
  EM_COUT("-Loader::loadModule", 1);
}

/****************************************************************
 ** Top level loading
 ****************************************************************/

int Loader::cmpVersion(const FileVersion & version, const int major, const int minor, const int micro) const {
  if (version.major > major) return 1;
  else if (version.minor < major) return -1;
  else {
    if (version.minor > minor) return 1;
    else if (version.minor < minor) return -1;
    else {
      if (version.micro > micro) return 1;
      else if (version.micro < micro) return -1;
    }
  }
  return 0;
}

/* Things added to objects, e.g. lights, animation, behavior*/
void Loader::loadMisc(ifstream & file, istringstream & ist, Engine * engine, Group * group, Behavior * beh) {
  EM_COUT("Loader::loadMisc", 0);
  EmAssert(engine != NULL && group != NULL, "Engine or group NULL in loadMisc");

  string str;

  this->readNextToken(file, ist, str);
  while (str != "}") {
    if (str == "properties"){
      this->loadProperties(file, ist, group);
    } else if (str == "arm_behavior") {
      this->loadArmBehavior(file, ist, group);
    } else if (str == "state_behavior") {
      this->loadStateBehavior(file, ist, engine, group);
    } else if (str == "bumper_behavior") {
      this->loadBumperBehavior(file, ist, engine, group);
    } else if (str == "plunger_behavior") {
      this->loadPlungerBehavior(file, ist, engine, group);
    } else if (str == "light") {
      this->loadBehaviorLight(file, ist, engine, group, beh);
    } else if (str == "state_item") {
      this->loadStateItem(file, ist, engine, group, beh);
    } else if (str == "animation") {
      this->loadAnimation(file, ist, engine, group, beh);
    } else if (str == "shape") {
      this->loadShape(file, ist, engine, group, beh);
    } else if (str == "script") {
      this->loadScript(file, ist, engine, group);
    } else if (str == "module") {
      this->loadModule(file, ist, engine, group);
    } else if (str == "sub_object") {
      Group * g = this->loadStdObject(file, ist, engine);
      group->add(g);

#ifdef RZR_PATCHES_3DS // version 0.3.0 and above 
    } else if (str == "shape_include_3ds_ascii") {
      this->loadShape3dsAscii(file, ist, engine, group, beh);
    } else if (str == "group_include_3ds_ascii") {
      this->loadGroup3dsAscii(file, ist, engine, group, beh);
#endif //ok but I'd like set as option unless it get fully usable (bugless too)
    } else {
      cerr << str << endl;
      throw string("UNKNOWN in misc block");
    }
    this->readNextToken(file, ist, str);
  }
}

/* Top level object */
Group * Loader::loadStdObject(ifstream & file, istringstream & ist, Engine * engine) {
  string str;

  this->readNextToken(file, ist, str);
  Group * group = new Group();
  group->setName(str.c_str());
  EM_COUT("Loader::loadStdObject " << str, 0);

  EmReadCmp(file, ist, str, "{");

  float tx, ty, tz, rx, ry, rz, sx, sy, sz;
  this->readNextToken(file, ist, tx); 
  this->readNextToken(file, ist, ty); 
  this->readNextToken(file, ist, tz);
  this->readNextToken(file, ist, rx); 
  this->readNextToken(file, ist, ry);	
  this->readNextToken(file, ist, rz);
  if (this->cmpVersion(m_FileVersion, 0, 3, 0) >= 0) { // 0.3.0 and above
    this->readNextToken(file, ist, sx); 
    this->readNextToken(file, ist, sy);	
    this->readNextToken(file, ist, sz);
  } else { // below 0.3.0
    sx = 1.0f;
    sy = 1.0f;
    sz = 1.0f;
  }
	
  group->setTransform(tx, ty, tz, rx, ry, rz);
  group->setScale(sx, sy, sz);

  this->loadMisc(file, ist, engine, group, NULL);
  return group;
}

/****************************************************************
 ** Shape specific loads
 ****************************************************************/

void Loader::loadShape(ifstream & file, istringstream & ist, Engine *, 
                       Group * group, Behavior *) {
  EM_COUT("Loader::loadShape", 0);
  
  string str;
  
  Shape3D * s = this->loadShape3DChunk(file, ist);
  if (s != NULL) {
    group->addShape3D(s); 
  } else {
    cerr << "could not load shape" << endl;
  }
}

void Loader::readUnknown(ifstream & file, istringstream & ist) {
  EM_COUT("Loader::readUnknown", 0);
  string str;
  EmReadCmp(file, ist, str, "{");

  int brackets = 1;
  while (brackets > 0) {
    this->readNextToken(file, ist, str);
    if (str == "{") brackets++;
    if (str == "}") brackets--;
  }
}

void Loader::readPolygon(ifstream & file, istringstream & ist, Shape3D* shape) {
  EM_COUT("Loader::readPolygon", 0);
  string str;
  Polygon3D * poly = new Polygon3D(shape);

  EmReadCmp(file, ist, str, "{");

  this->readNextToken(file, ist, str);
  while (str != "}") {
    if (str == "pes") {
      // TODO reserce space for 
      this->readUnknown(file, ist);
    } else if (str == "tpt") {
      this->readUnknown(file, ist);
      poly->setProperty(EM_POLY_TRANS);
      shape->setProperty(EM_SHAPE3D_USE_TRANS);
      // 		} else if (str == "dbl") {
      // 			this->readUnknown(file, ist);
      // 			poly->setProperty(EM_POLY_DOUBLE);
      // 		} else if (str == "flt") {
      // 			this->readUnknown(file, ist);
      // 			poly->setProperty(EM_POLY_FLAT);
    } else if (str == "ple") {
      this->readPolygonEdge(file, ist, poly);
    } else {
      this->readUnknown(file, ist);
    }
    this->readNextToken(file, ist, str);
  }
  shape->add(poly);
}

void Loader::readPolygonEdge(ifstream & file, istringstream & ist, Polygon3D* poly) {
  EM_COUT("Loader::readPolygonEdge", 0);
  string str;
  int i;

  EmReadCmp(file, ist, str, "{");

  this->readNextToken(file, ist, i);
  poly->add(i);

  EmReadCmp(file, ist, str, "}");

}

void Loader::readVertex(ifstream & file, istringstream & ist, Shape3D* shape) {
  EM_COUT("Loader::readVertex", 0);
  float x, y, z, r, g, b, a, u, v;
  string str;

  EmReadCmp(file, ist, str, "{");

  this->readNextToken(file, ist, x);
  this->readNextToken(file, ist, y);
  this->readNextToken(file, ist, z);
  this->readNextToken(file, ist, r);
  this->readNextToken(file, ist, g);
  this->readNextToken(file, ist, b);
  this->readNextToken(file, ist, a);
  this->readNextToken(file, ist, u);
  this->readNextToken(file, ist, v);

  EmReadCmp(file, ist, str, "}");

  shape->add(x, y, z, r, g, b, a, u, v);
}

void Loader::readTexture(ifstream & file, istringstream & ist, Shape3D* shape) {
  EM_COUT("Loader::readTexture", 0);
  string str;

  EmReadCmp(file, ist, str, "{");
  this->readNextToken(file, ist, str);

  // hack to load pcx files instead of png files for allegro
#if EM_USE_ALLEGRO
  str += ".pcx";
#endif	

  string filename = string(Config::getInstance()->getDataSubDir()) + "/" + str;
  EmTexture * tex = TextureUtil::getInstance()->loadTexture(filename.c_str());
  if (tex != NULL) {
    shape->setTexture(tex);
  } else {
    throw string("Loader::readTexture error loading: ") + filename;
  }

  EmReadCmp(file, ist, str, "}");
}

/* Assumes that the word 'shape' is already read and that we wish read stuff
 * between { and } */
Shape3D* Loader::loadShape3DChunk(ifstream & file, istringstream & ist) {
  EM_COUT("Loader::loadShape3DChunk loading shape", 0);

  Shape3D* shape = new Shape3D();

  string str;
  EmReadCmp(file, ist, str, "{");

  this->readNextToken(file, ist, str);
  while (str != "}") {
    if (str == "vtx") {
      this->readVertex(file, ist, shape);
    } else if (str == "ply") {
      this->readPolygon(file, ist, shape);
    } else if (str == "tex") {
      this->readTexture(file, ist, shape);
    } else if (str == "hid") {
      shape->setProperty(EM_SHAPE3D_HIDDEN);
      this->readUnknown(file, ist);
    } else if (str == "bhi") {
      shape->setProperty(EM_SHAPE3D_BEHIND);
      this->readUnknown(file, ist);
    } else if (str == "bh2") {
      shape->setProperty(EM_SHAPE3D_BEHIND2);
      this->readUnknown(file, ist);
    } else if (str == "lit") {
      shape->setProperty(EM_SHAPE3D_ALWAYSLIT);
      this->readUnknown(file, ist);
    } else {
      this->readUnknown(file, ist);
    }
    this->readNextToken(file, ist, str);
  }
  shape->countNormals();
  return shape;
}

/****************************************************************
 ** The main loader function
 ****************************************************************/

int Loader::loadFile(const char* fn, Engine * engine) {
  ifstream file(fn);
  try {
    if (!file) {
      throw string("Loader::loadFile file not found: ") + string(fn);
    }
    if (engine == NULL) {
      throw string("Engine NULL");
    }
    string str("");
    istringstream ist("");
    m_iLineNumber = 0;
    m_FileVersion.major = 0;
    m_FileVersion.minor = 2;
    m_FileVersion.micro = 0;
    this->readNextToken(file, ist, str);
    if (str == "version") {
      EmReadCmp(file, ist, str, "{");
      this->readNextToken(file, ist, m_FileVersion.major);
      this->readNextToken(file, ist, m_FileVersion.minor);
      this->readNextToken(file, ist, m_FileVersion.micro);
      EmReadCmp(file, ist, str, "}");
      if (this->cmpVersion(m_FileVersion, 0, 3, 0) > 0) {
        cerr << "WARNING! Version above 0.3.0 not supported, file is " <<
          m_FileVersion.major <<" "<< m_FileVersion.minor <<" "<< m_FileVersion.micro << endl;
      }
    } else {
      cerr << "Version string not found assuming, 0.2.0" << endl;
    }
    while (file) {
      if (str == "object") {
        Group * group = this->loadStdObject(file, ist, engine); 
        engine->add(group);
      }
      this->readNextToken(file, ist, str);
      //cerr << str << endl;
    }
  } catch (string str) {
    cerr << "Loader::loadFile caught exception ************" << endl;
    cerr << str << endl;
    cerr << "When loading file: " << fn << endl;
    cerr << "At line: " << m_iLineNumber << endl;
    cerr << "Loader::loadFile *****************************" << endl;
    return -1;
  }
  return 0;
}

#ifdef RZR_PATCHES_3DS //-----------------------------------------------------

/// load all scene in one shape
void Loader::loadShape3dsAscii(ifstream & file, istringstream & ist, 
			       Engine *, Group * group, Behavior * b) 
{
  EM_COUT("Loader::loadShape3dsAscii", 1);
  string str;
  EmReadCmp(file, ist, str, "{");
  this->readNextToken(file, ist, str);
  Shape3D* shape = 0;
  string filename = string(Config::getInstance()->getDataSubDir()) + "/" + str;
  //EM_COUT( filename , 1);
  int t = 0;

  shape = new Shape3D;
  t =  Obj3dsUtil::read(  *shape , filename.c_str());
  if ( t < 0 || shape == 0 ) { delete shape; return;}
  
  filename = shape->m_sMaterialName; //debug("assign textures");
  if ( ( filename != "" ) //&& ( filename.find(".",0) ) // name.ext = file ?
       ) {
    filename = string(Config::getInstance()->getDataSubDir() )+ "/" +filename ;
    EmTexture * tex =
      TextureUtil::getInstance()->loadTexture(filename.c_str());
    if (tex != NULL) { 
      shape->setTexture(tex); 
      shape->setColor(1,1,1,0); //TODO: check if nedded
      //EM_COUT("loaded and asssigned :"<<filename,1);
    } 
  } // this section my be placed in a shape visitor

  shape->countNormals();
  group->addShape3D(shape);
  
  this->readNextToken(file, ist, str);
  while (str != "}") {
    this->readNextToken(file, ist, str);
  }
  //EM_COUT("- Loader::loadShape3dsAscii", 0);
}


void Loader::loadGroup3dsAscii(ifstream & file, istringstream & ist, 
			       Engine *, Group * group, Behavior * b) 
{
  EM_COUT("+ Loader::loadGroup3dsAscii", 1);
  string str;
  EmReadCmp(file, ist, str, "{");
  this->readNextToken(file, ist, str);
  Group* arg = new Group;
  Shape3D* shape = 0;
  string filename = string(Config::getInstance()->getDataSubDir()) + "/" + str;
  //EM_COUT( filename , 1);
  int t = 0;
  int i=0;
  t =  Obj3dsUtil::read(  *arg , filename.c_str());
  if ( t < 0 ) { delete arg; return;}
  EmReadCmp(file, ist, str, "}");

  //debug("copy to current group");
#if 1
  group->add(arg);
#else
  for(i=0;i<arg->getShape3DSize(); i++) {
    shape = arg->getShape3D(i);
    shape->countNormals();
    group->addShape3D( shape);
  }
  //TODO delete group
#endif

#if 0
  //debug("read aliases");
  /*
   material {
   overide "Material #1" { texture "file.png" }
   overide "Material #1" { color "rbga" }
   overide "*"  { texture "file2.png"  }
   }
  */
  string key,value;
  map<string, string> aliases;
  this->readNextToken(file, ist, str);
  if (str == "material") {
    EmReadCmp(file, ist, str, "{"); 
    this->readNextToken(file, ist, key); 
    this->readNextToken(file, ist, value); 
    EmReadCmp(file, ist, str, "}");
    //map.insert(key, value);
  }
  
  //this->readNextToken(file, ist, str);
#endif

  for(i=0;i<arg->getShape3DSize(); i++) {
    shape = arg->getShape3D(i);
  
  filename = shape->m_sMaterialName; //debug("assign textures");
  if ( ( filename != "" ) //&& ( filename.find('.',0) ) 
       ) {
    filename = string(Config::getInstance()->getDataSubDir()) + "/" +filename ;
    EmTexture * tex =
      TextureUtil::getInstance()->loadTexture(filename.c_str());
    if (tex != NULL) { 
      shape->setTexture(tex); 
      shape->setColor(1,1,1,0); //TODO: check if nedded
      //EM_COUT("loaded and asssigned :"<<filename,1);
    } 
  } // this section my be placed in a shape visitor

  }
    
#if 0 //collision part , need some help
  float size = group->getCollisionSize();
  debugf(". add collision_size=%f\n",size);
  CollisionBounds* bounds = new CollisionBounds(size);

  for(int i=0;i<group->getShape3DSize(); i++) {
    shape = arg->getShape3D(i);
    if ( size > 16) {
      bounds->setShape3D(shape, 4); //4
    } else if ( size > 8) {
      bounds->setShape3D(shape, 3); //4
    } else if ( size > 4) {
      bounds->setShape3D(shape, 2); //4
    } else if ( size > 2) {
      bounds->setShape3D(shape, 1); //4
    } else {
      bounds->setShape3D(shape, 1);
    }
  }
  //debugf(". group collision slow\n");
  group->setCollisionBounds(bounds);
#endif
  EM_COUT("- Loader::loadGroup3dsAscii", 1);
}
#endif //--------------------------------------------------------------------
//EOF $Id: Loader.cpp,v 1.34 2003/07/16 20:02:04 rzr Exp $
