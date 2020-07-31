/***************************************************************************
                          SignalVisitor.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#include "Private.h"
#include "SignalSender.h"
#include "Group.h"
#include "Behavior.h"

#include <cstddef>

SignalSender * SignalSender::p_SignalSender = NULL;

SignalSender::SignalSender() {
}

SignalSender::~SignalSender() {
  p_SignalSender = NULL;
}

SignalSender * SignalSender::getInstance() {
  if (p_SignalSender == NULL) {
    p_SignalSender = new SignalSender();
  }
  return p_SignalSender;
}

void SignalSender::addSignal(int signal, int delay, Group * sender, Group * reciever) {
  SignalStruct sig;
  sig.signal = signal;
  sig.delay = delay;
  sig.sender = sender;
  sig.reciever = reciever;
  // add the signal to the buffer 
  m_vSignalBuffer.push_back(sig);
}

void SignalSender::addGroup(Group * g) {
  EmAssert(g != NULL, "Added NULL as a group to SignalSender");
  // TODO: check that the group is not added already
  m_vGroup.push_back(g);
}

void SignalSender::clear() {
  EM_COUT("SignalSender::clear()", 1);
  m_vSignal.clear();
  m_vSignalBuffer.clear();
  m_vGroup.clear();
}

/* Called each engine tick. */
void SignalSender::tick() {
  EM_COUT("SignalSender::tick() " << m_vGroup.size() << " groups", 0);
  // use a buffer to avoid changes in m_vSignal during loop
  vector<SignalStruct>::iterator sigIter = m_vSignalBuffer.begin();
  vector<SignalStruct>::iterator sigEnd = m_vSignalBuffer.end();
  for (; sigIter != sigEnd; sigIter++) {
    m_vSignal.push_back((*sigIter));
  }
  m_vSignalBuffer.clear();

  // send signals
  sigIter = m_vSignal.begin();
  sigEnd = m_vSignal.end();
  for (; sigIter != sigEnd; ++sigIter) {
    // check the delay and re-add it to the buffer
    if ((*sigIter).delay > 0) {
      (*sigIter).delay--;
      m_vSignalBuffer.push_back((*sigIter));
      continue;
    }
    // send to groups
    vector<Group*>::iterator groupIter = m_vGroup.begin();
    vector<Group*>::iterator groupEnd = m_vGroup.end();
    for(; groupIter != groupEnd; ++groupIter) {
      if ((*groupIter)->m_iProperties & EM_GROUP_NO_SIGNAL) continue;
      // check the address, NULL is multicast
      if ((*sigIter).reciever != NULL && (*sigIter).reciever != (*groupIter)) continue;
      // call onSignal function i behaviors
      // TODO: should use a hashtable for groups
      //cerr << "size " << (*groupIter)->m_vBehavior.size() << endl;

      // 			vector<Behavior*>::iterator behIter = (*groupIter)->m_vBehavior.begin();
      // 			vector<Behavior*>::iterator behEnd = (*groupIter)->m_vBehavior.end();
      // 			for (; behIter != behEnd; ++behIter) {
      // 				(*behIter)->onSignal((*sigIter).signal, (*sigIter).sender);
      // 			}
      if ((*groupIter)->getBehavior() != NULL) {
	(*groupIter)->getBehavior()->onSignal((*sigIter).signal, (*sigIter).sender);
      }
    }
  }
  m_vSignal.clear();
}
