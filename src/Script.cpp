/***************************************************************************
                          Script.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "Script.h"
#include "Group.h"
#include "Shape3D.h"

void Script::setVariable(int i, int v) {
	map<int, int>::iterator element = m_hVariable.find(i);
	if (element != m_hVariable.end()) {
		(*element).second = v;
	}	else {
		m_hVariable.insert(pair<int, int>(i, v));
	}
}

int Script::getVariable(int i) {
	map<int, int>::iterator element = m_hVariable.find(i);
	if (element != m_hVariable.end()) {
		return (*element).second;
	}	else {
		return 0;
	}
}

void Script::interpret(QueryItem * queryitem, int signal, Group * group) {
	EmAssert(queryitem != NULL, "Scrip::interpret queryitem NULL");

	switch (queryitem->m_iQuery) {
	case EM_SCRIPT_ONSIGNAL: {
		vector<int>::iterator iter = queryitem->m_vQueryParm.begin();
		vector<int>::iterator end = queryitem->m_vQueryParm.end();
		for (; iter != end; ++iter) {
			if ((*iter) == signal) {
				this->execute(queryitem);
				break;
			}
		}
	} break;
	}
}

void Script::execute(QueryItem * queryitem) {
	EmAssert(queryitem != NULL, "Script::execute queryitem NULL");
	EmAssert(this->getParent() != NULL, "Script::execute parent NULL");

	switch (queryitem->m_iAction) {
	case EM_SCRIPT_SENDSIGNAL: {
		if (queryitem->m_vActionParm.size() > 1) {
			SendSignal(queryitem->m_vActionParm[0], queryitem->m_vActionParm[1], this->getParent(), NULL);
		}
	} break;
	case EM_SCRIPT_SETVAR: {
		if (queryitem->m_vActionParm.size() > 1) {
			this->setVariable(queryitem->m_vActionParm[0], queryitem->m_vActionParm[1]);
		}
	} break;
	}
}

void Script::onSignal(int signal, Group * group) {
	vector<QueryItem*>::iterator iter = m_vQueryItem.begin();
	vector<QueryItem*>::iterator end = m_vQueryItem.end();
	for (; iter != end; ++iter) {
		this->interpret((*iter), signal, group);
	}
}

