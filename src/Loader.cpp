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
#include "Score.h"
#include "StdAnimation.h"

#define CHECK_EOF(file) if (!file) throw string("!!! EOF reached too soon");

void loadMisc(ifstream & file, Engine * engine, Group * group, Behavior * beh);

/* set properties for a group */
void loadProperties(ifstream & file, Group * g) {
	EM_COUT("loadProperties", 1);

	string str;
	
	file >> str;
	if (str != "{") {
		throw string("Parse error \"{\" missing");
	}

	file >> str;
	while (str != "}") {
		CHECK_EOF(file);
		if (str == "transform_once") {
			g->setProperty(EM_GROUP_TRANSFORM_ONCE);
			EM_COUT("--transform once", 1);
		} else if (str == "no_light") {
			g->setProperty(EM_GROUP_NO_LIGHT);
			EM_COUT("--no light", 1);
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
				g->getShape3D(a)->setProperty(EM_SHAPE3D_TRANS);
			}
			EM_COUT("--transparent", 1);
		} else {
			throw string("--UNKNOWN property in property block");
		}
		file >> str;
	}
}

void loadArmBehavior(ifstream & file, Group * group) {
	cerr << "arm" << endl;

	string str;

	file >> str;
	if (str != "{") {
		throw string("Parse error \"{\" missing");
	}

	file >> str;
	if (str == "right") {
		group->addBehavior(new ArmBehavior(true));
	} else {
		group->addBehavior(new ArmBehavior(false));
	}

	file >> str;
	if (str != "}") {
		throw string("Parse error \"}\" missing");
	}
}

