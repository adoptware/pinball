/***************************************************************************
                          Behavior.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "Behavior.h"
#include "Group.h"

Behavior::Behavior() {
	p_Light = NULL;
	p_Parent = NULL;
	for (int a=0; a<256; a++) {
		m_iState[a] = 0;
	}
}

void Behavior::setParent(Group* g) {
	p_Parent = g;
}
