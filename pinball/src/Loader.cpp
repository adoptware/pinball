/***************************************************************************
                            -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by 
    email                : 
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
#include "Animation.h"
#include "SignalVisitor.h"

/* Normal Object
 */
Group * loadStdObject(ifstream & file) {
	char collision[256], shape[256];
	float tx, ty, tz;
	float rx, ry, rz;
	float r, g, b;

	file >> collision;
	file >> shape;
	file >> tx; file >> ty; file >> tz;
	file >> rx; file >> ry;	file >> rz;
	file >> r; file >> g; file >> b;

	cout << collision << endl;
	cout << shape << endl;
	cout << tx <<" "<< ty <<" "<< tz << endl;
	cout << rx <<" "<< ry <<" "<< rz << endl;
	cout << r <<" "<< g <<" "<< b << endl;

	Group* group = new Group();
	group->setTransform(tx, ty, tz, rx, ry, rz);

	// load collision bounds
	if (strstr(collision, "?") == NULL) {			
		Shape3D* shape3d = Shape3DUtil::loadShape3D(collision);
		if (shape3d != NULL) {
			//shape3d->setPolygonProperty(EM_POLY_HIDDEN);
			group->addShape3D(shape3d);
			
			CollisionBounds* bounds = new CollisionBounds(shape3d->getCollisionSize());
			if (shape3d->getCollisionSize() > 10) {
				bounds->setShape3D(shape3d, 4); //4
			} else {
				bounds->setShape3D(shape3d, 0);
			}				
			group->setCollisionBounds(bounds);
		}
	}
		
	// load shape
	if (strstr(shape, "?") == NULL) {
		Shape3D* shape3d = Shape3DUtil::loadShape3D(shape);
		group->addShape3D(shape3d);
		shape3d->setPolygonProperty(EM_POLY_HIDDEN);
	}

	return group;
}

/* light
 */
Group * loadStdLight(ifstream & file, Engine * engine, Behavior * behavior) {
	string str;
	Group * group = NULL;
	file >> str;
	if (str == "light" ) {
		float tx, ty, tz;
		int r, g, b;
		file >> tx; file >> ty; file >> tz;
		file >> r;  file >> g;  file >> b;
		group = new Group();
		Light * light = new Light(1.3f, 0.5f, 0.0f, r, g, b);
		//		light->setColor(r, g, b);
		light->setOn(false);
		light->setProperty(EM_IGNORE_ANGLE_FULL);
		//light->setProperty(EM_IGNORE_DISTANCE);
		light->setProperty(EM_USE_BOUNDS);
		light->setBounds(10.0);

		if (behavior != NULL) behavior->setLight(light);

		engine->addLight(light);
		group->setTranslation(tx, ty, tz);
		group->setLight(light);
		cerr << "loaded light" << endl;
	}
	return group;
}

/* Animation
 */
Animation * loadStdAnimation(ifstream & file, Engine * engine) {
	string str;
	Animation * anim = NULL;
	file >> str;
	if (str == "animation" ) {
		string str;
		int steps, speed;
		file >> str;
		file >> steps;
		file >> speed;
		if (str == "type_light") {
			anim = new Animation(speed, EM_LIGHT);
		} else if (str == "type_rot") {
			anim = new Animation(speed, EM_ROTATION);
		} else {
			anim = new Animation(speed, EM_TRANSLATION);
		}
		for (int a=0; a<steps; a++) {
			float tx, ty, tz;
			file >> tx; file >> ty; file >> tz;
			anim->add(tx, ty, tz);
		}
		anim->setEndStart();
		cerr << "loaded animation" << endl;
	}
	return anim;
}

/* Walls
 */
void loadWalls(ifstream & file, Engine * engine) {
	Group * group = loadStdObject(file);
	engine->add(group);

	group->setUserProperty(PBL_WALLS);
	group->setUserProperty(PBL_GROUP1);
	group->setProperty(EM_GROUP_TRANSFORM_ONCE);
			
	cerr << "loaded walls" << endl;
}

/* Left arm
 */
void loadLeftarm(ifstream & file, Engine * engine) {
	Group * group = loadStdObject(file);
	engine->add(group);

	ArmBehavior* armBeh = new ArmBehavior(false);

	group->setBehavior(armBeh);
	group->setUserProperty(PBL_GROUP1);
}

