/***************************************************************************
                            -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by henqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <fstream>
#include <string>

#include <stdlib.h>
#include <time.h>
//#include <values.h>
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
#include "Shape3DUtil.h"
#include "Grid.h"
#include "KeyRotBehavior.h"
#include "Polygon.h"
#include "BumperBehavior.h"
#include "CaveBehavior.h"
#include "LockBehavior.h"
#include "TriggerBehavior.h"
#include "SwitchBehavior.h"
#include "TextureUtil.h"
#include "SoundUtil.h"
#include "Score.h"
#include "StdAnimation.h"
#include "Config.h"
#include "StateBehavior.h"


#define EM_READ(file, str) if (!(file >> str)) throw string("EOF reached too soon");

#define EM_READ_CMP(file, str, cmp) \
	if (!(file >> str)) throw string("EOF reached too soon"); \
	if (str != cmp) throw string("Parse error, unexpected token");


void loadMisc(ifstream & file, Engine * engine, Group * group, Behavior * beh);

/* set properties for a group */
void loadProperties(ifstream & file, Group * g) {
	EM_COUT("loadProperties", 1);

	string str;
	
	EM_READ_CMP(file, str, "{");

	EM_READ(file, str);

	while (str != "}") {
		if (str == "transform_once") {
			g->setProperty(EM_GROUP_TRANSFORM_ONCE);
			EM_COUT("--transform once", 1);
		} else if (str == "no_light") {
			g->setProperty(EM_GROUP_NO_LIGHT);
			EM_COUT("--no light", 1);
		} else if (str == "behind") {
			for (int a=0; a<g->getShape3DSize(); a++) {
				g->getShape3D(a)->setProperty(EM_SHAPE3D_BEHIND);
			}
			EM_COUT("--behind", 1);
		} else if (str == "light_once") {
			g->setProperty(EM_GROUP_LIGHT_ONCE);
			EM_COUT("--light once", 1);
		} else if (str == "no_signal") {
			g->setProperty(EM_GROUP_NO_SIGNAL);
			EM_COUT("--no signal - deprecated", 1);
		} else if (str == "user_walls") {
			g->setUserProperty(PBL_WALLS);
			EM_COUT("--wall", 1);
		} else if (str == "user_walls_one") {
			g->setUserProperty(PBL_WALLS_ONE);
			EM_COUT("--wall_one", 1);
		} else if (str == "user_group1") {
			g->setUserProperty(PBL_GROUP1);
			EM_COUT("--group 1", 1);
		} else if (str == "alpha_test") {
			for (int a=0; a<g->getShape3DSize(); a++) {
				g->getShape3D(a)->setProperty(EM_SHAPE3D_ALPHATEST);
			}
			EM_COUT("--alpha test", 1);
		} else if (str == "transparent") {
			for (int a=0; a<g->getShape3DSize(); a++) {
				Shape3D* shape = g->getShape3D(a);
				shape->setProperty(EM_SHAPE3D_USE_TRANS);
				vector<Polygon*>::iterator polyIter = shape->m_vPolygon.begin();
				vector<Polygon*>::iterator polyEnd = shape->m_vPolygon.end();
				for (; polyIter != polyEnd; polyIter++) {
					(*polyIter)->setProperty(EM_POLY_TRANS);
				}
			}
			EM_COUT("--transparent", 1);
		} else {
			throw string("--UNKNOWN property in property block");
		}
		EM_READ(file, str);
	}
}

void loadArmBehavior(ifstream & file, Group * group) {
	EM_COUT("arm", 1);

	string str;
	ArmBehavior* beh = NULL;
	
	EM_READ_CMP(file, str, "{");
	
	EM_READ(file, str);
	if (str == "right") {
		beh = new ArmBehavior(true);
	} else {
		beh = new ArmBehavior(false);
	}
	group->addBehavior(beh);	

	EM_READ(file, str);
	if (str == "sound") {
		char soundname[256], filename[256];
		EM_READ(file, soundname);
		sprintf(filename, "%s/%s", Config::getInstance()->getDataDir(), soundname);
		int sound = SoundUtil::getInstance()->loadSample(filename);
		beh->setSound(sound);
	} else if (str == "no_sound"){
	} else {
		throw string("No sound field in ArmBehavior");
	}

	EM_READ_CMP(file, str, "}");
}

