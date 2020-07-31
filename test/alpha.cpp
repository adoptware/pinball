/***************************************************************************
 Simple test. A cube should be visible in the middle of the screen.
 The arrow keys rotates the cube.
 ***************************************************************************/

#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyRotBehavior.h"
#include "KeyBehavior.h"
#include "Keyboard.h"
#include "Polygon.h"

/**
 * Main
 */
int main(int argc, char *argv[]) {
	cerr << "Simple emilia test." << endl;

	// Create the engine.
	Engine* engine = new Engine(argc, argv);
	engine->setLightning(0.7f, 0.2f);

	// Add a camera. Move a bit.
	Camera* camera = new Camera();
	Group* groupCamera = new Group();
	engine->add(groupCamera);
	groupCamera->setCamera(camera);
	groupCamera->setTranslation(0, 0, 3);
	engine->setEngineCamera(groupCamera);
	// Add three cubes.
	Cube* cubeA = new Cube(1.0, 1.0, 1.0, 1.0, 1.0);
	Group* groupCubeA = new Group();
	engine->add(groupCubeA);
	groupCubeA->addShape3D(cubeA);
	groupCubeA->setTranslation(0, 0, -2);

	Cube* cubeB = new Cube(1.0, 1.0, 0.0, 0.0, 0.3);
	cubeB->setProperty(EM_SHAPE3D_TRANS);
	Group* groupCubeB = new Group();
	engine->add(groupCubeB);
	groupCubeB->addShape3D(cubeB);

	Cube* cubeC = new Cube(1.0, 0.0, 0.0, 1.0, 0.3);
	cubeC->setProperty(EM_SHAPE3D_TRANS);
	Group* groupCubeC = new Group();
	engine->add(groupCubeC);
	groupCubeC->addShape3D(cubeC);
	
	// Add a behavior to the cubes
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	groupCubeB->addBehavior(keyRBeh);

	KeyBehavior* keyBeh = new KeyBehavior();
	groupCubeA->addBehavior(keyBeh);
		
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		engine->render();
		engine->swap();
	}
	delete(engine);
	return 0;
}
