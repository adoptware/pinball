/***************************************************************************
 Simple test. A grid should be visible in the middle of the screen.
 The arrow keys rotates the grid.
 ***************************************************************************/

#include "Engine.h"
#include "Camera.h"
#include "Grid.h"
#include "KeyRotBehavior.h"
#include "Keyboard.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "Simple emilia test." << endl;

	// Create the engine.
	Engine* engine = new Engine(argc, argv);
	engine->setLightning(0.5f, 0.1f);

	// Add a camera. Move a bit.
	Camera* camera = new Camera();
	Group* groupCamera = new Group();
	engine->add(groupCamera);
	groupCamera->setCamera(camera);
	groupCamera->setTranslation(0, 0, 100);
	engine->setEngineCamera(groupCamera);
	// Add a grid.
	Grid* grid = new Grid(NULL, 100.0f, 100.0f, 50, 0.01f,  1.0f, 1.0f, 0.0f, 1.0f);
	Group* groupGrid = new Group();
	engine->add(groupGrid);
	groupGrid->addShape3D(grid);
	groupGrid->setProperty(EM_GROUP_TRANSFORM_ONCE);
	
	// Add a behavior to the cube
	//KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	//groupGrid->setBehavior(keyRBeh);
	groupGrid->setTransform(0, 0, 0, -0.3f, 0.0f, 0.0f);
		
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
			engine->tick();
			engine->render();
			engine->swap();
	}
	delete(engine);
	return 0;
}
