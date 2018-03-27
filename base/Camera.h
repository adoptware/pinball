/***************************************************************************
                          camera.h  -  description
                             -------------------
    begin                : Fri Jan 28 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

/** Every engine needs a camera. The camera is the viewer of the world.
 * The camera is added to the engine with the "setEngineCamera" function.
 * If you want to move the camera around you should add it to a group.
 * <p>Example:<br><blockquote>
 * Group* groupC = new Group(1);<br>
 * Camera* camera = new Camera();<br>
 * <br>
 * engine->add(groupC);<br>
 * groupC->add(camera);<br>
 * <br>
 * engine->setEngineCamera(groupC);<br>
 * </blockquote></p>
 * @see Engine */
class Camera {
 public:
	Camera();
	~Camera();
};

#endif // CAMERA_H