void loadAnimation(ifstream & file, Engine * engine, Group * group, Behavior * beh) {
	EM_COUT("anim", 1);

	string str;

	EmAssert(beh != NULL, "Behavior NULL in loadAnimation");
	EM_READ_CMP(file, str, "{");

	int type = EM_LIGHT;
	EM_READ(file, str);
	if (str == "rotation") {
		type = EM_ROTATION;
	} else if (str == "translation") {
		type = EM_TRANSLATION;
	}
	StdAnimation* anim = new StdAnimation(50, type);

	int count;
	EM_READ(file, count);
	for (; count > 0; count--) {
		float a, b, c;
		EM_READ(file, a); 
		EM_READ(file, b); 
		EM_READ(file, c);
		if (count == 1) {
			anim->setEnd(a, b, c);
		} else {
			anim->add(a, b, c);
		}
	}
	group->addBehavior(anim);

	loadMisc(file, engine, group, anim);
}


void loadBehaviorLight(ifstream & file, Engine * engine, Group * group, Behavior * beh) {
	EM_COUT("light", 1);

	string str;
	
	EmAssert(beh != NULL, "Behavior NULL in loadBehaviorLight");
	EM_READ_CMP(file, str, "{");

	float r, g, b, x, y, z;
	EM_READ(file, r); 
	EM_READ(file, g); 
	EM_READ(file, b);
	EM_READ(file, x); 
	EM_READ(file, y); 
	EM_READ(file, z);

	Light * light = new Light(1.0f, 0.0f, 0.1f, r, g, b);
	light->setOn(false);
	light->setProperty(EM_IGNORE_ANGLE_FULL);
	//light->setProperty(EM_IGNORE_DISTANCE);
	light->setProperty(EM_USE_BOUNDS);
	light->setBounds(10.0);
	
	Group * gl = new Group();
	gl->addTranslation(x, y, z);
	gl->setLight(light);
	group->add(gl);
	engine->addLight(light);
	
	beh->setLight(light);

	loadMisc(file, engine, gl, beh);
}

void loadCaveBehavior(ifstream & file, Engine * engine, Group* group) {
	EM_COUT("cave", 1);

	string str;

	EM_READ_CMP(file, str, "{");

	CaveBehavior* beh = new CaveBehavior();
	group->addBehavior(beh);

	EM_READ(file, str);
	if (str == "sound") {
		char soundname[256], filename[256];
		EM_READ(file, soundname);
		sprintf(filename, "%s/%s", Config::getInstance()->getDataDir(), soundname);
		int sound = SoundUtil::getInstance()->loadSample(filename);
		beh->setSound(sound);
	} else if (str == "no_sound"){
	} else {
		throw string("No sound field in CaveBehavior");
	}

	Shape3D* shape = new Grid(NULL, 2, 2, 1, 0, 1, 1, 0, 1);
	group->addShape3D(shape);
	CollisionBounds* bounds = new CollisionBounds(shape->getCollisionSize());
	bounds->setShape3D(shape, 0); 
	group->setCollisionBounds(bounds);

	loadMisc(file, engine, group, beh);
}
/*
void loadLockBehavior(ifstream & file, Engine * engine, Group* group) {
	EM_COUT("lock", 1);

	string str;

	EM_READ_CMP(file, str,  "{");

	int asig, usig, lsig;
	EM_READ(file, asig); 
	EM_READ(file, usig); 
	EM_READ(file, lsig);

	Behavior* b = new LockBehavior(asig, usig, lsig, false);
	group->addBehavior(b);

	Shape3D* shape = new Grid(NULL, 2, 2, 1, 0, 1, 1, 0, 1);
	group->addShape3D(shape);
	CollisionBounds* bounds = new CollisionBounds(shape->getCollisionSize());
	bounds->setShape3D(shape, 0); 
	group->setCollisionBounds(bounds);

	loadMisc(file, engine, group, b);
}
*/
void loadBumperBehavior(ifstream & file, Engine * engine, Group * group) {
	EM_COUT("bumper", 1);

	string str;

	EM_READ_CMP(file, str, "{");

	BumperBehavior* beh = new BumperBehavior();
	group->addBehavior(beh);

	EM_READ(file, str);
	if (str == "sound") {
		char soundname[256], filename[256];
		EM_READ(file, soundname);
		sprintf(filename, "%s/%s", Config::getInstance()->getDataDir(), soundname);
		int sound = SoundUtil::getInstance()->loadSample(filename);
		beh->setSound(sound);
	} else if (str == "no_sound"){
	} else {
		throw string("No sound field in BumperBehavior");
	}

	loadMisc(file, engine, group, beh);
}

