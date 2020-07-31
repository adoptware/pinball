/***************************************************************************
 Simple test. A cube should be visible in the middle of the screen.
 The arrow keys rotates the cube. 
 ***************************************************************************/

#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "Keyboard.h"
#include "StdAnimation.h"
#include "Grid.h"

/**
 * Main
 */
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
	
	StdAnimation* anim1 = new StdAnimation(60, EM_ROTATION);
	anim1->add(0,0,0);
	anim1->add(0.25,0.5,0);
	anim1->setEnd(0,1,0);
	groupC1->addBehavior(anim1);
	
	StdAnimation* anim3 = new StdAnimation(40, EM_TRANSLATION);
	anim3->add(0, 0, 2);
	anim3->add(0, 0, 4);
	anim3->setEnd(0, 0, 2);
	groupC3->addBehavior(anim3);
	
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		engine->render();
		engine->swap();
	}
	delete(engine);
	return 0;
}
