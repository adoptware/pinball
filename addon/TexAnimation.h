/***************************************************************************
                          Animation.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef TEXANIMATION_H
#define TEXANIMATION_H

#include "EMath.h"
#include "Behavior.h"
#include "StateMachine.h"

class Group;

/** @see Group */
class TexAnimation : public Behavior {
	public:
		/** Animation(stepsize, nvertex, type). */
		TexAnimation(int step, int nvtx);
		~TexAnimation();
		void onTick();
		void add(float, float);
		void onCollision(const Vertex3D &, const Vertex3D &, Group *) {};
		void onSignal(int, Group *) {};
	private:
		vector<TexCoord> m_vTexCoord;
		int m_iStep;
		int m_iTick;
		int m_iTexCoord;
};

#endif // TEXANIMATION_H
