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
class AmbientLightVisitor;
class BehaviorVisitor;
class CollisionVisitor;
class OpenGLVisitor;
class OpenGLTransVisitor;
class PointLightVisitor;
class PNormalVisitor;
class TransformVisitor;
class SoundVisitor;

/**
 * The engine in the system. This first object you should create.
 *
 * @see Group
 * @see Shape3D
 * @see Light
 * @see Sound
 */
class Engine : public Group {
	public:
		/** Engine(argc, argv). All arguments belonging to the engine are removed from the vector. */
		Engine(int & argc, char** argv);
		~Engine();
		void clearScreen();
		/** Adds a static background that will be rendered before
		 * the 3D shapes. UNIMPLEMENTED. */
		void setBackground(EmImage*);
		/** Renders all polygons to the GL buffers.*/
		void render();
		/** Swaps the SDL buffers. Extern GLs must use something else. */
		void swap();
		/** Moves objects, detects collision, etc. Does not draw the polygons, use the render 
		 * function for that. */
		void tick();
		void setEngineCamera(Group*);
		/** Sets diffuse and ambient light. The light source is straight above. */
		void setLightning(float, float);
		void addLight(Light*);
		//		void sendSignal(int s, Group * g);
	private:
		EmImage* m_Background;

		AmbientLightVisitor* p_AmbientLightVisitor;
		BehaviorVisitor* p_BehaviorVisitor;
		CollisionVisitor* p_CollisionVisitor;
		OpenGLVisitor* p_OpenGLVisitor;
		OpenGLTransVisitor* p_OpenGLTransVisitor;
		PointLightVisitor* p_PointLightVisitor;
		PNormalVisitor* p_PNormalVisitor;
		//		SignalVisitor* p_SignalVisitor;
		SoundVisitor* p_SoundVisitor;
		TransformVisitor* p_TransformVisitor;

		void initGrx();
		void initSound();
};

#endif // ENGINE_H
