/***************************************************************************
                          Animation.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef TEXANIMATION_H
#define TEXANIMATION_H

#define EM_INTERPOLATE

#include "Private.h"
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
 * 	Group* group = new Group(1);
 *	Animation anim = new Animation(20, E_TRANSLATION);
 *	
 *  anim->add(0, 0, 0);
 *	anim->add(100, 0, 0);
 *	anim->add(0, 100, 0);
 *  anim->setEnd(0, 0, 0);
 *	
 *  group->setAnimation(anim);
 *
 * This example will create an animation which moves the group from origo to (100, 0 ,0) 
 * and then to (0, 100, 0) and back towards origo. Each movement will last for 20 engine 
 * ticks. When it reaches the end it will start over again.
 * @see Group
 */
class TexAnimation : public Behavior {
	public:
		/** Animation(stepsize, nvertex, type). */
		TexAnimation(int step, int nvtx, int type);
		~TexAnimation();
		void onTick();
		void add(float, float);

		void StdOnSignal() {};
		void StdOnCollision() {};
	private:
		vector<TexCoord> m_vTexCoord;
		int m_iStep;
		int m_iTick;
		int m_iType;
		int m_iTexCoord;
};

#endif // TEXANIMATION_H
