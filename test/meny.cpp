/***************************************************************************
 Simple meny test. A cube should be visible in the middle of the screen.
 The arrow keys rotates the cube.
 ***************************************************************************/

#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyRotBehavior.h"
#include "Keyboard.h"
#include "Meny.h"
#include "EmFont.h"
#include "Config.h"

/** Main */

void sndFunc(Meny * m) {
  if (Config::getInstance()->getSound()) {
		Config::getInstance()->setSound(false);
		m->setName("sound off");
	} else {
		Config::getInstance()->setSound(true);
		m->setName("sound on");
	}

	cerr << "sound" << endl;
}

void grxFunc(Meny * m) {
  if (Config::getInstance()->getFullScreen()) {
		Config::getInstance()->setFullScreen(false);
		m->setName("graphics windowed");
	} else {
		Config::getInstance()->setFullScreen(true);
		m->setName("graphics fullscreen");
	}

	cerr << "graphics" << endl;
}	

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
	
	// Load a font
	EmFont* font = EmFont::getInstance();
	font->loadFont("test/font_16.pcx");
	
	// Create the meny
	Meny* meny = new Meny("meny", engine);

	Meny* menyresume = new Meny("resume", engine);
	menyresume->setAction(EM_MENU_RESUME);
	meny->addMeny(menyresume);

	Meny* menycfg = new Meny("config", engine);
	menycfg->setAction(EM_MENU_SUB);
	meny->addMeny(menycfg);

	Meny* menyexit = new Meny("exit", engine);
	menyexit->setAction(EM_MENU_EXIT);
	meny->addMeny(menyexit);

	Meny* menysnd = new Meny("sound off", engine);
	if (Config::getInstance()->getSound()) {
		menysnd->setName("sound on");
	}
	menysnd->setAction(EM_MENU_SUB);
	menysnd->setFunction(sndFunc);
	menycfg->addMeny(menysnd);

	Meny* menygrx = new Meny("graphics windowed", engine);
	if (Config::getInstance()->getFullScreen()) {
		menygrx->setName("graphics fullscreen");
	}
	menygrx->setAction(EM_MENU_SUB);
	menygrx->setFunction(grxFunc);
	menycfg->addMeny(menygrx);

	Meny* menyctrl = new Meny("controls", engine);
	menyctrl->setAction(EM_MENU_SUB);
	menycfg->addMeny(menyctrl);

	Meny* menyback = new Meny("back", engine);
	menyback->setAction(EM_MENU_BACK);
	menycfg->addMeny(menyback);

	bool done = false;
	while (!Keyboard::isKeyDown(SDLK_ESCAPE) && !done) {
		engine->tick();
		engine->render();
		engine->swap();

		if (Keyboard::isKeyDown(SDLK_SPACE)) {
			if (meny->start() == EM_MENU_EXIT) done = true;
		}

		SDL_Delay(50);
	}
	delete(engine);
	return 0;
}
