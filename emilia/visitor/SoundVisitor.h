/***************************************************************************
                          SVisit.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef SOUNDVISITOR_H
#define SOUNDVISITOR_H

#include "Private.h"

#include "Visitor.h"


class Group;

/**
 *
 */
class SoundVisitor : public Visitor
{
	public:
		SoundVisitor();
		~SoundVisitor();
		void visit(Group*);
};

#endif // SOUNDVISITOR_H
