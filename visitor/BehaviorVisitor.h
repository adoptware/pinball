/***************************************************************************
                          BVisitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef BEHAVIORVISITOR_H
#define BEHAVIORVISITOR_H

#include "Private.h"
#include "Visitor.h"

class Group;
/** Only for internal use.
 * @see Visitor */
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
