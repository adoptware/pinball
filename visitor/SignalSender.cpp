/***************************************************************************
                          SignalVisitor.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by
    email                :
 ***************************************************************************/

#include "SignalSender.h"
#include "Group.h"
#include "Behavior.h"

SignalSender * SignalSender::p_SignalSender = NULL;

SignalSender::SignalSender() {
}

SignalSender::~SignalSender() {
}

SignalSender * SignalSender::getInstance() {
	if (p_SignalSender == NULL) {
		p_SignalSender = new SignalSender();
	}
	return p_SignalSender;
}

void SignalSender::addSignal(int signal, int delay, Group * sender, Group * reciever) {
	SignalStruct sig;
	sig.signal = signal;
	sig.delay = delay;
	sig.sender = sender;
	sig.reciever = reciever;
	m_vSignal.push_back(sig);
}

void SignalSender::addGroup(Group * g) {
	EmAssert(g != NULL, "Added NULL as a group to SignalSender");
	// TODO: check that the group is not added already
	m_vGroup.push_back(g);
}

void SignalSender::clear() {
	EM_COUT("SignalSender::empty() removed " << m_vSignal.size(), 0);
	m_vSignal.clear();	
}

/* Called each engine tick. */
void SignalSender::tick() {
	EM_COUT("SignalSender::tick()", 0);

	// TODO: the m_vSignal vector may be altered during
	// this loop, does this affect the elements in the vector ???
	vector<SignalStruct>::iterator sigIter = m_vSignal.begin();
	vector<SignalStruct>::iterator sigEnd = m_vSignal.end();
	for (; sigIter != sigEnd; sigIter++) {
		// check the delay
		if ((*sigIter).delay > 0) {
			(*sigIter).delay--;
		}
		// send to groups
		vector<Group*>::iterator groupIter = m_vGroup.begin();
		vector<Group*>::iterator groupEnd = m_vGroup.end();
		for(; groupIter != groupEnd; groupIter++) {
			if ((*groupIter)->m_iProperties & EM_GROUP_NO_SIGNAL) continue;
			// check the address, NULL is multicast
			if ((*sigIter).reciever != NULL && (*sigIter).reciever != (*groupIter)) continue;
			// call onSignal function i behaviors
			// TODO: should use a hashtable for groups
			vector<Behavior*>::iterator behIter = (*groupIter)->m_vBehavior.begin();
			vector<Behavior*>::iterator behEnd = (*groupIter)->m_vBehavior.end();
			for (; behIter != behEnd; behIter++) {
				(*behIter)->onSignal((*sigIter).signal, (*sigIter).sender);
			}
		}
	}
	// TODO: remove used signals instead of clearing the whole vector
	this->clear();
}
