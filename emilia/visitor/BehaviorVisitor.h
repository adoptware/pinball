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
/**
 * <p><i>Only for internal use.</i></p>
 * @see Visitor
 */
class BehaviorVisitor : public Visitor {
	public:
		BehaviorVisitor();
		~BehaviorVisitor();
		void visit(Group*);
};

#endif // BEHAVIORVISITOR_H