/* Right arm
 */
void loadRightarm(ifstream & file, Engine * engine) {
	Group * group = loadStdObject(file);
	engine->add(group);

	ArmBehavior* armBeh = new ArmBehavior(true);

	group->setBehavior(armBeh);
	group->setUserProperty(PBL_GROUP1);
}

/* Bumper
 */
void loadBumper(ifstream & file, Engine * engine) {
	Group * group = loadStdObject(file);
	engine->add(group);
	// The collision cylinder
	Shape3D* shape3d_c = new Cylinder(4.0, 1.0, 1.0, 0.0, 0.0, false);
	//shape3d_c->setPolygonProperty(EM_POLY_HIDDEN);
	group->addShape3D(shape3d_c);

	CollisionBounds* bounds = new CollisionBounds(shape3d_c->getCollisionSize());
	bounds->setShape3D(shape3d_c, 0);
	group->setCollisionBounds(bounds);

	// The visible cylinder
	Shape3D* shape3d = new Cylinder(4.0, 1.0, 1.0, 0.0, 0.0);
	group->addShape3D(shape3d);
	shape3d->setPolygonProperty(EM_POLY_HIDDEN);

	BumperBehavior* bumperBeh = new BumperBehavior();
	group->setBehavior(bumperBeh);
	group->setUserProperty(PBL_BUMPER);
	group->setUserProperty(PBL_GROUP1);
	group->setProperty(EM_GROUP_TRANSFORM_ONCE);
				
	Group * groupL = loadStdLight(file, engine, bumperBeh);
	group->add(groupL);	
  		
	cerr << "loaded bumper" << endl;
}

/* Flat bumper
 */
void loadFlatbumper(ifstream & file, Engine * engine) {
	Group * group = loadStdObject(file);
	engine->add(group);
	// The collision grid
	Shape3D* shape3d_c = new Grid(NULL, 4.0, 4.0, 1, 0);
	shape3d_c->setPolygonProperty(EM_POLY_HIDDEN);
	group->addShape3D(shape3d_c);

	CollisionBounds* bounds = new CollisionBounds(shape3d_c->getCollisionSize());
	bounds->setShape3D(shape3d_c, 0);
	group->setCollisionBounds(bounds);

	// The visible grid
	Shape3D* shape3d = new Grid(NULL, 4.0, 4.0, 1, 0);
	group->addShape3D(shape3d);

	BumperBehavior* bumperBeh = new BumperBehavior();
	group->setBehavior(bumperBeh);
	group->setUserProperty(PBL_BUMPER);
	group->setUserProperty(PBL_GROUP1);
	group->setProperty(EM_GROUP_TRANSFORM_ONCE);

	Group * groupL = loadStdLight(file, engine, bumperBeh);
	group->add(groupL);	
  		
	cerr << "loaded flatbumper" << endl;
}

/* Target
 */
void loadTarget(ifstream & file, Engine * engine) {
	string str;
	file >> str;

	Group * group = loadStdObject(file);
	engine->add(group);
	// The collision grid
	Shape3D* shape3d_c = new Grid(NULL, 2.0, 2.0, 1, 0);
	shape3d_c->setPolygonProperty(EM_POLY_HIDDEN);
	group->addShape3D(shape3d_c);

	CollisionBounds* bounds = new CollisionBounds(shape3d_c->getCollisionSize());
	bounds->setShape3D(shape3d_c, 0);
	group->setCollisionBounds(bounds);

	// The visible grid
	Shape3D* shape3d = new Grid(NULL, 2.0, 2.0, 1, 0, 0.0, 1.0, 0.0, 1.0);
	group->addShape3D(shape3d);

	TriggerBehavior* triggerBeh = NULL;
	if (str == "1") {
		triggerBeh = new TriggerBehavior( PBL_SIG_NEW_TARGET, PBL_SIG_TARGET_1, 
																			PBL_SIG_TARGET_1, true);
	} else {
		triggerBeh = new TriggerBehavior( PBL_SIG_NEW_TARGET, PBL_SIG_TARGET_2, 
																			PBL_SIG_TARGET_2, true);
	}
	group->setBehavior(triggerBeh);
	group->setUserProperty(PBL_GROUP1);
	group->setProperty(EM_GROUP_TRANSFORM_ONCE);

	Group * groupL = loadStdLight(file, engine, triggerBeh);
	Animation * anim = loadStdAnimation(file, engine);
	if (groupL != NULL) groupL->setAnimation(anim);
	group->add(groupL);	
  		
	cerr << "loaded target" << str<< endl;
}

