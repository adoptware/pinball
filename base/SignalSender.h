/***************************************************************************
                          SignalVisitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef SIGNALSENDER_H
#define SIGNALSENDER_H

#include <vector>

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
 protected:
	SignalSender();
 public:
	~SignalSender();
	static SignalSender * getInstance();
	/** If reciever is NULL the signal will be sent to all registered groups, multicast. */
	void addSignal(int signal, int delay, Group * sender, Group * reciever);
	void addGroup(Group * g);
	/** Removes all groups from the signalsender. Does not not free the memory
	 * allocated by the groups. */
	void clear();
	void tick();
 private:
	vector<SignalStruct> m_vSignal;
	vector<SignalStruct> m_vSignalBuffer;
	vector<Group*> m_vGroup;
	static SignalSender * p_SignalSender;
};
#endif // SIGNALVISITOR_H