void loadTriggerBehavior(ifstream & file, Engine * engine, Group * group) {
	EM_COUT("trigger", 1);
	
	string str;

	EM_READ_CMP(file, str, "{");
		
	int asig, usig, asigout, usigout, init;
	EM_READ(file, asig); EM_READ(file, usig); 
	EM_READ(file, asigout); EM_READ(file, usigout);
	EM_READ(file, init);
	
	TriggerBehavior* beh = new TriggerBehavior(asig, usig, asigout, usigout, (init==1));
	group->addBehavior(beh);

	EM_READ(file, str);
	if (str == "texcoord") {
		beh->useTexCoord(true);
		char texname[256], filename[256];
		EM_READ(file, texname);
		Shape3D * shape = group->getShape3D(0);
		if (shape != NULL) {
			sprintf(filename, "%s/%s", Config::getInstance()->getDataDir(), texname);
			EmTexture* tex = TextureUtil::getInstance()->loadTexture(filename);
			shape->setTexture(tex);
		} else {
			EM_COUT("NO SHAPE in loadTriggerBehavior", 1);
		}
		int count;
 		EM_READ(file, count);
		for (int a=0; a<count; a++) {
			float u, v;
			EM_READ(file, u); 
			EM_READ(file, v);
			beh->addTexCoordAct(u, v);
		}
		EM_READ(file, count);
		for (int a=0; a<count; a++) {
			float u, v;
			EM_READ(file, u); EM_READ(file, v);
			beh->addTexCoordUn(u, v);
		}
		if (init==1) {
			beh->setTexCoordAct();
		} else {
			beh->setTexCoordUn();
		}
	}
	
	EM_READ(file, str);
	if (str == "move") {
		beh->useMove(true);
		float tx, ty, tz, rx, ry, rz;
		EM_READ(file, tx); 
		EM_READ(file, ty); 
		EM_READ(file, tz);
		EM_READ(file, rx); 
		EM_READ(file, ry); 
		EM_READ(file, rz);
		beh->setActiveTransform(tx, ty, tz, rx, ry, rz);
		EM_READ(file, tx); 
		EM_READ(file, ty);
		EM_READ(file, tz);
		EM_READ(file, rx); 
		EM_READ(file, ry); 
		EM_READ(file, rz);
		beh->setUnActiveTransform(tx, ty, tz, rx, ry, rz);
	} 

	loadMisc(file, engine, group, beh);
}