/* Cave
 */
void loadCave(ifstream & file, Engine * engine) {
	Group * group = loadStdObject(file);
	engine->add(group);
	// The collision grid
	Shape3D* shape3d_c = new Grid(NULL, 2.0, 2.0, 1, 0);
	shape3d_c->setPolygonProperty(EM_POLY_HIDDEN);
	group->addShape3D(shape3d_c);

	CollisionBounds* bounds = new CollisionBounds(shape3d_c->getCollisionSize());
	bounds->setShape3D(shape3d_c, 0);
	group->setCollisionBounds(bounds);

	// The visible grid TODO: remove me
	//	Shape3D* shape3d = new Grid(NULL, 2.0, 2.0, 1, 0, 0.0, 1.0, 0.0, 1.0);
	//	group->addShape3D(shape3d);

	CaveBehavior* caveBeh = new CaveBehavior();
	group->setBehavior(caveBeh);
	group->setUserProperty(PBL_GROUP1);
	group->setProperty(EM_GROUP_TRANSFORM_ONCE);

	Group * groupL = loadStdLight(file, engine, caveBeh);
	Animation * anim = loadStdAnimation(file, engine);
	if (groupL != NULL) groupL->setAnimation(anim);
	group->add(groupL);	
  		
	cerr << "loaded cave" << endl;
}

/* Lock target
 */
void loadLock_target(ifstream & file, Engine * engine) {
	Group * group = loadStdObject(file);
	engine->add(group);
	// The collision grid
	Shape3D* shape3d_c = new Grid(NULL, 2.0, 2.0, 1, 0);
	shape3d_c->setPolygonProperty(EM_POLY_HIDDEN);
	group->addShape3D(shape3d_c);

	CollisionBounds* bounds = new CollisionBounds(shape3d_c->getCollisionSize());
	bounds->setShape3D(shape3d_c, 0);
	group->setCollisionBounds(bounds);

	// The visible grid
	Shape3D* shape3d = new Grid(NULL, 2.0, 2.0, 1, 0, 0.0, 1.0, 0.0, 1.0);
	group->addShape3D(shape3d);

	TriggerBehavior* triggerBeh = new TriggerBehavior( PBL_SIG_NEW_LOCK, PBL_SIG_ACTIVATE_LOCK, 
																										 PBL_SIG_ACTIVATE_LOCK, true);
	group->setBehavior(triggerBeh);
	group->setUserProperty(PBL_GROUP1);
	group->setProperty(EM_GROUP_TRANSFORM_ONCE);

	Group * groupL = loadStdLight(file, engine, triggerBeh);
	Animation * anim = loadStdAnimation(file, engine);
	if (groupL != NULL) groupL->setAnimation(anim);
	group->add(groupL);	
  		
	cerr << "loaded lock target" << endl;
}

/* Lock 1
 */
void loadLock(ifstream & file, Engine * engine) {
	string str;
	file >> str;

	Group * group = loadStdObject(file);
	engine->add(group);

	// The collision grid
	Shape3D* shape3d_c = new Grid(NULL, 2.0, 2.0, 1, 0);
	shape3d_c->setPolygonProperty(EM_POLY_HIDDEN);
	group->addShape3D(shape3d_c);

	CollisionBounds* bounds = new CollisionBounds(shape3d_c->getCollisionSize());
	bounds->setShape3D(shape3d_c, 0);
	group->setCollisionBounds(bounds);

	// The visible grid TODO: remove me
	//Shape3D* shape3d = new Grid(NULL, 2.0, 2.0, 1, 0, 0.0, 1.0, 0.0, 1.0);
	//group->addShape3D(shape3d);

	LockBehavior* lockBeh = NULL;
	if (str == "1") {
		lockBeh = new LockBehavior(PBL_SIG_ACTIVATE_LOCK, PBL_SIG_RELEASE_LOCK, PBL_SIG_LOCK_1);
	}	else {
		lockBeh = new LockBehavior(PBL_SIG_LOCK_1, PBL_SIG_RELEASE_LOCK, PBL_SIG_LOCK_2);
	}
	group->setBehavior(lockBeh);
	group->setUserProperty(PBL_GROUP1);
	group->setProperty(EM_GROUP_TRANSFORM_ONCE);

	Group * groupL = loadStdLight(file, engine, lockBeh);
	Animation * anim = loadStdAnimation(file, engine);
	if (groupL != NULL) groupL->setAnimation(anim);
	group->add(groupL);	
  		
	cerr << "loaded lock " << str << endl;
}

