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

#include "Loader.h"

/** Main */
int main(int argc, char *argv[]) {
	cout << "pinball" << endl;
	//	srandom(time(0));

	// Create a engine
	Engine* engine = new Engine(argc, argv);
	engine->setLightning(0.3f, 0.1f);

	// Add a score board and a menu.
	Meny* menu = createMenus(engine);
	Score* score = new Score();
	engine->addBehavior(score);
	
	// Add a camera.
	Group* groupCR = new Group();
	Group* groupCT = new Group();
	Camera* camera = new Camera();
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	KeyBehavior* keyBeh = new KeyBehavior();
		
	engine->add(groupCT);
	groupCT->addBehavior(keyBeh);
 	groupCT->setTransform(0, 25, 25, 0, 0, 0);
	groupCT->add(groupCR);
	groupCR->addBehavior(keyRBeh);
 	groupCR->setTransform(0.0f, 0.0f, 0.0f, 0.125f, 0, 0);
	groupCR->setCamera(camera);

	engine->setEngineCamera(groupCR);

	// Load from file
	loadFile("data/pinballtmp.pbl", engine);

	// Add pinball floor
	Group* groupG = new Group();
	EmTexture* tex = TextureUtil::loadTexture("data/floor2.jpg");
	Shape3D* gs = new Grid(tex, 80.0f, 40.0f, 16, 1.0f/16.0f, 1, 1, 1, 1);

	engine->add(groupG);
	groupG->addShape3D(gs);
	groupG->setTransform(0.0f, -1.0f, 0.0f, 0.0f, 0.25f, 0.0f);
	groupG->setProperty(EM_GROUP_TRANSFORM_ONCE);


#define BALL(c_r, c_g, c_b, pbl, x)    \
{                                      \
  Group* groupS = new Group();         \
                                       \
  Shape3D* ballCyl = new BigSphere(1, 1, c_r, c_g, c_b, 1);    \
  Shape3D* ballSphere = new BigSphere(1, 2, c_r, c_g, c_b, 1); \
  ballSphere->setProperty(EM_SPECULAR);                        \
  ballCyl->setProperty(EM_SHAPE3D_HIDDEN);                     \
  CollisionBounds* ballBounds = new CollisionBounds(ballCyl->getCollisionSize()); \
  ballBounds->setShape3D(ballCyl, 1);  \
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
  shadow->add(1,-0.99,1);              \
  shadow->add(-1,-0.99,1);             \
  shadow->add(-1,-0.99,-1);            \
  shadow->add(1,-0.99,-1);             \
  shadow->add(0,-0.99,0);              \
  Polygon* poly = new Polygon(shadow, 3);   \
  poly->add(0, 0,0, 0.5,0,0,0);        \
  poly->add(1, 0,0, 0.5,0,0,0);        \
  poly->add(2, 0,0, 0.5,0,0,0);        \
  poly->add(3, 0,0, 0.5,0,0,0);        \
  poly->add(4, 0,0, 0.5,0,0,0);        \
  poly->add(5, 0,0, 0.5,0,0,0);        \
  poly->add(6, 0,0, 0.5,0,0,0);        \
  poly->add(7, 0,0, 0.5,0,0,0);        \
  shadow->add(poly);                   \
  shadow->setProperty(EM_SHAPE3D_TRANS);    \
  shadow->countNormals();              \
                                       \
  BounceBehavior* bouBeh = new BounceBehavior(pbl);  \
                                       \
  engine->add(groupS);                \
  groupS->setUserProperty(pbl);             \
  groupS->setCollisionBounds(ballBounds);   \
  groupS->addShape3D(ballSphere);      \
  groupS->addShape3D(ballCyl);         \
  groupS->addShape3D(shadow);          \
  groupS->addBehavior(bouBeh);         \
  groupS->setTransform(x, 0, 8, 0, 0, 0);   \
}

  BALL(1, 0, 0, PBL_BALL_1, 4);
	/*
	BALL(0, 1, 0, PBL_BALL_2, 0);

	BALL(0, 0, 1, PBL_BALL_3, -4);

	BALL(1, 0, 1, PBL_BALL_4, -8);
	*/

	// Reset pinball
//	SignalVisitor::add(PBL_SIG_RESET_ALL, engine);
	SendSignal(PBL_SIG_RESET_ALL, 0, engine, NULL);
		
	// Draw to the screen.
  int exit = 0;
	
	while (exit == 0) {
		engine->tick();
		engine->tick();
		engine->render();
		engine->swap();
		if (Keyboard::isKeyDown(SDLK_r)) {
			SendSignal(PBL_SIG_RESET_ALL, 0, engine, NULL);
		}
		if (Keyboard::isKeyDown(SDLK_ESCAPE)) {
			engine->pauseRenderThread();
			if (menu->start() == EM_MENU_EXIT) exit = 1;
			engine->resumeRenderThread();
		}
		if (Keyboard::isKeyDown(SDLK_p)) {
			Keyboard::waitForKey();
			Keyboard::clear();
		}
		engine->limitFPS(25);
	}

	extern float em_groups_m, em_shapes_m, em_bounds_m, em_polygons_m;

	cerr << "Groups " << em_groups_m << endl;
	cerr << "Shapes " << em_shapes_m << endl;
	cerr << "Bounds " << em_bounds_m << endl;
	cerr << "Polys " << em_polygons_m << endl;


	delete(engine);
	delete(menu);
	return 0;
}