void loadStateItem(ifstream & file, Engine * engine, Group * group, Behavior * beh) {
	EM_COUT("state item", 1);

	string str;
	
	EmAssert(beh != NULL, "Behavior NULL in loadBehaviorLight");
	EmAssert(beh->getType() == PBL_TYPE_STATEBEH, "Not StateBehavior");

	EM_READ_CMP(file, str, "{");

	int asig, csig, delay;
	EM_READ(file, asig); 
	EM_READ(file, csig);
	EM_READ(file, delay);
	StateItem* stateitem = new StateItem(asig, csig, delay);
	((StateBehavior*)beh)->addStateItem(stateitem);

	EM_READ(file, str);
	if (str == "move") {
		((StateBehavior*)beh)->useMove(true);
		float tx, ty, tz, rx, ry, rz, speed;
		EM_READ(file, speed);
		EM_READ(file, tx); 
		EM_READ(file, ty); 
		EM_READ(file, tz);
		EM_READ(file, rx); 
		EM_READ(file, ry); 
		EM_READ(file, rz);
		stateitem->setTr(tx, ty, tz, speed);
		stateitem->setRot(rx, ry, rz, speed);
	} else if (str == "no_move") {
	} else {
		throw string("No move field in StateItem");
	}

	EM_READ(file, str);
	if (str == "light") {
		stateitem->setLight(true);
	} else if (str == "no_light"){
		stateitem->setLight(false);
	} else {
		throw string("No light field in StateItem");
	}

	EM_READ(file, str);
	if (str == "sound") {
		char soundname[256], filename[256];
		EM_READ(file, soundname);
		sprintf(filename, "%s/%s", Config::getInstance()->getDataDir(), soundname);
		int sound = SoundUtil::getInstance()->loadSample(filename);
		stateitem->setSound(sound);
	} else if (str == "no_sound"){
	} else {
		throw string("No sound field in StateItem");
	}

	EM_READ(file, str);
	if (str == "property") {
		int p;
		EM_READ(file, p);
		stateitem->setProperty(p);
	} else if (str == "no_property") {
	} else {
		throw string("No property field in StateItem");
	}

	EM_READ(file, str);
	if (str == "texcoord") {
		((StateBehavior*)beh)->useTexCoord(true);
		char texname[256], filename[256];
		EM_READ(file, texname);
		Shape3D * shape = group->getShape3D(0);
		if (shape != NULL) {
			sprintf(filename, "%s/%s", Config::getInstance()->getDataDir(), texname);
			EmTexture* tex = TextureUtil::getInstance()->loadTexture(filename);
			shape->setTexture(tex);
		} else {
			throw string("No shape in for StateItem");
		}
		int count;
 		EM_READ(file, count);
		for (int a=0; a<count; a++) {
			float u, v;
			EM_READ(file, u); 
			EM_READ(file, v);
			stateitem->addTexCoord(u, v);
		}
	} else if (str == "no_texcoord") {
	} else {
		throw string("No texcoord field in StateItem");
	}

	loadMisc(file, engine, group, beh);
}

void loadStateBehavior(ifstream & file, Engine * engine, Group * group) {
	EM_COUT("state", 1);
	
	string str;

	EM_READ_CMP(file, str, "{");
		
	StateBehavior* b = new StateBehavior();
	group->addBehavior(b);

	loadMisc(file, engine, group, b);
}
/*
void loadSwitchBehavior(ifstream & file, Engine * engine, Group * group) {
	EM_COUT("switch", 1);

	string str;

	EM_READ_CMP(file, str, "{");

	int asig, usig, init;
	EM_READ(file, asig);	
	EM_READ(file, usig);	
	EM_READ(file, init);

	Behavior* b = new SwitchBehavior(asig, usig, (init==1));
	group->addBehavior(b);

	loadMisc(file, engine, group, b);
}
*/
/* Things added to objects, e.g. lights, animation, behavior*/
void loadMisc(ifstream & file, Engine * engine, Group * group, Behavior * beh) {
	EM_COUT("misc", 1);

	string str;

	EmAssert(engine != NULL && group != NULL, "Engine or group NULL in loadMisc");

	EM_READ(file, str);
	while (str != "}") {
		if (str == "properties"){
			loadProperties(file, group);
		} else if (str == "arm_behavior") {
			loadArmBehavior(file, group);
// 		} else if (str == "switch_behavior") {
// 			loadSwitchBehavior(file, engine, group);
		} else if (str == "trigger_behavior") {
			loadTriggerBehavior(file, engine, group);
		} else if (str == "state_behavior") {
			loadStateBehavior(file, engine, group);
		} else if (str == "cave_behavior") {
			loadCaveBehavior(file, engine, group);
// 		} else if (str == "lock_behavior") {
// 			loadLockBehavior(file, engine, group);
		} else if (str == "bumper_behavior") {
			loadBumperBehavior(file, engine, group);
			group->setUserProperty(PBL_BUMPER);
		} else if (str == "light") {
			loadBehaviorLight(file, engine, group, beh);
		} else if (str == "state_item") {
			loadStateItem(file, engine, group, beh);
		} else if (str == "animation") {
			loadAnimation(file, engine, group, beh);
		} else {
			cerr << str << endl;
			throw string("UNKNOWN in misc block");
		}
		EM_READ(file, str);
	}
}