/* Lock release
 */
void loadLock_release(ifstream & file, Engine * engine) {
	Group * group = loadStdObject(file);
	engine->add(group);
	// The collision grid
	Shape3D* shape3d_c = new Grid(NULL, 2.0, 2.0, 1, 0);
	shape3d_c->setPolygonProperty(EM_POLY_HIDDEN);
	group->addShape3D(shape3d_c);

	CollisionBounds* bounds = new CollisionBounds(shape3d_c->getCollisionSize());
	bounds->setShape3D(shape3d_c, 0);
	group->setCollisionBounds(bounds);

	// The visible grid TODO: remove me
	Shape3D* shape3d = new Grid(NULL, 2.0, 2.0, 1, 0, 0.0, 1.0, 0.0, 1.0);
	group->addShape3D(shape3d);

	TriggerBehavior* triggerBeh = new TriggerBehavior( PBL_SIG_LOCK_2, PBL_SIG_RELEASE_LOCK, 
																										 PBL_SIG_RELEASE_LOCK);
	group->setBehavior(triggerBeh);
	group->setUserProperty(PBL_GROUP1);
	group->setProperty(EM_GROUP_TRANSFORM_ONCE);

	Group * groupL = loadStdLight(file, engine, triggerBeh);
	Animation * anim = loadStdAnimation(file, engine);
	if (groupL != NULL) groupL->setAnimation(anim);
	group->add(groupL);	
  		
	cerr << "loaded lock release" << endl;
}

/* Jackpot
 */
void loadJackpot(ifstream & file, Engine * engine) {
	Group * group = loadStdObject(file);
	engine->add(group);
	// The collision grid
	Shape3D* shape3d_c = new Grid(NULL, 2.0, 2.0, 1, 0);
	shape3d_c->setPolygonProperty(EM_POLY_HIDDEN);
	group->addShape3D(shape3d_c);

	CollisionBounds* bounds = new CollisionBounds(shape3d_c->getCollisionSize());
	bounds->setShape3D(shape3d_c, 0);
	group->setCollisionBounds(bounds);

	// The visible grid TODO: remove me
	//Shape3D* shape3d = new Grid(NULL, 2.0, 2.0, 1, 0, 0.0, 1.0, 0.0, 1.0);
	//group->addShape3D(shape3d);

	TriggerBehavior* triggerBeh = new TriggerBehavior( PBL_SIG_RELEASE_LOCK, 
																										 PBL_SIG_MULTIBALL_DEAD, PBL_SIG_JACKPOT);
	group->setBehavior(triggerBeh);
	group->setUserProperty(PBL_GROUP1);
	group->setProperty(EM_GROUP_TRANSFORM_ONCE);

	Group * groupL = loadStdLight(file, engine, triggerBeh);
	Animation * anim = loadStdAnimation(file, engine);
	if (groupL != NULL) groupL->setAnimation(anim);
	group->add(groupL);	
  		
	cerr << "loaded jackpot" << endl;
}

/* Left loop
 */
