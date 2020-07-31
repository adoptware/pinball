/***************************************************************************
                          Behavior.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist GPL
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "Behavior.h"
#include "Group.h"

#include <cstddef>

Behavior::Behavior() {
	//EM_COUT("Behavior::Behavior", 1);
	p_Light = NULL;
	p_Parent = NULL;
	m_iType = 0;
}

