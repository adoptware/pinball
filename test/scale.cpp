/***************************************************************************
 Simple test. A cube should be visible in the middle of the screen.
 The arrow keys rotates the cube.
 ***************************************************************************/

#include <iostream>

#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyRotBehavior.h"
#include "Keyboard.h"
#include "Cylinder.h"
#include "BigSphere.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "Simple emilia test." << endl;

	// Create the engine.
	Engine* engine = new Engine(argc, argv);
	engine->setLightning(0.7f, 0.1f);

	// Add a camera. Move a bit.
	Camera* camera = new Camera();
	Group* groupCamera = new Group();
	engine->add(groupCamera);
	groupCamera->setCamera(camera);
	groupCamera->setTranslation(0, 0, 6);
	engine->setEngineCamera(groupCamera);
	// Add a cube.
	Cube* cube = new Cube(1.0, 1.0, 1.0, 0.0, 1.0);
	Group* groupCube = new Group();
	engine->add(groupCube);
	groupCube->addShape3D(cube);
	// Add a cylinder
	Cylinder * cylinder = new Cylinder(1.0f, 32, 1.0f, 0.0f, 1.0f, 1.0f);
	Group* groupCylinder = new Group();
	engine->add(groupCylinder);
	groupCylinder->addShape3D(cylinder);
	// Add two spheres
	BigSphere * sphere1 = new BigSphere(0.5f, 2, 1.0f, 0.0f, 1.0f, 1.0f);
	BigSphere * sphere2 = new BigSphere(0.5f, 2, 1.0f, 0.0f, 1.0f, 1.0f);
	Group* groupSphere1 = new Group();
	Group* groupSphere2 = new Group();
	engine->add(groupSphere1);
  groupSphere1->add(groupSphere2);
  groupSphere1->setTranslation(1, 0, 0);
	groupSphere1->addShape3D(sphere1);
  groupSphere2->setTranslation(0, 1, 0);
  groupSphere2->addShape3D(sphere2);


	// Add a behavior to the cube
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	groupCube->setBehavior(keyRBeh);
		
  float x=0, y=0, z=0;
  float dx=-1, dy=-1, dz=-1;
  engine->resetTick();
  while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
    if (engine->nextTick()) {
      if (x > 2) dx=-0.012;
      if (y > 2) dy=-0.021;
      if (z > 2) dz=-0.027;
      if (x < 0) dx=0.012;
      if (y < -2) dy=0.021;
      if (z < 0) dz=0.027;
      
      x += dx;
      y += dy;
      z += dz;
      
      groupCylinder->setScale(x, y, z);
      groupSphere1->setScale(x, y, z);
      //cerr << "x " << x << " y " << y << " z " << z << endl;
      engine->tick(); 
    } else {
			engine->render();
			engine->swap();
		}
	}
	delete(engine);
	return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
