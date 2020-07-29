/***************************************************************************
                          SoundVisitor.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist 
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "SoundVisitor.h"
#include "Sound.h"
#include "Group.h"

SoundVisitor * SoundVisitor::p_SoundVisitor = NULL;

SoundVisitor::SoundVisitor() {
}

SoundVisitor::~SoundVisitor() {
	p_SoundVisitor = NULL;
}

SoundVisitor * SoundVisitor::getInstance() {
	if (p_SoundVisitor == NULL) {
		p_SoundVisitor = new SoundVisitor();
	}
	return p_SoundVisitor;
}

void SoundVisitor::visit(Group* g) {
	if (g->p_Sound != NULL) {
		g->p_Sound->adjust();
	}
}
