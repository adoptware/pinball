/***************************************************************************
 Simple meny test. A cube should be visible in the middle of the screen.
 The arrow keys rotates the cube.
***************************************************************************/

#include <iostream>

#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "KeyRotBehavior.h"
#include "Keyboard.h"
#include "Menu.h"
#include "EmFont.h"
#include "Config.h"

/** Main */

MenuChoose* menuchoose = NULL;

int fctApply(void) {
  cerr << menuchoose->getCurrent() << endl;
  return 0;
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
  groupCube->setBehavior(keyRBeh);
	
  // Load a font
  EmFont* font = EmFont::getInstance();
#if EM_USE_SDL
  font->loadFont("data/font_34.png");
#endif
#if EM_USE_ALLEGRO
  font->loadFont("data/font_35.pcx");
#endif
	
  // Create the menu
  MenuSub* menumain = new MenuSub("main", engine);

  MenuSub* menusub = new MenuSub("sub", engine);
  menumain->addMenuItem(menusub);

  MenuSub* menuresume = new MenuSub("resume", engine);
  menuresume->setAction(EM_MENU_RESUME);
  menumain->addMenuItem(menuresume);

  MenuSub* menuexit = new MenuSub("exit", engine);
  menuexit->setAction(EM_MENU_EXIT);
  menumain->addMenuItem(menuexit);

  menuchoose = new MenuChoose(engine);
  menuchoose->addText("first");
  menuchoose->addText("second");
  menuchoose->addText("third");
  menusub->addMenuItem(menuchoose);

  MenuFct* menuapply = new MenuFct("apply", fctApply, engine);
  menusub->addMenuItem(menuapply);

  MenuSub* menuback = new MenuSub("back", engine);
  menuback->setAction(EM_MENU_BACK);
  menusub->addMenuItem(menuback);
	
  bool done = false;
  engine->resetTick();
  while (!Keyboard::isKeyDown(SDLK_SPACE) && !done) {
    if (engine->nextTickFPS(50)) {
      engine->tick();
    } else {
      engine->render();
      engine->swap();
    }
    if (Keyboard::isKeyDown(SDLK_ESCAPE)) {
      if (menumain->perform() == EM_MENU_EXIT) done = true;
    }
  }
  delete(engine);
  return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
