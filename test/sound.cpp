/***************************************************************************
 Simple test. A cube should be visible in the middle of the screen.
 The arrow keys rotates the cube.
 ***************************************************************************/

#include <iostream>

#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyRotBehavior.h"
#include "Keyboard.h"
#include "SoundUtil.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "Sound test." << endl;

	if (argc < 3) {
		cerr << "Usage: sound <file.wav> <file.mid>" << endl;
		return -1;
	}
	// Create the engine.
	Engine* engine = new Engine(argc, argv);

	if (argc < 3) {
		cerr << "Usage: sound <file.wav> <file.mid>" << endl;
		return -1;
	}

	int sound = SoundUtil::getInstance()->loadSample(argv[1]);
	if (sound < 0) {
		cerr << "Sound not loaded" << endl;
		return -1;
	}

	int music = SoundUtil::getInstance()->loadMusic(argv[2]);
	if (music < 0) {
		cerr << "Sound not loaded" << endl;
		return -1;
	}
	SoundUtil::getInstance()->playMusic(music, true);

	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		if (Keyboard::isKeyDown(SDLK_SPACE)) {
			SoundUtil::getInstance()->playSample(sound, false);
		}
		engine->tick();
		engine->delay(20);
	}
	//delete(engine);
	return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
