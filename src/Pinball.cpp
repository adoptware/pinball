/***************************************************************************
                            -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Henrik Enqvist
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
#include "Meny.h"
#include "Engine.h"
#include "Camera.h"
#include "Light.h"
#include "BigSphere.h"
#include "Cylinder.h"
#include "BounceBehavior.h"
#include "KeyBehavior.h"
#include "Grid.h"
#include "KeyRotBehavior.h"
#include "Polygon.h"
#include "TextureUtil.h"
#include "Score.h"
#include "CollisionBounds.h"
#include "StateMachine.h"
#include "Shape3DUtil.h"
#include "EyeBehavior.h"
#include "Config.h"

#include "Loader.h"

/** Main */
int main(int argc, char *argv[]) {
	cout << "pinball" << endl;
	char filename[256];
	//	srandom(time(0));
	
	// Load config from .emilia/pinball
	Config::getInstance()->loadConfig();

	// Create a engine and remove emilia arguments
	Engine* engine = new Engine(argc, argv);
	engine->setLightning(0.0f, 0.1f);

	// Add a score board and a menu.
	Meny* menu = createMenus(engine);
	Score* score = Score::getInstance();
	engine->addBehavior(score);
	
	// Add a camera.
	Group* groupCR = new Group();
	Group* groupCT = new Group();
	Camera* camera = new Camera();
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	KeyBehavior* keyBeh = new KeyBehavior();
		
	engine->add(groupCT);

	// 	groupCT->addBehavior(keyBeh);
	//	groupCT->setTransform(0, 25, 25, 0, 0, 0);
	groupCT->add(groupCR);
	// 	groupCR->addBehavior(keyRBeh);
	//	groupCR->setTransform(0.0f, 0.0f, 0.0f, 0.175f, 0, 0);
	groupCR->setCamera(camera);

	Light* cl = new Light(1, 0.05f, 0, 1,1,1);
	//engine->setLight(cl);
	groupCR->setLight(cl);
	engine->addLight(cl);

	engine->setEngineCamera(groupCR);

	// Load from file
	sprintf(filename, "%s/pinballnew.pbl", Config::getInstance()->getDataDir());
	if (loadFile(filename, engine) != 0) {
		cerr << "Error loading level" << endl;
		cerr << "Try reinstalling the game or use the -data switch to specify the data directory" << endl;
		return -1;
	}

	// Add pinball floor
	Group* groupG = new Group();
	sprintf(filename, "%s/floor2.png", Config::getInstance()->getDataDir());
	EmTexture* tex = TextureUtil::getInstance()->loadTexture(filename);
	Shape3D* gs = new Grid(tex, 80.0f, 40.0f, 16, 1.0f/16.0f, 1, 1, 1, 1);

	engine->add(groupG);
	groupG->addShape3D(gs);
	groupG->setTransform(0.0f, -1.0f, 0.0f, 0.0f, 0.25f, 0.0f);
	groupG->setProperty(EM_GROUP_TRANSFORM_ONCE);


#define BALL(c_r, c_g, c_b, pbl, x, group)    \
{                                             \
  /*Shape3D* ballCyl = new BigSphere(1, 1, c_r, c_g, c_b, 1);*/    \
	sprintf(filename, "%s/ball_co.emi", Config::getInstance()->getDataDir()); \
  Shape3D* ballCyl = Shape3DUtil::loadShape3D(filename); \
  Shape3D* ballSphere = new BigSphere(1, 2, c_r, c_g, c_b, 1);     \
  ballSphere->setProperty(EM_SHAPE3D_SPECULAR);                    \
  ballCyl->setProperty(EM_SHAPE3D_HIDDEN);                      \
  CollisionBounds* ballBounds = new CollisionBounds(ballCyl->getCollisionSize()); \
  ballBounds->setShape3D(ballCyl, 0);  \
                                       \
  Shape3D* shadow = new Shape3D(8, 1); \
	                                     \
  shadow->add(0,     -0.95, 1);        \
  shadow->add(-0.707,-0.95, 0.707);    \
  shadow->add(-1,    -0.95, 0);        \
  shadow->add(-0.707,-0.95, -0.707);   \
  shadow->add(0,		 -0.95, -1);       \
  shadow->add(0.707, -0.95, -0.707);   \
  shadow->add(1,     -0.95, 0);        \
  shadow->add(0.707, -0.95, 0.707);    \
                                       \
  shadow->add(1,-0.98,1);              \
  shadow->add(-1,-0.98,1);             \
  shadow->add(-1,-0.98,-1);            \
  shadow->add(1,-0.98,-1);             \
  shadow->add(0,-0.98,0);              \
  Polygon* poly = new Polygon(shadow, 3);   \
  poly->add(0, 0,0, 0,0,0,0.5);        \
  poly->add(1, 0,0, 0,0,0,0.5);        \
  poly->add(2, 0,0, 0,0,0,0.5);        \
  poly->add(3, 0,0, 0,0,0,0.5);        \
  poly->add(4, 0,0, 0,0,0,0.5);        \
  poly->add(5, 0,0, 0,0,0,0.5);        \
  poly->add(6, 0,0, 0,0,0,0.5);        \
  poly->add(7, 0,0, 0,0,0,0.5);        \
  poly->setProperty(EM_POLY_TRANS);    \
  shadow->add(poly);                   \
  shadow->setProperty(EM_SHAPE3D_USE_TRANS);         \
  shadow->countNormals();              \
                                       \
  BounceBehavior* bouBeh = new BounceBehavior(pbl);  \
                                       \
  engine->add(group);                  \
  group->setUserProperty(pbl);         \
  group->setCollisionBounds(ballBounds);    \
  group->addShape3D(ballCyl);          \
  group->addShape3D(ballSphere);       \
  group->addShape3D(shadow);           \
  group->addBehavior(bouBeh);          \
  group->setTransform(x, 0, 8, 0, 0, 0);   \
}

	Group* gb1 = new Group();
  BALL(1, 0, 0, PBL_BALL_1, 4, gb1);

	Group* gb2 = new Group();
	BALL(0, 1, 0, PBL_BALL_2, 0, gb2);

	Group* gb3 = new Group();
	BALL(0, 0, 1, PBL_BALL_3, -4, gb3);
	/*
	BALL(1, 0, 1, PBL_BALL_4, -8);
	*/

	groupCT->addBehavior(new EyeBehavior(gb1, gb2, gb3));

	// Reset pinball
	SendSignal(PBL_SIG_RESET_ALL, 0, engine, NULL);
		
	// Draw to the screen.
  int exit = 0;
	bool render = true;
	int skip = 0;
	int all = 0;
	while (exit == 0) {
		engine->tick();
		engine->tick();
		//		engine->tick();
		if (render) {
			engine->render();
			score->draw();
			engine->swap();
		} else {
			skip++;
		}
		all++;
		if (Keyboard::isKeyDown(SDLK_r)) {
			SendSignal(PBL_SIG_RESET_ALL, 0, engine, NULL);
		}
		if (Keyboard::isKeyDown(SDLK_ESCAPE)) {
			if (menu->start() == EM_MENU_EXIT) exit = 1;
		}
		if (Keyboard::isKeyDown(SDLK_p)) {
			Keyboard::waitForKey();
			Keyboard::clear();
		}
		render = engine->limitFPS(60);
	}

	extern float em_groups_m, em_shapes_m, em_bounds_m, em_polygons_m;

#if EM_DEBUG
	cerr << "Groups " << em_groups_m << endl;
	cerr << "Shapes " << em_shapes_m << endl;
	cerr << "Bounds " << em_bounds_m << endl;
	cerr << "Polys " << em_polygons_m << endl;

	cerr << "Skip " << skip  << " of " << all << endl;
#endif

	Config::getInstance()->saveConfig();

	delete(engine);
	delete(menu);
	return 0;
}
