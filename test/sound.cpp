/***************************************************************************
 Sound test.
 ***************************************************************************/

#include "Engine.h"
#include "Camera.h"
#include "Keyboard.h"
#include "SoundUtil.h"

/**
 * Main
 */
int main(int argc, char *argv[]) {
	cerr << "Simple emilia test." << endl;

	if (argc < 2) {
		cerr << "Usage: sound <wave-file>>" << endl;
		return 0;
	}

	// Create the engine.
	Engine* engine = new Engine(argc, argv);

	// Load waves
	EmSound wave = SoundUtil::loadSample(argv[1]);

	SoundUtil::play(wave, true);

	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
	}

	delete(engine);
	return 0;
}
