/***************************************************************************
                          SignalVisitor.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by
    email                :
 ***************************************************************************/

#include "SignalVisitor.h"
#include "Group.h"
#include "Behavior.h"

SignalVisitor::SignalVisitor() {
}

SignalVisitor::~SignalVisitor() {
}

void SignalVisitor::add(int s, Group * g) {
	SignalStruct sig;
	sig.signal = s;
	sig.group = g;
	m_vSignal.push_back(sig);
}

void SignalVisitor::empty() {
}

void SignalVisitor::clear() {
	EM_COUT("SignalVisitor::empty() removed " << m_vSignal.size(), 0);
	m_vSignal.clear();	
}

void SignalVisitor::visit(Group* g) {
	EM_COUT("SignalVisitor::visit()", 0);
	if (g->m_iProperties & EM_GROUP_NO_SIGNAL) return;

	vector<Behavior*>::iterator behIter = g->m_vBehavior.begin();
	vector<Behavior*>::iterator behEnd = g->m_vBehavior.end();
	for(; behIter != behEnd; behIter++) {
		vector<SignalStruct>::iterator sigIter = m_vSignal.begin();
		vector<SignalStruct>::iterator sigEnd = m_vSignal.end();
		for (; sigIter != sigEnd; sigIter++) {
			(*behIter)->onSignal((*sigIter).signal, (*sigIter).group);
		}
	}
}
