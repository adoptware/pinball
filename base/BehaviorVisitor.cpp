/***************************************************************************
                          BehaviorVisitor.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "BehaviorVisitor.h"
#include "Behavior.h"
#include "Group.h"

#include <cstddef>

BehaviorVisitor * BehaviorVisitor::p_BehaviorVisitor = NULL;

BehaviorVisitor::BehaviorVisitor() {
}

BehaviorVisitor::~BehaviorVisitor() {
  p_BehaviorVisitor = NULL;
}

BehaviorVisitor * BehaviorVisitor::getInstance() {
  if (p_BehaviorVisitor == NULL) {
    p_BehaviorVisitor = new BehaviorVisitor();
  }
  return p_BehaviorVisitor;
}

void BehaviorVisitor::visit(Group* g) {
  // Check properties before applying behavior
  if (g->m_iProperties & EM_GROUP_NO_BEHAVIOR) return;
  
  // 	vector<Behavior*>::iterator iter = g->m_vBehavior.begin();
  // 	vector<Behavior*>::iterator end = g->m_vBehavior.end();
  // 	for(; iter != end; iter++) {
// 		(*iter)->onTick();
// 	}
  if (g->getBehavior() != NULL) {
    g->getBehavior()->onTick();
  }
}
