/***************************************************************************
                          module.cpp  -  description
                             -------------------
    begin                : Sun Sep 22 2002
    copyright            : (C) 2002 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <stdlib.h>
#include "Behavior.h"


class MyBehavior : public Behavior {
 public:
	MyBehavior() : Behavior() {};
	~MyBehavior() {};
	void onTick() {};
	void onCollision(const Vertex3D & vtxWall, const Vertex3D & vtxOwn, Group * g) {};
	void onSignal(int signal, Group * sender) {};
};

extern "C"  void * new_object_fct(void) {
	return new MyBehavior();
}
