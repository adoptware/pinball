/***************************************************************************
                          Group.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "Group.h"
#include "Visitor.h"
#include "Shape3D.h"
#include "Behavior.h"
#include "BillBoard.h"
#include "CollisionBounds.h"
#include "Sound.h"
#include "Camera.h"
#include "Polygon.h"
#include "SignalSender.h"

Group::Group() {
	m_iProperties = 0;
	m_iUserProperties = 0;
	p_BillBoard = NULL;
	p_CollisionBounds = NULL;
	p_Sound = NULL;
	p_Camera = NULL;
	p_Light = NULL;
	p_Parent = NULL;
	EM_COUT("Group::Group()" << endl, 0);
}

Group::~Group() {
}

void Group::accept(Visitor * v) {
	// TODO: if property
	v->visit(this);
	vector<Group*>::iterator iter = m_vChildren.begin();
	vector<Group*>::iterator end = m_vChildren.end();
	for ( ; iter != end; iter++) {
		(*iter)->accept(v);
	}
}

void Group::add(Group * g) {
	if (g == NULL) return;
	m_vChildren.push_back(g);
	g->setParent(this);
}

void Group::addShape3D(Shape3D * s) {
	if (s == NULL) return;
	m_vShape3D.push_back(s);
	s->setParent(this);
}

void Group::addBehavior(Behavior * b, bool signal=true) {
	if (b == NULL) return;
	m_vBehavior.push_back(b);
	b->setParent(this);
	if (signal) SignalSender::getInstance()->addGroup(this);
}

Shape3D * Group::getShape3D(int i) {
	if (i < 0 || m_vShape3D.size() <= (unsigned) i ) return NULL;
	return m_vShape3D[i];
}

int Group::getShape3DSize() {
	return m_vShape3D.size();
}


Behavior * Group::getBehavior(int i) {
	if (i < 0 || m_vBehavior.size() <= (unsigned) i ) return NULL;
	return m_vBehavior[i];
}

int Group::getBehaviorSize() {
	return m_vBehavior.size();
}

BillBoard * Group::getBillBoard() {
	return p_BillBoard;
}

Camera * Group::getCamera() {
	return p_Camera;
}

CollisionBounds * Group::getCollisionBounds() {
	return p_CollisionBounds;
}

Light * Group::getLight() {
	return p_Light;
}

Group * Group::getParent() {
	return p_Parent;
}

int Group::getProperties() {
	return m_iProperties;
}

int Group::getUserProperties() {
	return m_iUserProperties;
}

void Group::setBillBoard(BillBoard * b) {
	p_BillBoard = b;
	if (b == NULL) return;
	b->setParent(this);
}

void Group::setCamera(Camera * c) {
	p_Camera = c;
	//c->setParent(this);
}

void Group::setCollisionBounds(CollisionBounds* cb) {
	p_CollisionBounds = cb;
	if (cb == NULL) return;
	cb->setParent(this);
}

void Group::setSound(Sound * s) {
	p_Sound = s;
	if (s == NULL) return;
	s->setParent(this);
}

void Group::setLight(Light * l) {
	p_Light = l;
	//l->setParent(this);
}

void Group::setParent(Group * p) {
	p_Parent = p;
}

void Group::setProperty(int p) {
	m_iProperties |= p;
}

void Group::setUserProperty(int p) {
	m_iUserProperties |= p;
}

/* Sets properties, also set property for all children.
 */
void Group::setPropertyRecursive(int p) {
	m_iProperties |= p;
	vector<Group*>::iterator iter = m_vChildren.begin();
	vector<Group*>::iterator end = m_vChildren.end();
	for ( ; iter != end; iter++) {
		(*iter)->setPropertyRecursive(p);
	}
}

/* Unsets properties.
 */
void Group::unsetProperty(int p) {
	m_iProperties -= (m_iProperties & p);
}

void Group::unsetUserProperty(int p) {
	m_iUserProperties -= (m_iUserProperties & p);
}

/* Unsets properties, also unset property for all children.
 */
void Group::unsetPropertyRecursive(int p) {
	m_iProperties -= (m_iProperties & p);
	vector<Group*>::iterator iter = m_vChildren.begin();
	vector<Group*>::iterator end = m_vChildren.end();
	for ( ; iter != end; iter++) 	{
		(*iter)->unsetPropertyRecursive(p);
	}
}

/* Send the signal up to the root. The root then propagates the signal
 * through the tree and calls the behaviors onSignal methods.
void Group::sendSignal(int signal, Group * sender) {
	if (p_Parent != NULL) {
		p_Parent->sendSignal(signal, sender);
	}
}
*/
/* Send the signal to all children.
 *
void Group::propagateSignal(int signal, Group * sender) {
	if (p_Behavior != NULL) {
		p_Behavior->onSignal(signal, sender);
	}

	vector<Group*>::iterator iter = m_vChildren.begin();
	vector<Group*>::iterator end = m_vChildren.end();
	for ( ; iter != end; iter++) {
		(*iter)->propagateSignal(signal, sender);
	}
} */
