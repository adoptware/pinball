/***************************************************************************
 Texture test. A cube should be visible in the middle of the screen.
 The cube has a texture applied to it.
 The arrow keys rotates the cube.
 ***************************************************************************/

#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyRotBehavior.h"
#include "Keyboard.h"
#include "TextureUtil.h"
#include "Light.h"

/**
 * Main
 */
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
	EmTexture* texture = TextureUtil::loadTexture(argv[1]);

	// Add a cube.
	Cube* cube = new Cube(1.0, texture);
	Group* groupCube = new Group();
	engine->add(groupCube);
	groupCube->addShape3D(cube);
	
	// Add a light
	Light* lightR = new Light(1, 0, 0,  1, 0, 0);
	Group* groupLightR = new Group();
	engine->add(groupLightR);
	groupLightR->setLight(lightR);
	groupLightR->setTranslation(5, 0, 0);
	engine->addLight(lightR);

	// Add a behavior to the cube
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	groupCube->addBehavior(keyRBeh);
		
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		engine->render();
		engine->swap();
		//		SDL_Delay(50);
	}
	delete(engine);
	return 0;
}
