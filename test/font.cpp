/***************************************************************************
 Font test. A cube should be visible in the middle of the screen.
 Some text applied on the screen.
 The arrow keys rotates the cube.
 ***************************************************************************/

#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyRotBehavior.h"
#include "Keyboard.h"
#include "EmFont.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "Font test." << endl;

	if (argc < 2) {
		cerr << "Usage: font <fontimage>" << endl;
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

	// Add a cube.
	Cube* cube = new Cube(1.0, 1,1,0,1);
	Group* groupCube = new Group();
	engine->add(groupCube);
	groupCube->addShape3D(cube);
	
	// Add a behavior to the cube
	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	groupCube->setBehavior(keyRBeh);
		
	// Aquire the singleton font object.
	EmFont* font = EmFont::getInstance();
	font->loadFont(argv[1]);

	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		engine->render();
		font->printRow("the brown fox jumped over",  0);
		font->printRow("the lazy dog :;<=>?@ {}", 1);
		font->printRow("1234567890 !\"#$%&'()*+,-./", 2);
		font->printRow("press esc to quit", 3);
		engine->swap();
	}
	delete(engine);
	return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
