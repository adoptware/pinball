/***************************************************************************
                          SoundVisitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist 
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef SOUNDVISITOR_H
#define SOUNDVISITOR_H

#include "Private.h"
#include "Visitor.h"

class Group;

/** @author Henrik Enqvist */
class SoundVisitor : public Visitor {
 protected:
	SoundVisitor();
 public:
	~SoundVisitor();
	static SoundVisitor * getInstance();
	void visit(Group*);
 private:
	static SoundVisitor * p_SoundVisitor;
};

#endif // SOUNDVISITOR_H
