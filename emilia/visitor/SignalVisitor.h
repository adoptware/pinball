/***************************************************************************
                          SignalVisitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by
    email                :
 ***************************************************************************/

#ifndef SIGNALVISITOR_H
#define SIGNALVISITOR_H

#include "Private.h"
#include "Visitor.h"

class Group;

typedef struct {
	int signal;
	Group * group;
} SignalStruct;


/** <p><i>Only for internal use.</i></p>
 */
class SignalVisitor : public Visitor {
	public:
		SignalVisitor();
		~SignalVisitor();
		void visit(Group*);
		void empty();
		void clear();
		void add(int s, Group * g);
	private:
		vector<SignalStruct> m_vSignal;
};
#endif // SIGNALVISITOR_H
