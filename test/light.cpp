/***************************************************************************
 Simple test. A cube should be visible in the middle of the screen.
 The arrow keys rotates the cube.
 ***************************************************************************/

#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "BigSphere.h"
#include "KeyRotBehavior.h"
#include "Keyboard.h"
#include "Light.h"
#include "Polygon.h"
#include "Shape3D.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "Simple emilia test." << endl;

	// Create the engine.
	Engine* engine = new Engine(argc, argv);
	engine->setLightning(0.5f, 0.0f);

	// Add a camera. Move it a bit.
	Camera* camera = new Camera();
	Group* groupCamera = new Group();
	engine->add(groupCamera);
	groupCamera->setCamera(camera);
	groupCamera->setTranslation(0, 0, 3);
	engine->setEngineCamera(groupCamera);
	// Add a sphere
	BigSphere* sphere = new BigSphere(1, 3, 1, 1, 1, 1);
	sphere->setProperty(EM_SHAPE3D_SPECULAR);
	Group* groupSphere = new Group();
	engine->add(groupSphere);
	groupSphere->addShape3D(sphere);
	// Add a light
	BigSphere* li = new BigSphere(1, 0, 1, 1, 1, 1);
	li->setProperty(EM_SHAPE3D_ALLWAYSLIT);
	Light* lightR = new Light(2, 0, 0,  1, 0, 0);
	Group* groupLightR1 = new Group();
	Group* groupLightR2 = new Group();
	engine->add(groupLightR1);
	groupLightR1->add(groupLightR2);
	groupLightR2->addShape3D(li);
	groupLightR2->setLight(lightR);
	groupLightR2->setTranslation(10, 0, 0);
	engine->addLight(lightR);

	
	// Add a behavior to the light
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	groupLightR1->setBehavior(keyRBeh);
		
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		engine->render();
		engine->swap();
		//		SDL_Delay(50);
	}
	engine->stopEngine();
	return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
