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
	groupCamera->setTranslation(0, 0, 3);
	engine->setEngineCamera(groupCamera);
	// Add a cube.
	//Cube* cube = new Cube(1.0, 1.0, 1.0, 0.0, 1.0);
	Shape3D * cube = new Cylinder(1.0f, 32, 1.0f, 1.0f, 0.0f, 1.0f);
	Group* groupCube = new Group();
	engine->add(groupCube);
	groupCube->addShape3D(cube);

	// Add a behavior to the cube
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	groupCube->setBehavior(keyRBeh);
		
	bool render = true;
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		if (render) {
			engine->render();
			engine->swap();
		}
		//render = engine->limitFPS(100);
	}
	delete(engine);
	return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