void loadLeft_loop(ifstream & file, Engine * engine) {
	Group * group = loadStdObject(file);
	engine->add(group);
	// The collision grid
	Shape3D* shape3d_c = new Grid(NULL, 2.0, 2.0, 1, 0);
	shape3d_c->setPolygonProperty(EM_POLY_HIDDEN);
	group->addShape3D(shape3d_c);

	CollisionBounds* bounds = new CollisionBounds(shape3d_c->getCollisionSize());
	bounds->setShape3D(shape3d_c, 0);
	group->setCollisionBounds(bounds);

	// The visible grid TODO: remove me
	//Shape3D* shape3d = new Grid(NULL, 2.0, 2.0, 1, 0, 0.0, 1.0, 0.0, 1.0);
	//group->addShape3D(shape3d);

	TriggerBehavior* triggerBeh = new TriggerBehavior( PBL_SIG_ACTIVATE_LEFT_LOOP, 
																										 PBL_SIG_LEFT_LOOP, PBL_SIG_LEFT_LOOP);
	group->setBehavior(triggerBeh);
	group->setUserProperty(PBL_GROUP1);
	group->setProperty(EM_GROUP_TRANSFORM_ONCE);

	Group * groupL = loadStdLight(file, engine, triggerBeh);
	Animation * anim = loadStdAnimation(file, engine);
	if (groupL != NULL) groupL->setAnimation(anim);
	group->add(groupL);	
  		
	cerr << "loaded left loop" << endl;
}

/* Right loop
 */
void loadRight_loop(ifstream & file, Engine * engine) {
	Group * group = loadStdObject(file);
	engine->add(group);
	// The collision grid
	Shape3D* shape3d_c = new Grid(NULL, 2.0, 2.0, 1, 0);
	shape3d_c->setPolygonProperty(EM_POLY_HIDDEN);
	group->addShape3D(shape3d_c);

	CollisionBounds* bounds = new CollisionBounds(shape3d_c->getCollisionSize());
	bounds->setShape3D(shape3d_c, 0);
	group->setCollisionBounds(bounds);

	// The visible grid TODO: remove me
	//Shape3D* shape3d = new Grid(NULL, 2.0, 2.0, 1, 0, 0.0, 1.0, 0.0, 1.0);
	//group->addShape3D(shape3d);

	TriggerBehavior* triggerBeh = new TriggerBehavior( PBL_SIG_ACTIVATE_RIGHT_LOOP, 
																										 PBL_SIG_RIGHT_LOOP, PBL_SIG_RIGHT_LOOP);
	group->setBehavior(triggerBeh);
	group->setUserProperty(PBL_GROUP1);
	group->setProperty(EM_GROUP_TRANSFORM_ONCE);

	Group * groupL = loadStdLight(file, engine, triggerBeh);
	Animation * anim = loadStdAnimation(file, engine);
	if (groupL != NULL) groupL->setAnimation(anim);
	group->add(groupL);	
  		
	cerr << "loaded right loop" << endl;
}

/* Light
 */
void loadExtra_light(ifstream & file, Engine * engine) {
	int active, unactive, signal;
	file >> active; 
	file >> unactive; 
	file >> signal;
	TriggerBehavior* triggerBeh = new TriggerBehavior( active, unactive, signal );

	Group * group = loadStdLight(file, engine, triggerBeh);
	Animation * anim = loadStdAnimation(file, engine);
	if (group != NULL) {
		group->setBehavior(triggerBeh);
		group->setUserProperty(PBL_GROUP1);
		group->setProperty(EM_GROUP_TRANSFORM_ONCE);

		group->setAnimation(anim);
		engine->add(group);	
  }

	cerr << "loaded extra light" << endl;
}

/* Main load file function
 */
int loadFile(const char * fn, Engine * engine) {
	ifstream file(fn);
	string str;
	if (!file) {
		cerr << "file not found" << endl;
		return -1;
	}
	cerr << "file found" << endl;

	while (file >> str) {
		if (str == "allend") return 0;
		if (str == "walls")      loadWalls(file, engine);
		if (str == "leftarm")    loadLeftarm(file, engine);
		if (str == "rightarm")   loadRightarm(file, engine);
		if (str == "bumper")     loadBumper(file, engine);
		if (str == "flatbumper") loadFlatbumper(file, engine);
		if (str == "target")   loadTarget(file, engine);
		if (str == "cave")       loadCave(file, engine);
		if (str == "lock_target") loadLock_target(file, engine);
		if (str == "lock")       loadLock(file, engine);
		if (str == "lock_release") loadLock_release(file, engine);
		if (str == "jackpot")    loadJackpot(file, engine);
		if (str == "left_loop")  loadLeft_loop(file, engine);
		if (str == "right_loop") loadRight_loop(file, engine);
		if (str == "extra_light") loadExtra_light(file, engine);
		while (file >> str && str != "end") {
			cerr << "Ignored " << str <<endl;
		}
  }
	return 0;
}
