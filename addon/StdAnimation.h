/***************************************************************************
                          Animation.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef STDANIMATION_H
#define STDANIMATION_H

#define EM_TYPE_STDANIMATION 1001

#define EM_ROTATION		1
#define EM_TRANSLATION	2
#define EM_LIGHT 4
#define EM_BILLBOARD_SIZE 8

#include <vector>

#include "EMath.h"
#include "Behavior.h"
#include "StateMachine.h"

class Group;

/**
 * Animation moves or rotates a group according to a given path.
 *
 * Animation is represented as list of "x, y, z" points.
 * The x, y, z can be a translation or a rotation.
 * Each time the engines render method is called the animation
 * object will interpolate the group it is attached to, from one point
 * to an other. When the animation has reached its end it will
 * start over.
 * 
 * Usage.
 * Points are added to the animation with the 'add' method.
 * The order of the points will be same in which they are added
 * to the animation.
 *
 * Example:
 * 	Group* group = new Group();
 *	StdAnimation anim = new StdAnimation(20, E_TRANSLATION);
 *	
 *  anim->add(0, 0, 0);
 *	anim->add(100, 0, 0);
 *	anim->add(0, 100, 0);
 *  anim->setEnd(0, 0, 0);
 *	
 *  group->addBehavior(anim);
 *
 * This example will create an animation which moves the group from origo to (100, 0 ,0) 
 * and then to (0, 100, 0) and back towards origo. Each movement will last for 20 engine 
 * ticks. When it reaches the end it will start over again.
 * @see Group
 */
class StdAnimation : public Behavior {
	public:
		/** Animation(step, type). */
		StdAnimation(int step, int type);
		~StdAnimation();
		void onTick();
		void add(float, float, float);
		void setEnd(float, float, float);
		/** Sets the end point to the first point. */
		void setEndStart();
		inline void setStep(int s) {	m_iStep = s; };
		inline void setAnimType(int t) { m_iAnimType = t; };
		inline void clear() { m_vVertex.clear(); };
		void StdOnSignal() {};
		void StdOnCollision() {};
	private:
		vector<Vertex3D> m_vVertex;
		int m_iStep;
		int m_iTick;
		int m_iIndex;
		int m_iAnimType;
		Vertex3D m_vtxEnd;
};

#endif // ANIMATION_H
