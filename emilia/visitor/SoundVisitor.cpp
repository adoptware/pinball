/***************************************************************************
                          SVisit.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "SoundVisitor.h"
#include "Sound.h"
#include "Group.h"

SoundVisitor::SoundVisitor() {
}

SoundVisitor::~SoundVisitor() {
}

void SoundVisitor::visit(Group* g) {
	if (g->p_Sound != NULL) {
		g->p_Sound->adjust();
	}
}