/* Top level object */
Group * loadStdObject(ifstream & file, Engine * engine) {
	char fn[256], filename[256];
	string str;

	EM_READ(file, str);
	EM_COUT("object " << str, 1);

	EM_READ_CMP(file, str, "{");

	Group* group = new Group();
	Shape3D* shape = NULL;

	EM_READ(file, str);
	// load shape
	if (str == "visual") {
		EM_READ(file, fn);
		sprintf(filename, "%s/%s", Config::getInstance()->getDataDir(), fn);
		shape = Shape3DUtil::loadShape3D(filename);
		if (shape == NULL) {
			throw string("File not found");
		}
		group->addShape3D(shape);
	} else if (str == "no_visual") {
	} else {
		throw string("Parse error: visual missing");
	}

	EM_READ(file, str);
	if (str == "collision_same") {
		// use object as collision shape
		if (shape == NULL) {
			throw string("No shape loaded");
		}
		// find a suitable split for the collisionbounds
		CollisionBounds* bounds = new CollisionBounds(shape->getCollisionSize());
		if (shape->getCollisionSize() > 16) {
			bounds->setShape3D(shape, 5); //4
		} else if (shape->getCollisionSize() > 8) {
			bounds->setShape3D(shape, 4); //4
		} else if (shape->getCollisionSize() > 4) {
			bounds->setShape3D(shape, 3); //4
		} else if (shape->getCollisionSize() > 2) {
			bounds->setShape3D(shape, 2); //4
		} else {
			bounds->setShape3D(shape, 1);
		}				
		group->setCollisionBounds(bounds);
	} else if (str == "collision_other") {
		// use diffrent collision shape
		EM_READ(file, fn);
		sprintf(filename, "%s/%s", Config::getInstance()->getDataDir(), fn);
		Shape3D* collshape = Shape3DUtil::loadShape3D(filename);
		if (collshape == NULL) {
			throw string("File not found");
		}
		// TODO add this
		collshape->setProperty(EM_SHAPE3D_HIDDEN);
		group->addShape3D(collshape);
		
		// find a suitable split for the collisionbounds
		CollisionBounds* bounds = new CollisionBounds(collshape->getCollisionSize());
		if (collshape->getCollisionSize() > 16) {
			bounds->setShape3D(collshape, 5); //4
		} else if (collshape->getCollisionSize() > 8) {
			bounds->setShape3D(collshape, 4); //4
		} else if (collshape->getCollisionSize() > 4) {
			bounds->setShape3D(collshape, 3); //4
		} else if (collshape->getCollisionSize() > 2) {
			bounds->setShape3D(collshape, 2); //4
		} else {
			bounds->setShape3D(collshape, 1);
		}				
		group->setCollisionBounds(bounds);
	} else if (str == "no_collision") {
	} else {
		throw string("Parse error: collision missing");
	}

	float tx, ty, tz, rx, ry, rz;
	EM_READ(file, tx); 
	EM_READ(file, ty); 
	EM_READ(file, tz);
	EM_READ(file, rx); 
	EM_READ(file, ry);	
	EM_READ(file, rz);
	
	group->setTransform(tx, ty, tz, rx, ry, rz);
	//
	loadMisc(file, engine, group, NULL);

	return group;
}

int loadFile(const char* fn, Engine * engine) {
	ifstream file(fn);
	try {
		if (!file) {
			throw string("Shape3DUtil::load() : file not found: ");
		}
		string str;
		Group * g = NULL;

		while (file >> str) {
			if (str == "object") {
				g = loadStdObject(file, engine); 
				engine->add(g);
			}
		}
	} catch (string str) {
		cerr << str << endl;
		cerr << "When loading file: " << fn << endl;
		return -1;
	}
	return 0;
}