void loadAnimation(ifstream & file, Engine * engine, Group * group, Behavior * beh) {
	cerr << "anim" << endl;

	string str;

	EmAssert(beh != NULL, "Behavior NULL in loadAnimation");
	file >> str;
	if (str != "{") {
		throw string("Parse error \"{\" missing");
	}

	int type = EM_LIGHT;
	file >> str;
	if (str == "rotation") {
		type = EM_ROTATION;
	} else if (str == "translation") {
		type = EM_TRANSLATION;
	}
	StdAnimation* anim = new StdAnimation(20, type);

	int count;
	file >> count;
	for (; count > 0; count--) {
		float a, b, c;
		file >> a; file >> b; file >> c;
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
	cerr << "light" << endl;

	string str;
	
	EmAssert(beh != NULL, "Behavior NULL in loadBehaviorLight");
	file >> str;
	if (str != "{") {
		throw string("Parse error \"{\" missing");
	}

	float r, g, b, x, y, z;
	file >> r; file >> g; file >> b;
	file >> x; file >> y; file >> z;

	Light * light = new Light(1.3f, 0.5f, 0.0f, r, g, b);
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
	cerr << "cave" << endl;

	string str;

	file >> str;
	if (str != "{") {
		throw string("Parse error \"{\" missing");
	}

	Behavior* b = new CaveBehavior();
	group->addBehavior(b);

	Shape3D* shape = new Grid(NULL, 2, 2, 1, 0, 1, 1, 0, 1);
	group->addShape3D(shape);
	CollisionBounds* bounds = new CollisionBounds(shape->getCollisionSize());
	bounds->setShape3D(shape, 0); 
	group->setCollisionBounds(bounds);

	loadMisc(file, engine, group, b);
}

void loadBumperBehavior(ifstream & file, Engine * engine, Group * group) {
	cerr << "bumper" << endl;

	string str;

	file >> str;
	if (str != "{") {
		throw string("Parse error \"{\" missing");
	}

	Behavior* b = new BumperBehavior();
	group->addBehavior(b);

	loadMisc(file, engine, group, b);
}

void loadTriggerBehavior(ifstream & file, Engine * engine, Group * group) {
	cerr << "trigger" << endl;
	
	string str;

	file >> str;
	if (str != "{") {
		throw string("Parse error \"{\" missing");
	}
		
	int asig, usig, asigout, usigout, init;
	file >> asig; file >> usig; 
	file >> asigout; file >> usigout;
	file >> init;
	
	TriggerBehavior* b = new TriggerBehavior(asig, usig, asigout, usigout, (init==1));
	group->addBehavior(b);

	file >> str;
	if (str == "texcoord") {
		b->useTexCoord(true);
		char fn[256];
		file >> fn;
		Shape3D * shape = group->getShape3D(1);
		if (shape != NULL) {
				EmTexture* tex = TextureUtil::loadTexture(fn);
				shape->setTexture(tex);
		} else {
			EM_COUT("NO SHAPE in loadTriggerBehavior", 1);
		}
		int count;
		file >> count;
		for (int a=0; a<count; a++) {
			float u, v;
			file >> u; file >> v;
			b->addTexCoordAct(u, v);
		}
		file >> count;
		for (int a=0; a<count; a++) {
			float u, v;
			file >> u; file >> v;
			b->addTexCoordUn(u, v);
		}
		if (init==1) {
			b->setTexCoordAct();
		} else {
			b->setTexCoordUn();
		}
	}
	
	file >> str;
	if (str == "move") {
		b->useMove(true);
		float tx, ty, tz, rx, ry, rz;
		file >> tx; file >> ty; file >> tz;
		file >> rx; file >> ry; file >> rz;
		b->setActiveTransform(tx, ty, tz, rx, ry, rz);
		file >> tx; file >> ty; file >> tz;
		file >> rx; file >> ry; file >> rz;
		b->setUnActiveTransform(tx, ty, tz, rx, ry, rz);
	}

	loadMisc(file, engine, group, b);
}

void loadSwitchBehavior(ifstream & file, Engine * engine, Group * group) {
	cerr << "switch" << endl;

	string str;

	file >> str;
	if (str != "{") {
		throw string("Parse error \"{\" missing");
	}

	int asig, usig, init;
	file >> asig;	file >> usig;	file >> init;

	Behavior* b = new SwitchBehavior(asig, usig, (init==1));
	group->addBehavior(b);

	loadMisc(file, engine, group, b);
}

/* thins added to object, e.g. lights, animation, behavior*/
void loadMisc(ifstream & file, Engine * engine, Group * group, Behavior * beh) {
	cerr << "misc" << endl;

	string str;

	EmAssert(engine != NULL && group != NULL, "Engine or group NULL in loadMisc");

	file >> str;
	while (str != "}") {
		CHECK_EOF(file);
		if (str == "properties"){
			loadProperties(file, group);
		} else if (str == "arm_behavior") {
			loadArmBehavior(file, group);
		} else if (str == "switch_behavior") {
			loadSwitchBehavior(file, engine, group);
		} else if (str == "trigger_behavior") {
			loadTriggerBehavior(file, engine, group);
		} else if (str == "cave_behavior") {
			loadCaveBehavior(file, engine, group);
		} else if (str == "bumper_behavior") {
			loadBumperBehavior(file, engine, group);
			group->setUserProperty(PBL_BUMPER);
		} else if (str == "light") {
			loadBehaviorLight(file, engine, group, beh);
		} else if (str == "animation") {
			loadAnimation(file, engine, group, beh);
		} else {
			throw string("UNKNOWN in misc block");
		}
		file >> str;
	}
}

/* Normal Object */
Group * loadStdObject(ifstream & file, Engine * engine) {
	cerr << "object" << endl;

	char collision[256], shape[256];
	float tx, ty, tz;
	float rx, ry, rz;
	string str;

	file >> str;
	if (str != "{") {
		throw string("Parse error \"{\" missing");
	}

	file >> collision;
	file >> shape;
	file >> tx; file >> ty; file >> tz;
	file >> rx; file >> ry;	file >> rz;
	
	Group* group = new Group();
	group->setTransform(tx, ty, tz, rx, ry, rz);

	// load collision bounds
	if (strstr(collision, "?") == NULL) {			
		Shape3D* shape3d = Shape3DUtil::loadShape3D(collision);
		if (shape3d == NULL) {
			throw string("File not found");
		}
		// TODO add this
		shape3d->setProperty(EM_SHAPE3D_HIDDEN);
		group->addShape3D(shape3d);
		
		CollisionBounds* bounds = new CollisionBounds(shape3d->getCollisionSize());
		if (shape3d->getCollisionSize() > 16) {
			bounds->setShape3D(shape3d, 5); //4
		} else if (shape3d->getCollisionSize() > 8) {
			bounds->setShape3D(shape3d, 4); //4
		} else if (shape3d->getCollisionSize() > 4) {
			bounds->setShape3D(shape3d, 3); //4
		} else if (shape3d->getCollisionSize() > 2) {
			bounds->setShape3D(shape3d, 2); //4
		} else {
			bounds->setShape3D(shape3d, 1);
		}				
		group->setCollisionBounds(bounds);
	}
		
	// load shape
	if (strstr(shape, "?") == NULL) {
		Shape3D* shape3d = Shape3DUtil::loadShape3D(shape);
		if (shape3d == NULL) {
			throw string("File not found");
		}
		group->addShape3D(shape3d);
		// TODO remove
		//shape3d->setProperty(EM_SHAPE3D_HIDDEN);
	}

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
	}
	return 0;
}

#undef CHECK_EOF
