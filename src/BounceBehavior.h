/***************************************************************************
                          BouBeh.h  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef BOUNCEBEHAVIOR_E
#define BOUNCEBEHAVIOR_E

#include "Behavior.h"
#include "EMath.h"
#include "StateMachine.h"

/** A behavior for the pinball demo. */

class BounceBehavior : public Behavior {
	public:
		BounceBehavior(int ball);
		~BounceBehavior();
		void onTick();
		void StdOnSignal();
		void onCollision(const Vertex3D & vtx, const Vertex3D & vtx, Group* g);
		void getDirection(Vertex3D & vtx);
		bool isAlive() {return m_bAlive;};
	private:
		bool m_bAlive;
		int m_iKnock;
		int m_iBall;
		Vertex3D m_vtxDir;
		Vertex3D m_vtxOldDir;
		int m_iDirFactor;
		int m_iCollisionPrio;
};

#endif // BounceBehavior
