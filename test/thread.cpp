/***************************************************************************
 Threaded rendering test. A cube should be visible in the middle of the screen.
 The arrow keys rotates the cube.
 ***************************************************************************/

#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyRotBehavior.h"
#include "Keyboard.h"
#include "EMath.h"

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
	groupCamera->setTranslation(0, 0, 3);
	engine->setEngineCamera(groupCamera);
	// Add a cube.
	Cube* cube = new Cube(1.0, 1.0, 1.0, 0.0, 1.0);
	Group* groupCube = new Group();
	engine->add(groupCube);
	groupCube->addShape3D(cube);
	
	// Add a behavior to the cube
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	groupCube->addBehavior(keyRBeh);
		
	engine->startRenderThread();

	int start = SDL_GetTicks();
	int desired = start;
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		desired += 50

		int ticks = SDL_GetTicks();
		int old_tick = ticks;
		ticks = SDL_GetTicks();
		int delay = 50 + old_tick - ticks;
		SDL_Delay(EM_MAX(delay, 0));
		
	}
	engine->stopRenderThread();

	delete(engine);
	return 0;
}
