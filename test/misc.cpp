/***************************************************************************
 Triangle point distance test.
***************************************************************************/

#include <iostream>

#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "Cube.h"
#include "Cone.h"
#include "Keyboard.h"
#include "KeyBehavior.h"
#include "KeyRotBehavior.h"
#include "Shape3D.h"
#include "Polygon.h"
#include "CollisionBounds.h"
#include "EMath.h"
#include "CollisionVisitor.h"
#include "Cube.h"

/** A Triangle */
class TriClass : public Shape3D {
public:
  TriClass();
  ~TriClass() {};
};

TriClass::TriClass() {
  this->add(1, -1, 0, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
  this->add(-1, -1, 0, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
  this->add(0, 1, 0, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
  this->add(1, 1, 0, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);

  Polygon3D * p = new Polygon3D(this, 3);
  p->add(0);
  p->add(1);
  p->add(2);
  p->add(3);
  this->add(p);

  this->countNormals();
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
  groupCamera->setTranslation(0, 0, 2);
  engine->setEngineCamera(groupCamera);

  engine->addShape3D(new Cube(0.1f, 1.0f, 1.0f, 1.0f, 1.0f));

  Group * gt = new Group();
  engine->add(gt);
  gt->setBehavior(new KeyBehavior());
  Group * gr = new Group();
  gt->add(gr);
  gr->setBehavior(new KeyRotBehavior());
  TriClass * tri = new TriClass();
  gr->addShape3D(tri);
		
  Vertex3D vtx = {0.0f, 0.0f, 0.0f};
  Vertex3D vtxDist;
  engine->resetTick();
  while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {
    if (engine->nextTickFPS(10)) {
      engine->tick();
      float sqrdist = CollisionVisitor::getInstance()->
	vtxPolySqrDist(vtx, tri->getPolygon(0), vtxDist);
      cerr << EMath::emSqrt(sqrdist) <<" : "<< 
	vtxDist.x <<" "<< vtxDist.y <<" "<< vtxDist.z << endl; 
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
