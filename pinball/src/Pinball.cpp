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

#include "Loader.h"

/**
 * Main
 */
int main(int argc, char *argv[]) {
	cout << "pinball" << endl;
	//	srandom(time(0));

	// Create a engine
	Engine* oEngine = new Engine(argc, argv);
	oEngine->setLightning(0.3f, 0.1f);

	// Add a score board and a menu.
	Meny* menu = createMenus(oEngine);
	Score* oScore = new Score();
	oEngine->setBehavior(oScore);
	
	// Add a camera.
	Group* groupCR = new Group();
	Group* groupCT = new Group();
	Camera* camera = new Camera();
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	KeyBehavior* keyBeh = new KeyBehavior();
		
	oEngine->add(groupCT);
	groupCT->setBehavior(keyBeh);
 	groupCT->setTransform(0, 25, 25, 0, 0, 0);
	groupCT->add(groupCR);
	//	groupCR->setBehavior(keyRBeh);
 	groupCR->setTransform(0.0f, 0.0f, 0.0f, 0.125f, 0, 0);
	groupCR->setCamera(camera);

	oEngine->setEngineCamera(groupCR);

	// Load from file
	loadFile("data/pinballtmp.pbl", oEngine);

	// Add pinball floor
	Group* groupG = new Group();
	EmTexture* tex = TextureUtil::loadTexture("data/floor2.jpg");
	Shape3D* gs = new Grid(tex, 80.0f, 40.0f, 16, 1.0f/16.0f, 1, 1, 1, 1);

	oEngine->add(groupG);
	groupG->addShape3D(gs);
	groupG->setTransform(0.0f, -1.0f, 0.0f, 0.0f, 0.25f, 0.0f);
	groupG->setProperty(EM_GROUP_TRANSFORM_ONCE);

		// A shadow
#define SHADOW(shadow) \
  }

#define BALL(c_r, c_g, c_b, pbl, x)    \
{                                      \
  Group* groupS = new Group();         \
                                       \
  Shape3D* ballCyl = new BigSphere(1, 0, c_r, c_g, c_b, 1);  \
  Shape3D* ballSphere = new BigSphere(1, 2, c_r, c_g, c_b, 1); \
  ballSphere->setProperty(EM_SPECULAR);                      \
  ballSphere->setPolygonProperty(EM_POLY_HIDDEN);            \
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
  shadow->add(1,-0.99,1);              \
  shadow->add(-1,-0.99,1);             \
  shadow->add(-1,-0.99,-1);            \
  shadow->add(1,-0.99,-1);             \
  shadow->add(0,-0.99,0);              \
  Polygon* poly = new Polygon(shadow, 3);   \
  poly->setProperty(EM_POLY_TRANS);    \
  poly->add(0, 0,0, 0.5,0,0,0);        \
  poly->add(1, 0,0, 0.5,0,0,0);        \
  poly->add(2, 0,0, 0.5,0,0,0);        \
  poly->add(3, 0,0, 0.5,0,0,0);        \
  poly->add(4, 0,0, 0.5,0,0,0);        \
  poly->add(5, 0,0, 0.5,0,0,0);        \
  poly->add(6, 0,0, 0.5,0,0,0);        \
  poly->add(7, 0,0, 0.5,0,0,0);        \
  shadow->add(poly);                   \
  shadow->countNormals();              \
                                       \
  BounceBehavior* bouBeh = new BounceBehavior(1);            \
                                       \
  oEngine->add(groupS);              \
  groupS->setUserProperty(pbl);                       \
  groupS->setCollisionBounds(ballBounds);                    \
  groupS->addShape3D(ballSphere);    \
  groupS->addShape3D(ballCyl);       \
  groupS->addShape3D(shadow);        \
  groupS->setBehavior(bouBeh);       \
  groupS->setTransform(x, 0, 8, 0, 0, 0);                    \
}

	BALL(1, 0, 0, PBL_BALL_1, 4);

	BALL(0, 1, 0, PBL_BALL_2, 0);

	BALL(0, 0, 1, PBL_BALL_3, -4);


	// Reset pinball
//	SignalVisitor::add(PBL_SIG_RESET_ALL, oEngine);
	oEngine->sendSignal(PBL_SIG_RESET_ALL, oEngine);
		
	// Draw to the screen.
	int exit = 0;
	extern int iSeconds;
	int old_sec = iSeconds;
	int render = 0;
//	for (int a=0; a<1000; a++) {	
	while (exit == 0) {
		// time has elapsed
		if (iSeconds - old_sec >= 1) {
			old_sec++;
			oEngine->tick();
			render++;		

			if (Keyboard::isKeyDown(SDLK_r)) {
				oEngine->sendSignal(PBL_SIG_RESET_ALL, oEngine);
			}
			if (Keyboard::isKeyDown(SDLK_ESCAPE)) {
				if (menu->start() == EM_MENU_EXIT) exit = 1;
				old_sec = iSeconds;
			}
			if (Keyboard::isKeyDown(SDLK_p)) {
				Keyboard::waitForKey();
				Keyboard::clear();
				old_sec = iSeconds;
			}
		} else if ( render > 1) {
			//oEngine->tick();
			oEngine->render();
			oScore->draw();
			oEngine->swap();
			render = 0;
		}

		if (render > 100) {
			cerr << "YOUR MACHINE IS TOO SLOW, BUY NEW HARDWARE!!!" << endl;
			render = 0;
		}
		
	}

	delete(oEngine);
	delete(menu);
	return 0;
}
