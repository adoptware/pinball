/***************************************************************************
                          Camera.h  -  description
                             -------------------
    begin                : Fri Jan 28 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#include "Private.h"

/**
 * <p>Every engine needs a camera. The camera is the viewer of the world.</p>
 * <p>The camera is added to the engine with the "setCamera" function.
 * If you want to move the camera around you should add ir to a group.</p>
 * <p>Example:<br><blockquote>
 * Group* groupC = new Group(1);<br>
 * Camera* camera = new Camera();<br>
 * <br>
 * engine->add(groupC);<br>
 * groupC->add(camera);<br>
 * <br>
 * engine->setCamera(groupC);<br>
 * </blockquote></p>
 * @see Engine
 */
class Camera {
	public:
		Camera();
		~Camera();
};

#endif // CAMERA_H
