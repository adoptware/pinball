/***************************************************************************
 BillBoard test
 ***************************************************************************/

#include "Engine.h"
#include "Camera.h"
#include "BillBoard.h"
#include "KeyBehavior.h"
#include "Keyboard.h"
#include "TextureUtil.h"

/** Main */
int main(int argc, char *argv[]) {
	cerr << "BillBoard test." << endl;

	if (argc < 2) {
		cerr << "Usage: billboard <image-file>" << endl;
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

	// Add a BillBoard
	BillBoard* billboard = new BillBoard(texture, 1.0, 1.0);
	billboard->setProperty(EM_BILLBOARD_ALPHATEST);
	billboard->setProperty(EM_BILLBOARD_TRANS);
	Group* groupBB = new Group();
	engine->add(groupBB);
	groupBB->setBillBoard(billboard);
	
	// Add a behavior to the billboard
	KeyBehavior* keyBeh = new KeyBehavior();
	groupBB->addBehavior(keyBeh);
		
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		engine->tick();
		engine->render();
		engine->swap();
	}
	delete(engine);
	return 0;
}
