/***************************************************************************
 Collision test. Some shapes move around and change color if the collide.
 ***************************************************************************/

#include <cstdlib>
#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "Cone.h"
#include "BigSphere.h"
#include "Keyboard.h"
#include "Behavior.h"
#include "Shape3D.h"
#include "StateMachine.h"
#include "StdAnimation.h"
#include "CollisionBounds.h"

#define EM_ALT 0
#define EM_FAST_SPHERE 1

/***************************************************************************
This test is also used as a performance measure. Below is a
log of frame rates at important dates. This logs shows how
various optimization affects the frame rate.

System: AMD K6-II 500Mhz (83.3Mhz bus) 
        Voodoo 3 PCI 16 Mb (85Hz vrate)
        640x480 windowed
Compile: -g -W -Wall -O2
Defines: EM_DEBUG EM_USE_SDL
Use:     -nosound

20020604: 53 (forgott -nosound)

20020630: 69 (57 with sound)(77 without rendering)

20020812: 61 (79 without rendering, rendering has become slower, collision
              detection is faster - changed a "sqrt(a) < len" to "a < len*len")

20020815: 134 (fast sphere detection, nice)

****************************************************************************

System: AMD K6-II 500Mhz (83.3Mhz bus) 
        Voodoo 3 PCI 16 Mb (85Hz vrate)
        320x240 windowed
Compile: -g -W -Wall -O2
Defines: EM_DEBUG EM_USE_ALLEGRO

20020701: 23

20020701: 24
  Removed some debug and changed some a++ to ++a.

****************************************************************************/

/** A behavior class that implements a onSignal method */
class CollisionTest : public StdAnimation {
public:
#if EM_ALT
	CollisionTest() : StdAnimation(250, EM_TRANSLATION) {};
#else
	CollisionTest() : StdAnimation(50, EM_TRANSLATION) {};
#endif
	~CollisionTest() {};
	void StdOnCollision();
};

void CollisionTest::StdOnCollision() {
	EmAssert(this->getParent() != NULL, "CollisionTest::StdOnCollision() parent null");
	for (int a=0; a<this->getParent()->getShape3DSize(); a++) {
		this->getParent()->getShape3D(a)->setColor(1,0,0,1);
	}
}

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
	groupCamera->setTranslation(0, 0, 10);
	engine->setEngineCamera(groupCamera);

	vector<Shape3D*> vShape3D;
	for (int a=0; a<16; a++) {
		Shape3D * shape;
		switch (a%3) {
		case 0: shape = new BigSphere(1.0, 1, 1.0, 1.0, 1.0, 1.0); break;
		case 1: shape = new Cone(2.0, 1.0, 1.0, 1.0, 1.0); break;
		default: shape = new Cube(2.0, 1.0, 1.0, 1.0, 1.0);
		}
		// Add the shape.
		Group* groupShape = new Group();
		groupShape->addShape3D(shape);
		vShape3D.push_back(shape);
		
		// Add the collisionbounds
#if EM_FAST_SPHERE
		CollisionBounds* cb;
		if (a%3 == 0) {
			cb = new CollisionBounds(1.0f/EM_SQRT_3);
		} else {
			cb = new CollisionBounds(shape->getCollisionSize());
			cb->setShape3D(shape, 1);
		}
		groupShape->setCollisionBounds(cb);
#else
		CollisionBounds* cb = new CollisionBounds(shape->getCollisionSize());
		cb->setShape3D(shape, 1);
		groupShape->setCollisionBounds(cb);
#endif
		// Add a behavior to the cube
		CollisionTest* beh = new CollisionTest();
#if EM_ALT
 		beh->add(-16, 0, 0);
 		beh->add(a*3-16, 0, 0);
#else
 		beh->add(a-8, 0, a-8);
 		beh->add(a-8, a-8, 0);
 		beh->add(0, a-8, a-8);
#endif
//  		beh->add(random()%11-5, random()%11-5, random()%11-5);
//  		beh->add(random()%11-5, random()%11-5, random()%11-5);
//  		beh->add(random()%11-5, random()%11-5, random()%11-5);
		beh->setEndStart();
		groupShape->setBehavior(beh);
		
		engine->add(groupShape);
	}
		
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
		vector<Shape3D*>::iterator iter = vShape3D.begin();
		vector<Shape3D*>::iterator end = vShape3D.end();
		for (; iter != end; iter++) {
			(*iter)->setColor(0, 0, 1, 1);
		}
		engine->tick();
		engine->render();
		engine->swap();
		//engine->limitFPS(10);
	}

	extern float em_groups_m, em_shapes_m, em_bounds_m, em_polygons_m;

#if EM_DEBUG
	cerr << "Groups " << em_groups_m << endl;
	cerr << "Shapes " << em_shapes_m << endl;
	cerr << "Bounds " << em_bounds_m << endl;
	cerr << "Polys " << em_polygons_m << endl;
#endif

	delete(engine);
	return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
