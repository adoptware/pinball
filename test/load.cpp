/***************************************************************************
 Simple test. Load a shape from an emi-file.
 The arrow keys rotates the object.
 ***************************************************************************/

#include <iostream>

#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyBehavior.h"
#include "KeyRotBehavior.h"
#include "Keyboard.h"
#include "Loader.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "Simple emilia test." << endl;
	
	if (argc < 2) {
		cerr << "Usage: load <emi-file>" << endl;
		return 0;
	}

	// create the engine.
	Engine* engine = new Engine(argc, argv);
	engine->setLightning(0.5f, 0.1f);
	engine->setClearColor(0.1f, 0.0f, 0.3f, 0.0f);

	// add a camera. Move a bit.
	Camera* camera = new Camera();
	Group* groupCamera = new Group();
	engine->add(groupCamera);
	groupCamera->setCamera(camera);
	groupCamera->setTranslation(0, 0, 10);
	engine->setEngineCamera(groupCamera);

	// add a behavior to the camera
	KeyBehavior* keyBeh = new KeyBehavior();
	groupCamera->setBehavior(keyBeh);

	// load the file
	if (Loader::getInstance()->loadFile(argv[1], engine) < 0) {
		cerr << "error loading file " << argv[1] << endl;
		return 0;
	}
	// add a behavior to the shape
	Group * g = engine->getGroup(1);
	if (g != NULL) {
		KeyRotBehavior* keyRBeh = new KeyRotBehavior();
		g->setBehavior(keyRBeh);
	}
		
	engine->resetTick();
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
	  if (engine->nextTickFPS(100)) {
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
