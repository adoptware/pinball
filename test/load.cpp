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
	engine->setClearColor(1.0f, 1.0f, 1.0f, 0.0f);

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
	bool keydown = false;
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
	  if (engine->nextTick()) {
	    engine->tick();
			Group * g = engine->getGroup(1);
			if (g != NULL) {
				Shape3D * s = NULL;
				
				bool skip = false;
				if (keydown) skip = true;
				if (Keyboard::isKeyDown(SDLK_1)) { s = g->getShape3D(0); keydown = true; }
				else if (Keyboard::isKeyDown(SDLK_2)) { s = g->getShape3D(1); keydown = true; }
				else if (Keyboard::isKeyDown(SDLK_3)) { s = g->getShape3D(2); keydown = true; }
				else if (Keyboard::isKeyDown(SDLK_4)) { s = g->getShape3D(3); keydown = true; }
				else if (Keyboard::isKeyDown(SDLK_5)) { s = g->getShape3D(4); keydown = true; }
				else if (Keyboard::isKeyDown(SDLK_6)) { s = g->getShape3D(5); keydown = true; }
				else if (Keyboard::isKeyDown(SDLK_7)) { s = g->getShape3D(6); keydown = true; }
				else if (Keyboard::isKeyDown(SDLK_8)) { s = g->getShape3D(7); keydown = true; }
				else if (Keyboard::isKeyDown(SDLK_9)) { s = g->getShape3D(8); keydown = true; }
				else if (Keyboard::isKeyDown(SDLK_0)) { s = g->getShape3D(9); keydown = true; }
				else keydown = false;
				
				if (s != NULL && !skip) {
					if (s->getProperties() & EM_SHAPE3D_HIDDEN) {
						s->unsetProperty(EM_SHAPE3D_HIDDEN);
					} else {
						s->setProperty(EM_SHAPE3D_HIDDEN);
					}
				}
			}
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
