/***************************************************************************
 Texture test. A cube should be visible in the middle of the screen.
 The cube has a texture applied to it.
 The arrow keys rotates the cube.
 ***************************************************************************/

#include <iostream>

#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyRotBehavior.h"
#include "KeyBehavior.h"
#include "Keyboard.h"
#include "TextureUtil.h"
#include "Light.h"
#include "Polygon.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "Simple emilia test." << endl;

	if (argc < 2) {
		cerr << "Usage: texture <image-file>" << endl;
		return 0;
	}

	// Create the engine.
	Engine* engine = new Engine(argc, argv);
	engine->setLightning(0.5f, 0.1f);

	// Add a camera. Move a bit.
	Camera* camera = new Camera();
	Group* groupCamera = new Group();
	engine->add(groupCamera);
	groupCamera->setCamera(camera);
	groupCamera->setTranslation(0, 0, 3);
	engine->setEngineCamera(groupCamera);

	// Load a texture
	EmTexture* texture = TextureUtil::getInstance()->loadTexture(argv[1]);
	if (texture == NULL) {
		cerr << "Error loading texture" << endl;
		return -1;
	}

	// Add a cube.
	Cube* cube1 = new Cube(1.0, texture);
	Cube* cube2 = new Cube(1.0, texture);
	Group* groupCube1 = new Group();
	Group* groupCube2 = new Group();
	engine->add(groupCube1);
	engine->add(groupCube2);
	groupCube1->addShape3D(cube1);
	groupCube2->addShape3D(cube2);
	cube1->setPolygonProperty(EM_POLY_TRANS);
	cube1->setProperty(EM_SHAPE3D_USE_TRANS);
	
	// Add a light
	Light* lightR = new Light(1, 0, 0,  1, 0, 0);
	Group* groupLightR = new Group();
	engine->add(groupLightR);
	groupLightR->setLight(lightR);
	groupLightR->setTranslation(5, 0, 0);
	engine->addLight(lightR);

	// Add behaviors to the cubes
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	groupCube1->setBehavior(keyRBeh);
	KeyBehavior* keyBeh = new KeyBehavior();
	groupCube2->setBehavior(keyBeh);

	engine->resetTick();
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
	  if (engine->nextTick()) {
	    engine->tick();
	  } else {
	    engine->render();
	    engine->swap();
	  }
	}
	engine->stopEngine();
	return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
