/***************************************************************************
 Simple test. A cube should be visible in the middle of the screen.
 The arrow keys rotates the cube. 
 ***************************************************************************/

#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "Keyboard.h"
#include "KeyRotBehavior.h"
#include "KeyBehavior.h"
#include "Grid.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "Simple emilia test." << endl;

	// Create the engine.
	Engine* engine = new Engine(argc, argv);
	engine->setLightning(0.5f, 0.1f);

	// Add a camera. Add some animiation to the camera.
	Group* groupC1 = new Group();
	Group* groupC2 = new Group();
	Group* groupC3 = new Group();
	Group* groupC4 = new Group();

	engine->add(groupC1);
	groupC1->add(groupC2);
	groupC2->add(groupC3);
	groupC3->add(groupC4);

	Cube* cube2 = new Cube(1.0, 1, 1, 0, 1);
	groupC2->addShape3D(cube2);
	groupC2->setTranslation(0, 0, 2);

	Cube* cube3 = new Cube(1.0, 0, 1, 1, 1);
	groupC3->addShape3D(cube3);
	groupC3->setTranslation(0, 0, 2);

	Camera* camera = new Camera();
	groupC4->setCamera(camera);
	groupC4->setTranslation(0, 1, 2);
	engine->setEngineCamera(groupC4);

	Grid* grid = new Grid(NULL, 10, 10, 1, 0, 1, 1, 1, 1);
	engine->addShape3D(grid);

	Behavior* keyRBeh = new KeyRotBehavior();
	groupC1->addBehavior(keyRBeh);
	
	Behavior* keyBeh = new KeyBehavior();
	groupC3->addBehavior(keyBeh);

	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		engine->render();
		engine->swap();
		SDL_Delay(50);
	}
	delete(engine);
	return 0;
}
