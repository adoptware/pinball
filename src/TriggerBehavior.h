/***************************************************************************
                          TriggerBehavior.h  -  description
                             -------------------
    begin                : Thu Jan 25 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef TRIGGERBEHAVIOR_H
#define TRIGGERBEHAVIOR_H

#include "Behavior.h"
#include "StateMachine.h"

class Light;

/** @author Henrik Enqvist */
class TriggerBehavior : public Behavior  {
	public:
		TriggerBehavior(int aSignal, int uSignal, int aSignalOut, int uSignalOut, bool init=false);
		~TriggerBehavior();
		void onTick();
		void StdOnSignal();
		void StdOnCollision();
		void useTexCoord(bool t) { m_bTexCoord = t; };
		void addTexCoordAct(float u, float v) { 
			TexCoord tex;
			tex.u = u;
			tex.v = v;
			m_vTexCoordAct.push_back(tex); 
		};
		void addTexCoordUn(float u, float v) {
			TexCoord tex;
			tex.u = u;
			tex.v = v;
			m_vTexCoordUn.push_back(tex); 
		};
		void setTexCoordAct();
		void setTexCoordUn();
		void useMove(bool t) { m_bMove = t; };
		void setActiveTransform(float tx, float ty, float tz, float rx, float ry, float rz) {
			m_vtxActTr.x = tx; m_vtxActTr.y = ty; m_vtxActTr.z = tz;
			m_vtxActRot.x = rx; m_vtxActRot.y = ry; m_vtxActRot.z = rz;
		}
		void setUnActiveTransform(float tx, float ty, float tz, float rx, float ry, float rz) {
			m_vtxUnTr.x = tx; m_vtxUnTr.y = ty; m_vtxUnTr.z = tz;
			m_vtxUnRot.x = rx; m_vtxUnRot.y = ry; m_vtxUnRot.z = rz;
		}
	private:
		int m_iActiveSignal;
		int m_iUnActiveSignal;
		int m_iActiveSignalOut;
		int m_iUnActiveSignalOut;
		int m_iTick;
		int m_iMoveEnd;
		int m_iCollisionSafe;
		bool m_bActive;
		bool m_bTexCoord;
		bool m_bMove;
		bool m_bInit;

		vector<TexCoord> m_vTexCoordAct;
		vector<TexCoord> m_vTexCoordUn;
		Vertex3D m_vtxActTr;
		Vertex3D m_vtxActRot;
		Vertex3D m_vtxUnTr;
		Vertex3D m_vtxUnRot;
		Vertex3D m_vtxRot;
};

#endif
