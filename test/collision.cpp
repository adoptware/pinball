/***************************************************************************
 Collision test. Some shapes move around and change color if the collide.
 ***************************************************************************/

#include <stdlib.h>
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

/** A behavior class that implements a onSignal method */
class CollisionTest : public StdAnimation {
public:
	CollisionTest() : StdAnimation(10, EM_TRANSLATION) {};
	~CollisionTest() {};
	void StdOnCollision();
};

void CollisionTest::StdOnCollision() {
	EmAssert(p_Parent != NULL, "CollisionTest::StdOnCollision() parent null");
	for (int a=0; a<p_Parent->getShape3DSize(); a++) {
		p_Parent->getShape3D(a)->setColor(1,0,0,1);
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
	for (int a=0; a<4; a++) {
		Shape3D * shape;
		switch (a%3) {
		case 9: shape = new BigSphere(1.0, 2, 1.0, 1.0, 1.0, 1.0); break;
		case 10: shape = new Cone(2.0, 1.0, 1.0, 1.0, 1.0); break;
		default: shape = new Cube(2.0, 1.0, 1.0, 1.0, 1.0);
		}
		// Add the shape.
		Group* groupShape = new Group();
		groupShape->addShape3D(shape);
		vShape3D.push_back(shape);
		
		// Add the collisionbounds
		CollisionBounds* cb = new CollisionBounds(shape->getCollisionSize());
		cb->setShape3D(shape, 1);
		groupShape->setCollisionBounds(cb);
		// Add a behavior to the cube
		CollisionTest* beh = new CollisionTest();
// 		beh->add(random()%9-4, random()%9-4, random()%9-4);
// 		beh->add(random()%9-4, random()%9-4, random()%9-4);
// 		beh->add(random()%9-4, random()%9-4, random()%9-4);
		beh->add(random()%3-1, random()%3-1, random()%3-1);
		beh->add(random()%3-1, random()%3-1, random()%3-1);
		beh->add(random()%3-1, random()%3-1, random()%3-1);
		beh->setEndStart();
		groupShape->addBehavior(beh);
		
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
		engine->limitFPS(100);
	}
	delete(engine);

	extern float em_groups_m, em_shapes_m, em_bounds_m, em_polygons_m;

	cerr << "Groups " << em_groups_m << endl;
	cerr << "Shapes " << em_shapes_m << endl;
	cerr << "Bounds " << em_bounds_m << endl;
	cerr << "Polys " << em_polygons_m << endl;

	return 0;
}
