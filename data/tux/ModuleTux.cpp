/***************************************************************************
                          Score.cpp  -  description
                             -------------------
    begin                : Fri Jan 26 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "Behavior.h"
#include "Group.h"
#include "Pinball.h"
#include "Loader.h"
#include "StateMachine.h"

class TuxBehavior : public Behavior {
public:
	TuxBehavior() : Behavior() {
		// init signals
		m_iLinuxL = Loader::getInstance()->getSignal("linuxl");
		m_iLinuxI = Loader::getInstance()->getSignal("linuxi");
		m_iLinuxN = Loader::getInstance()->getSignal("linuxn");
		m_iLinuxU = Loader::getInstance()->getSignal("linuxu");
		m_iLinuxX = Loader::getInstance()->getSignal("linuxx");
	};
	~TuxBehavior() {};

	void onTick() {};

	void StdOnCollision() {};

	void StdOnSignal() {
		EM_COUT((int)em_signal, 1);
		
		OnSignal( PBL_SIG_RESET_ALL ) {
			this->clear();
		} else
			OnSignal(m_iLinuxL) {
			cerr << "L" << endl;
		} else 
			OnSignal(m_iLinuxI) {
			cerr << "I" << endl;
		} else
			OnSignal(m_iLinuxN) {
			cerr << "N" << endl;
		} else
			OnSignal(m_iLinuxU) {
			cerr << "U" << endl;
		} else
			OnSignal(m_iLinuxX) {
			cerr << "X" << endl;
		}
	}

	void clear() {
		m_aLinux[0] = false;
		m_aLinux[1] = false;
		m_aLinux[2] = false;
		m_aLinux[3] = false;
		m_aLinux[4] = false;
		m_aBoot[0] = false;
		m_aBoot[1] = false;
		m_aBoot[2] = false;
		m_aBoot[3] = false;
		m_aTux[0] = false;
		m_aTux[1] = false;
		m_aTux[2] = false;
	};
private:
	int m_iLinuxL;
	int m_iLinuxI;
	int m_iLinuxN;
	int m_iLinuxU;
	int m_iLinuxX;
	int m_aLinux[5];
	int m_aBoot[4];
	int m_aTux[3];
};

extern "C"  void * new_object_fct(void) {
	return new TuxBehavior();
}

