/***************************************************************************
                          SignalVisitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by
    email                :
 ***************************************************************************/

#ifndef SIGNALSENDER_H
#define SIGNALSENDER_H

#include "Private.h"
#include "Visitor.h"

class Group;

typedef struct {
	int signal;
	int delay;
	Group * sender;
	Group * reciever;
} SignalStruct;


/** A medium for sending signals between behaviors. */
class SignalSender {
 private:
	SignalSender();
 public:
	~SignalSender();
	static SignalSender * getInstance();

	/** If reciever is NULL the signal will be sent to all registered groups, multicast. */
	void addSignal(int signal, int delay, Group * sender, Group * reciever);
	void addGroup(Group * g);
	void clear();
	void tick();
 private:
	vector<SignalStruct> m_vSignal;
	vector<Group*> m_vGroup;
	static SignalSender * p_SignalSender;
};
#endif // SIGNALVISITOR_H
