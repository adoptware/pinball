/***************************************************************************
                          Engine.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef ENGINE_H
#define ENGINE_H


#include "Private.h"
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
		void clearScreen();
		/** Adds a static background that will be rendered before
		 * the 3D shapes. UNIMPLEMENTED. */
		void setBackground(EmImage*);
		/** Creates a thread that draws the scene in the engine to the screen. */
		void startRenderThread();
		/** Pauses the render thread in safe state, allows other threads to render to the screen. 
		 * ATTENTION! You must pause the render thread if you want to add more data to the
		 * scene after you started the thread. */
		void pauseRenderThread();
		/** Resume the render thread after a pause, pauseRenderThread must be called
		 * before resumeRenderThread. */
		void resumeRenderThread();
		/** Kill the render thread, waits for render thread to die before it returns. */
		void stopRenderThread();
		/** Renders all polygons to the GL buffers. DEPRECATED */
		void render();
		/** Swaps the SDL buffers. Extern GLs must use something else. */
		void swap();
		/** Moves objects, detects collision, etc. Does not draw the polygons, use the render 
		 * function for that. Pauses the render thread when shared data is acessed. DO NOT
		 * use tick between a pauseRenderThread and resumeRenderThread. */
		void tick();
		void setEngineCamera(Group*);
		/** Sets diffuse and ambient light. The light source is straight above. */
		void setLightning(float diffuse, float ambient);
		void addLight(Light*);
	private:
		EmImage* m_Background;

		void initGrx();
		void initSound();
		void resize(unsigned int w, unsigned int h);
};

#endif // ENGINE_H
