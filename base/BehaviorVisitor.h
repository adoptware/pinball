/***************************************************************************
                          BehaviorVisitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef BEHAVIORVISITOR_H
#define BEHAVIORVISITOR_H

#include "Visitor.h"

class Group;
/** Only for internal use. @see Visitor */
class BehaviorVisitor : public Visitor {
 protected:
	BehaviorVisitor();
 public:
	~BehaviorVisitor();
	static BehaviorVisitor * getInstance();
	void visit(Group*);
 private:
	static BehaviorVisitor * p_BehaviorVisitor;
};

#endif // BEHAVIORVISITOR_H
