/***************************************************************************
 Joystick test
 ***************************************************************************/

#include "Engine.h"
#include "Keyboard.h"
#include "SDL/SDL.h"

/**
 * Main
 */
int main(int argc, char *argv[]) {
	cerr << "Joystick test" << endl;
	Engine* engine = new Engine(argc, argv);
	
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		cerr << "Couldn't initialize joystick: " << SDL_GetError() << endl;
		exit(1);
	}
	
	int njoystick = SDL_NumJoysticks();
	cerr << njoystick << " joysticks were found." << endl;
	
	if (njoystick != 0) {
		cerr << "The names of the joysticks are:" << endl;
		for(int a=0; a<njoystick; a++ ) {
			cerr << "  " << SDL_JoystickName(a) << endl;
		}
	}

	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		engine->render();
		engine->swap();
	}	

	delete(engine);
	return 0;
}
