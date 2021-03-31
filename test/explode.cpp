/***************************************************************************
 Texture animation test. An explosion.
 ***************************************************************************/

#include <iostream>

#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "BillBoard.h"
#include "KeyBehavior.h"
#include "Keyboard.h"
#include "TextureUtil.h"
#include "Cube.h"
#include "StdAnimation.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "Explode test." << endl;

	if (argc < 2) {
		cerr << "Usage: explode <image-file>" << endl;
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
	groupCamera->setTranslation(0, 0, 7);
	engine->setEngineCamera(groupCamera);

	// Load a texture
	EmTexture* texture = TextureUtil::getInstance()->loadTexture(argv[1]);

	// Add a cube
	Group* groupCube = new Group();
	Cube* cube = new Cube(1, 1, 0, 0, 1);
	groupCube->addShape3D(cube);
	engine->add(groupCube);
	// Add some BillBoards
#define BILLBOARD(tx, ty, tz) \
	{                           \
		BillBoard* billboard = new BillBoard(texture, 2.0, 3.0); \
		billboard->setProperty(EM_BILLBOARD_ALPHATEST);          \
		billboard->setProperty(EM_BILLBOARD_TRANS);              \
		Group* groupBB = new Group();                            \
		groupBB->setTranslation(tx, ty, tz);                     \
		groupCube->add(groupBB);                                 \
		groupBB->setBillBoard(billboard);                        \
                                                             \
		StdAnimation * anim = new StdAnimation(50, EM_BILLBOARD_SIZE);  \
		anim->add(0,0,0);                                        \
		anim->add(0,0,0);                                        \
		anim->add(0,0,0);                                        \
		anim->add(0,0,0);                                        \
		anim->add(0,0,0);                                        \
		anim->setEnd(4, 4, 4);                                   \
		groupBB->setBehavior(anim);                              \
	}

	BILLBOARD(0,0,0);


	// Add a behavior to the billboard
	KeyBehavior* keyBeh = new KeyBehavior();
	groupCube->setBehavior(keyBeh);
		
	engine->resetTick();
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
	  if (engine->nextTick()) {
	    engine->tick();
	  } else {
	    engine->render();
	    engine->swap();
	  }
	}
	delete(engine);
	return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
