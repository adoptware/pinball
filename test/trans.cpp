/***************************************************************************
 Transparency test. 
 ***************************************************************************/

#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyRotBehavior.h"
#include "KeyBehavior.h"
#include "Keyboard.h"
#include "Light.h"
#include "Polygon.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "Simple emilia test." << endl;

	// create the engine.
	Engine* engine = new Engine(argc, argv);
	engine->setLightning(0.5f, 0.1f);

	// add a camera. Move a bit.
	Camera* camera = new Camera();
	Group* groupCamera = new Group();
	engine->add(groupCamera);
	groupCamera->setCamera(camera);
	groupCamera->setTranslation(0, 0, 3);
	engine->setEngineCamera(groupCamera);

	// add cubes
	Cube* cube1 = new Cube(1.0f, 1.0f, 0.0f, 0.0f, 0.5f);
	Cube* cube2 = new Cube(1.0f, 0.0f, 0.0f, 1.0f, 0.5f);
	Group* groupCube1 = new Group();
	Group* groupCube2 = new Group();
	engine->add(groupCube1);
	engine->add(groupCube2);
	groupCube1->addShape3D(cube1);
	groupCube2->addShape3D(cube2);
	cube1->setPolygonProperty(EM_POLY_TRANS);
	cube1->setProperty(EM_SHAPE3D_USE_TRANS);
	cube2->setPolygonProperty(EM_POLY_TRANS);
	cube2->setProperty(EM_SHAPE3D_USE_TRANS);
	
	// add behaviors to the cubes
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	groupCube1->setBehavior(keyRBeh);
	KeyBehavior* keyBeh = new KeyBehavior();
	groupCube2->setBehavior(keyBeh);
		
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		engine->render();
		engine->swap();
		engine->limitFPS(50);
	}
	engine->stopEngine();
	return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
