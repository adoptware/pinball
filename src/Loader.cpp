/***************************************************************************
                            Loader.cpp -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
//#include <gmodule.h>
//#include <values.h>

#include <ltdl.h>

#include "Private.h"
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
#include "Config.h"
#include "StateBehavior.h"
#include "Script.h"
#include "FakeModuleBehavior.h"

#define EmReadCmp(file_, ist_, str_, cmp_) \
	this->readNextToken(file_, ist_, str_);               \
 	if (str_ != cmp_) throw string("Parse error, unexpected token \'") + str_ +  \
										string("\' expecting \'") + cmp_ + string("\'");

/******************************************************* 
 * Singleton suff 
 ******************************************************/
Loader * Loader::p_Loader = NULL;

Loader::Loader() {
	m_iNextSignal = LOADER_FIRSTSIGNAL;
	m_iNextVariable = LOADER_FIRSTVARIABLE;
	m_bModules = true;
	lt_dlinit();
};

Loader::~Loader() {
	lt_dlexit();
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
	if (strncmp(signal, "extraball", 63) == 0) return PBL_SIG_EXTRABALL;
	if (strncmp(signal, "multiball_off", 63) == 0) return PBL_SIG_MULTIBALL_OFF;
	if (strncmp(signal, "allballs_off", 63) == 0) return PBL_SIG_ALLBALLS_OFF;
	if (strncmp(signal, "lock1", 63) == 0) return PBL_SIG_LOCK_1;
	if (strncmp(signal, "lock2", 63) == 0) return PBL_SIG_LOCK_2;
	if (strncmp(signal, "lock3", 63) == 0) return PBL_SIG_LOCK_3;
	if (strncmp(signal, "lock4", 63) == 0) return PBL_SIG_LOCK_4;
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
	case PBL_SIG_EXTRABALL : return "extraball";
	case PBL_SIG_MULTIBALL_OFF : return "multiball_off";
	case PBL_SIG_ALLBALLS_OFF : return "allballs_off";
	case PBL_SIG_LOCK_1 : return "lock1";
	case PBL_SIG_LOCK_2 : return "lock2";
	case PBL_SIG_LOCK_3 : return "lock3";
	case PBL_SIG_LOCK_4 : return "lock4";
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
			group->setUserProperty(PBL_GROUP1);
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
				 vector<Polygon*>::iterator polyIter = shape->m_vPolygon.begin();
				 vector<Polygon*>::iterator polyEnd = shape->m_vPolygon.end();
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
	group->addBehavior(beh);	

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

void Loader::loadAnimation(ifstream & file, istringstream & ist, 
													 Engine * engine, Group * group, Behavior * beh) {
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
	}
	StdAnimation* anim = new StdAnimation(50, type);

	int count;
	this->readNextToken(file, ist, count);
	for (; count > 0; count--) {
		float a, b, c;
		this->readNextToken(file, ist, a); 
		this->readNextToken(file, ist, b); 
		this->readNextToken(file, ist, c);
		if (count == 1) {
			anim->setEnd(a, b, c);
		} else {
			anim->add(a, b, c);
		}
	}
	group->addBehavior(anim);

	this->loadMisc(file, ist, engine, group, anim);
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
	gl->setName("light");
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
	group->addBehavior(beh);

	int score;
	this->readNextToken(file, ist, score);
	beh->setScore(score);

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

	this->readNextToken(file, ist, str);
	if (str == "property") {
		int p;
		this->readNextToken(file, ist, p);
		stateitem->setProperty(p);
	} else if (str == "no_property") {
	} else {
		throw string("No property field in StateItem");
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
	group->addBehavior(b);

	this->loadMisc(file, ist, engine, group, b);
}

/****************************************************************
 ** Script loading
 ****************************************************************/

void Loader::loadScript(ifstream & file, istringstream & ist, Engine * engine, Group * group) {
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
	group->addBehavior(script);
}

/****************************************************************
 ** Module (plugin) loading
 ****************************************************************/

void Loader::loadModule(ifstream & file, istringstream & ist, Engine * engine, Group * group) {
	EM_COUT("Loader::loadModule", 1);
	EmAssert(group != NULL, "Group NULL in loadMisc");

	string str;

	EmReadCmp(file, ist, str, "{");
	this->readNextToken(file, ist, str);
	string filename = string(Config::getInstance()->getDataSubDir()) + "/" + str;

	if (m_bModules) {
		lt_dlhandle handle = lt_dlopen(filename.c_str());
		if (handle == NULL) {
			throw (string("Could not open shared library: ") + string(lt_dlerror())); 
		} else {
			lt_ptr fct_ptr = lt_dlsym(handle, "new_object_fct");
			if (fct_ptr == NULL) {
				throw (string("Could not find symbol 'new_object_fct' in library") + string(lt_dlerror()));
			} else {
				Behavior * beh = (Behavior*) ((void * (*)(void))fct_ptr)();
				if (beh == NULL) {
					throw string("Could not allocate behavior object");
				} else {
					group->addBehavior(beh);
				}
			}
		}
	} else {
		group->addBehavior(new FakeModuleBehavior(filename.c_str()));
	}
	EmReadCmp(file, ist, str, "}");
}

/****************************************************************
 ** Top level loading
 ****************************************************************/

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

	float tx, ty, tz, rx, ry, rz;
	this->readNextToken(file, ist, tx); 
	this->readNextToken(file, ist, ty); 
	this->readNextToken(file, ist, tz);
	this->readNextToken(file, ist, rx); 
	this->readNextToken(file, ist, ry);	
	this->readNextToken(file, ist, rz);
	
	group->setTransform(tx, ty, tz, rx, ry, rz);

	this->loadMisc(file, ist, engine, group, NULL);
	return group;
}

/****************************************************************
 ** Shape specific loads
 ****************************************************************/

void Loader::loadShape(ifstream & file, istringstream & ist, Engine * engine, Group * group, Behavior * beh) {
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
	Polygon * poly = new Polygon(shape);

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

void Loader::readPolygonEdge(ifstream & file, istringstream & ist, Polygon* poly) {
	EM_COUT("Loader::readPolygonEdge", 0);
	string str;
	int i;

	EmReadCmp(file, ist, str, "{");

	this->readNextToken(file, ist, i);
#if EM_USE_SHARED_COLOR
	poly->add(i);
#else
	float u, v, a, r, g, b;
	this->readNextToken(file, ist, u);
	this->readNextToken(file, ist, v);
	this->readNextToken(file, ist, r);
	this->readNextToken(file, ist, g);
	this->readNextToken(file, ist, b);
	this->readNextToken(file, ist, a);
	poly->add(i, u, v, r, g, b, a);
#endif

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
#if EM_USE_SHARED_COLOR
	this->readNextToken(file, ist, r);
	this->readNextToken(file, ist, g);
	this->readNextToken(file, ist, b);
	this->readNextToken(file, ist, a);
	this->readNextToken(file, ist, u);
	this->readNextToken(file, ist, v);
#endif

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
			shape->setProperty(EM_SHAPE3D_ALLWAYSLIT);
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
		this->readNextToken(file, ist, str);
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
