//#ident "$Id: Group.cpp,v 1.10 2003/06/13 13:39:45 rzr Exp $"
/***************************************************************************
                          Group.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
***************************************************************************/

#include "Private.h"
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
#include "Light.h"

Group::Group() {
  m_iProperties = 0;
  m_iUserProperties = 0;
  m_iCollisionGroup = 0;
  strncpy(m_Name, "unknown", 256);
  p_BillBoard = NULL;
  p_CollisionBounds = NULL;
  p_Sound = NULL;
  p_Camera = NULL;
  p_Light = NULL;
  p_Parent = NULL;
  m_Name[0] = '\0';
  m_vChildren.clear();
  m_vShape3D.clear();
  //m_vBehavior.clear();
  p_Behavior = NULL;	

  EM_COUT("Group::Group()" << endl, 0);
}

Group::~Group() {
  this->freeObjects();
}

void Group::freeObjects() {
  vector<Group*>::iterator giter = m_vChildren.begin();
  vector<Group*>::iterator gend = m_vChildren.end();
  for (; giter != gend; ++giter) {
    EmAssert(*giter != NULL, "Group::freeObjects group NULL");
    delete (*giter);
  }
  m_vChildren.clear();

  vector<Shape3D*>::iterator siter = m_vShape3D.begin();
  vector<Shape3D*>::iterator send = m_vShape3D.end();
  for (; siter != send; ++siter) {
    EmAssert(*siter != NULL, "Group::freeObjects shape NULL");
    delete (*siter);
  }
  m_vShape3D.clear();

  // 	vector<Behavior*>::iterator biter = m_vBehavior.begin();
  // 	vector<Behavior*>::iterator bend = m_vBehavior.end();
  // 	for (; biter != bend; ++biter) {
  // 		EmAssert(*biter != NULL, "Group::freeObjects behavior NULL");
  // 		delete (*biter);
  // 	}
  // 	m_vBehavior.clear();
#ifdef RZR_LIBSTATIC
  if (p_Behavior != NULL) p_Behavior->clear(); //!+-rzr reuse allocated obj
#else
  if (p_Behavior != NULL) delete p_Behavior;
#endif
  if (p_BillBoard != NULL) delete p_BillBoard;
  if (p_Camera != NULL) delete p_Camera;
  if (p_Light != NULL) delete p_Light;
  if (p_Sound != NULL) delete p_Sound;
  if (p_CollisionBounds != NULL) delete p_CollisionBounds;
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

void Group::setName(const char * name) { 
  strncpy(m_Name, name, 256); 
}

const char * Group::getName() { 
  return m_Name; 
}

void Group::add(Group * g) {
  if (g == NULL) return;
  m_vChildren.push_back(g);
  g->setParent(this);
}

void Group::removeGroup(Group * g) {
  if (g == NULL) return;
  vector<Group*>::iterator iter = m_vChildren.begin();
  vector<Group*>::iterator end = m_vChildren.end();
  for ( ; iter != end; iter++) {
    if ((*iter) == g) {
      m_vChildren.erase(iter);
      return;
    }
  }
}

Group * Group::getGroup(int i) {
  if (i < 0 || (signed)m_vChildren.size() <= i) return NULL;
  return m_vChildren[i];
}

void Group::addShape3D(Shape3D * s) {
  if (s == NULL) return;
  m_vShape3D.push_back(s);
  s->setParent(this);
}

void Group::removeShape3D(Shape3D * s) {
  if (s == NULL) return;
  vector<Shape3D*>::iterator iter = m_vShape3D.begin();
  vector<Shape3D*>::iterator end = m_vShape3D.end();
  for (; iter != end; iter++) {
    if ((*iter) == s) {
      m_vShape3D.erase(iter);
      return;
    }
  }
}


#ifdef PINBALL_TODO
void Group::addBehavior(Behavior * b, bool signal)
{
  PINBALL(FUNCT());
 	if (b == NULL) return;
 	m_vBehavior.push_back(b);
 	b->setParent(this);
 	if (signal) SignalSender::getInstance()->addGroup(this);
 }
#endif
void Group::setBehavior(Behavior * b, bool signal)
{
  p_Behavior = b;
  b->setParent(this);
  if (b != NULL && signal) {
    SignalSender::getInstance()->addGroup(this);
  }
}

#ifdef PINBALL_TODO
void Group::removeBehavior(Behavior * b) {
	if (b == NULL) return;
	vector<Behavior*>::iterator iter = m_vBehavior.begin();	vector<Behavior*>::iterator end = m_vBehavior.end();
	for (; iter != end; iter++) {
		if ((*iter) == b) {
			m_vBehavior.erase(iter);
			return;
		}
	}
}
#endif

Shape3D * Group::getShape3D(int i) {
  if (i < 0 || (signed) m_vShape3D.size() <=  i) return NULL;
  return m_vShape3D[i];
}

int Group::getShape3DSize() {
  return m_vShape3D.size();
}


#ifdef PINBALL_TODO
Behavior * Group::getBehavior(int i) {
	if (i < 0 || (signed) m_vBehavior.size() <= i) return NULL;
	return m_vBehavior[i];
}

int Group::getBehaviorSize() {
	return m_vBehavior.size();
}
#endif

Behavior * Group::getBehavior() {
  return p_Behavior;
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
  if (l == NULL) return;
  l->setParent(this);
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

/* Sets properties, also set property for all children. */
void Group::setPropertyRecursive(int p) {
  m_iProperties |= p;
  vector<Group*>::iterator iter = m_vChildren.begin();
  vector<Group*>::iterator end = m_vChildren.end();
  for ( ; iter != end; iter++) {
    (*iter)->setPropertyRecursive(p);
  }
}

void Group::unsetProperty(int p) {
  m_iProperties -= (m_iProperties & p);
}

void Group::unsetUserProperty(int p) {
  m_iUserProperties -= (m_iUserProperties & p);
}

/* Unsets properties, also unset property for all children. */
void Group::unsetPropertyRecursive(int p) {
  m_iProperties -= (m_iProperties & p);
  vector<Group*>::iterator iter = m_vChildren.begin();
  vector<Group*>::iterator end = m_vChildren.end();
  for ( ; iter != end; iter++) 	{
    (*iter)->unsetPropertyRecursive(p);
  }
}
//TODO: Compute Bounds //!rzr
//EOF $Id: Group.cpp,v 1.10 2003/06/13 13:39:45 rzr Exp $
