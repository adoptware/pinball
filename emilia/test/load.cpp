/***************************************************************************
 Simple test. Load a shape from an emi-file.
 The arrow keys rotates the object.
 ***************************************************************************/

#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyBehavior.h"
#include "KeyRotBehavior.h"
#include "Keyboard.h"
#include "Shape3DUtil.h"
#include "SDL/SDL.h"

/**
 * Main
 */
int main(int argc, char *argv[]) {
	cerr << "Simple emilia test." << endl;
	
	if (argc < 2) {
		cerr << "Usage: load <emi-file>" << endl;
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
	groupCamera->setTranslation(0, 0, 10);
	engine->setEngineCamera(groupCamera);

	// Add a behavior to the camera
	KeyBehavior* keyBeh = new KeyBehavior();
	groupCamera->addBehavior(keyBeh);

	// Load the shape.
	Shape3D* shape = Shape3DUtil::loadShape3D(argv[1]);
	Group* groupShape = new Group();
	engine->add(groupShape);
	groupShape->addShape3D(shape);
	
	// Add a behavior to the shape
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	groupShape->addBehavior(keyRBeh);
		
	while (!Keyboard::isKeyDown(KEY_ESC)) {
		engine->tick();
		engine->render();
		engine->swap();
		SDL_Delay(20);
	}
	delete(engine);
	return 0;
}
