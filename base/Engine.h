/***************************************************************************
                          Engine.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
***************************************************************************/

#ifndef ENGINE_H
#define ENGINE_H

#ifndef PRIVATE_H
#error Must include Private.h before Engine.h
#endif

#include "TextureUtil.h"
#include "Group.h"

class Light;
class Camera;

/** The engine in the system.
 * @see Group
 * @see Shape3D
 * @see Light
 * @see Sound */
class Engine : public Group {
 public:
  /** Engine(argc, argv). All arguments belonging to the engine are removed from the vector. */
  Engine(int & argc, char** argv);
  ~Engine();
  /** Shutdown the engine */
  void stopEngine();
  void clearScreen();
  /** Deallocate all objects in engine. */
  void clear();
	/** Draws a background to the screen. Do not use inside render loop, it
			might put OpenGL in a weird state. */
	void drawSplash(EmTexture * tex);
  /** Aligns vertices, calculates light, clears the screen and draws the polygons to 
   * the GL buffer. */
  void render();
  /** Swaps the SDL buffers. Extern GLs must use something else. */
  void swap();
  /** Moves objects, detects collision, etc. Does NOT align vertices, caclulate light or
   * draw any polygons. Call the render() function for those things that. */
  void tick();
  void setEngineCamera(Group*);
  /** Sets diffuse and ambient light. The light source is straight above. */
  void setLightning(float diffuse, float ambient);
  void addLight(Light*);
  /** Use this function to limit the main loop to a desired frame rate.
   ** Returns false if the thread is behind schedule else true. 
   ** Available FSP 100, 50, 40, 33, 25, 20 */
  bool nextTickFPS(int fps);
  void resetTick();
  //  bool limitFPS(int fps);
  void delay(int ms);
  void setClearColor(float r, float g, float b, float a);
#if EM_THREADS
  /** When using threaded ticks you must use this function instead of render(). */
  void renderThreadSafe();
  void startTickThread();
  void pauseTickThread();
  void resumeTickThread();
  void endTickThread();
#endif
};

#endif // ENGINE_H
