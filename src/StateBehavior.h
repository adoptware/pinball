/***************************************************************************
                          StateBehavior.h  -  description
                             -------------------
    begin                : Thu Feb 24 2002
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef STATEBEHAVIOR_H
#define STATEBEHAVIOR_H

#include "Behavior.h"
#include "StateMachine.h"

class Light;

class StateItem {
 public:
	StateItem(int asig, int csig, int delay);
	~StateItem();
	void setTr(float x, float y, float z, float speed);
	void setRot(float x, float y, float z, float speed);
	void addTexCoord(float u, float v);
	void setProperty(int p) { m_iProperty = p; };
	void setLight(bool b) { m_bLight = b; };
	void setSound(int s) { m_iSound = s; };
	void setDelay(int d) { m_iDelay = d; };

	int m_iActSig;
	int m_iCollSig;
	int m_iSigDelay;
	bool m_bLight;
	Vertex3D m_vtxTr;
	Vertex3D m_vtxRot;
	float m_fTrSpeed;
	float m_fRotSpeed;
	vector<TexCoord> m_vTexCoord;
	int m_iProperty;
	int m_iSound;
	int m_iDelay;
	int m_iTick;
	StateItem* p_NextStateItem;
};

/** @author Henrik Enqvist */
class StateBehavior : public Behavior  {
	public:
		StateBehavior();
		~StateBehavior();
		void onTick();
		void StdOnSignal();
		void StdOnCollision();
		void useMove(bool t) { m_bMove = t; };
		void useTexCoord(bool t) { m_bTexCoord = t; };
		void useProperty(bool t) { m_bProperty = t; };
		void addStateItem(StateItem* s);
		int getOwnerBall() { return m_iOwnerBall; };
		void setState(StateItem* s);
	private:
		int m_iOwnerBall;
		int m_iTick;
		int m_iCollisionSafe;
		bool m_bProperty;
		bool m_bTexCoord;
		bool m_bMove;
		StateItem* p_CurrentStateItem;
		vector<StateItem*> m_vStateItem;
};

#endif
