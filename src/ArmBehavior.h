/***************************************************************************
                          ArmBehavior.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef ARMBEHAVIOR_H
#define ARMBEHAVIOR_H

#include "Behavior.h"
#include "StateMachine.h"
#include "EMath.h"

/** A Behavior for the pinball demo. */
class ArmBehavior : public Behavior {
 public:
	ArmBehavior(bool right = true);
	~ArmBehavior();
	void onTick();
	void StdOnSignal() {};
	void StdOnCollision() {};
	inline void setSound(int s) { m_iSound = s; };
	inline int getSound() { return m_iSound; };
	inline void setIsRight(bool r) { m_bRight = r; };
	inline bool getIsRight() { return m_bRight; };
 private:
	Vertex3D m_vtxRot;
	bool m_bRight;
	int m_iCount;
	bool m_bOn;
	int m_iSound;
	bool m_bRot;
};

#endif // ARMBEHAVIOR_H
