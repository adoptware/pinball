/***************************************************************************
                          StateMachine.h  -  description
                             -------------------
    begin                : Fri Jul 13 2001
    copyright            : (C) 2001 by Henrik Enqvist IB
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "SignalSender.h"
/* Macros to do a state machine */

#define StdOnCollision()  onCollision(const Vertex3D & em_vtx, const Vertex3D & em_vtxOwn, Group* em_group)
#define StdOnSignal() 		onSignal(int em_signal, Group* em_group)
#define StdOnTick()       onTick()

#define OnCallerProperty(a)	if ( em_group != NULL && ( em_group->getUserProperties() & a ) )
#define OR_CP							|| em_group->getUserProperties() &
#define AND_CP						&& em_group->getUserProperties() &

#define OnProperty(a)			if ( p_Parent != NULL && ( p_Parent->getUserProperties() & a ) )
#define SetProperty(a)		if ( p_Parent != NULL ) p_Parent->setUserProperty(a)
#define UnsetProperty(a)	if ( p_Parent != NULL ) p_Parent->unsetUserProperty(a)
#define OR_PR							|| p_Parent->getUserProperties() &
#define AND_PR						&& p_Parent->getUserProperties() &

#define OnState(a) 				if ( m_iState == a )
#define OR_ST							|| m_iState ==
//#define AND_ST						&& m_iState ==
#define SetState(a) 			m_iState = a

#define OnSignal(a) 			if ( em_signal == a )
#define OR_SI							|| em_signal ==
//#define AND_SI						&& em_signal ==
#define SendSignal(a, d, s, r)  SignalSender::getInstance()->addSignal(a, d, s, r)

#define SetLightOn()			if (p_Light != NULL) p_Light->setOn(true)
#define SetLightOff()			if (p_Light != NULL) p_Light->setOn(false)

#endif // STATEMACHINE_H
