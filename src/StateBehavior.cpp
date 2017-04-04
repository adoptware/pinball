/***************************************************************************
                          StateBehavior.cpp  -  description
                             -------------------
    begin                : Thu Jan 25 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com


    ========================= Modifications =========================

        Apr. 4, 2017:
            Clear the keyboard on game reset. (c30zD)

***************************************************************************/

#include "Private.h"
#include "StateBehavior.h"
#include "Pinball.h"
#include "Group.h"
#include "Light.h"
#include "Shape3D.h"
#include "Polygon.h"
#include "SoundUtil.h"
#include "BallGroup.h"
#include "Loader.h"
#include "Keyboard.h"

StateItem::StateItem() {
  m_iActSig = -1;
  m_iCollSig = -1;
  m_iSigDelay = 0;
  m_bLight = false;
  m_iUserProperty = PBL_NULL;
  m_iShapeProperty = 0;
  m_iSound = -1;
  m_iMusic = -1;
  m_iDelay = -1;
  m_iDelayState = -1;
  m_iCollState = -1;
  m_iMoveSteps = 0;
  m_iTick = 0;
  m_vtxTr.x = 0;
  m_vtxTr.y = 0;
  m_vtxTr.z = 0;
  m_vtxRot.x = 0;
  m_vtxRot.y = 0;
  m_vtxRot.z = 0;
}

StateItem::~StateItem() {
}

void StateItem::setTr(float x, float y, float z) {
  m_vtxTr.x = x;
  m_vtxTr.y = y;
  m_vtxTr.z = z;
  //m_fTrSpeed = EM_ABS(speed);
}

void StateItem::setRot(float x, float y, float z) {
  m_vtxRot.x = x;
  m_vtxRot.y = y;
  m_vtxRot.z = z;
  //m_fRotSpeed = EM_ABS(speed);
}

void StateItem::addTexCoord(float u, float v) { 
  TexCoord tex;
  tex.u = u;
  tex.v = v;
  m_vTexCoord.push_back(tex); 
}

void StateItem::addShapeEnable(int i, bool e) {
  if (i < 0) return;
  // expand vector
  while (i+1 > (signed)m_vShapeEnable.size()) {
    m_vShapeEnable.push_back(false);
  }
  m_vShapeEnable[i] = e;
}

bool StateItem::getShapeEnable(int i) {
  if (i < 0 || i >= (signed)m_vShapeEnable.size()) return false;
  return m_vShapeEnable[i];
}

StateBehavior::StateBehavior() : Behavior() {
  m_iOwnerBall = -1;
  m_iTick = 0;
  m_iCollisionSafe = 0;
  m_bTexCoord = false;
  m_bMove = false;
  m_bShape = false;
  p_CurrentStateItem = NULL;
  m_vtxTr.x = 0;
  m_vtxTr.y = 0;
  m_vtxTr.z = 0;
  m_vtxRot.x = 0;
  m_vtxRot.y = 0;
  m_vtxRot.z = 0;
  this->setType(PBL_TYPE_STATEBEH);
}

StateBehavior::~StateBehavior(){
}

void StateBehavior::addStateItem(StateItem* stateitem) {
  if (stateitem == NULL) return;
  // insert state
  m_vStateItem.push_back(stateitem);
  if (m_vStateItem.size() == 1) {
    p_CurrentStateItem = stateitem;
  }
}

void StateBehavior::removeStateItem(StateItem* stateitem) {
  if (stateitem == NULL) return;
  if (p_CurrentStateItem == stateitem) {
    p_CurrentStateItem = NULL;
  }

  vector<StateItem*>::iterator iter = m_vStateItem.begin();
  vector<StateItem*>::iterator end = m_vStateItem.end();
  for (; iter != end; iter++) {
    if ((*iter) == stateitem) {
      m_vStateItem.erase(iter);
      return;
    }
  }

  if (m_vStateItem.size() >= 1) {
    p_CurrentStateItem = m_vStateItem[0];
  }
}

StateItem * StateBehavior::getStateItem(int i) {
  if (i < 0 || m_vStateItem.size() <= (unsigned) i ) return NULL;
  return m_vStateItem[i];
}

void StateBehavior::onTick() {
  EmAssert(this->getParent() != NULL, "Parent NULL");
  if (p_CurrentStateItem == NULL) return;

  // initialize origo
  if (m_iTick == 0) {
    this->getParent()->getTranslation(m_vtxTr.x, m_vtxTr.y, m_vtxTr.z);
    this->getParent()->getRotation(m_vtxRot.x, m_vtxRot.y, m_vtxRot.z);
  }

  m_iTick++;
  p_CurrentStateItem->m_iTick++;

  if (p_CurrentStateItem->m_iDelay >= 0 && 
      p_CurrentStateItem->m_iTick >= p_CurrentStateItem->m_iDelay &&
      p_CurrentStateItem->m_iDelayState >= 0) {
    this->setState(this->getStateItem(p_CurrentStateItem->m_iDelayState));
  }
  // the ball recently collided with this object is in a safe distance
  if (m_iTick > m_iCollisionSafe) {
    m_iOwnerBall = -1;
  }
  // move the object to towards the next spot
  // TODO smother move
  if (m_bMove) {
    int steps = p_CurrentStateItem->m_iMoveSteps - p_CurrentStateItem->m_iTick;
			
    if (steps <= 0) {
      this->getParent()->setTransform(m_vtxTr.x + p_CurrentStateItem->m_vtxTr.x,
				      m_vtxTr.y + p_CurrentStateItem->m_vtxTr.y,
				      m_vtxTr.z + p_CurrentStateItem->m_vtxTr.z,
				      m_vtxRot.x + p_CurrentStateItem->m_vtxRot.x,
				      m_vtxRot.y + p_CurrentStateItem->m_vtxRot.y,
				      m_vtxRot.z + p_CurrentStateItem->m_vtxRot.z);
    } else {
      float dtx, dty, dtz, drx, dry, drz;
      float tx, ty, tz, rx, ry, rz;
      this->getParent()->getTranslation(tx, ty, tz);
      this->getParent()->getRotation(rx, ry, rz);

      dtx =  p_CurrentStateItem->m_vtxTr.x + m_vtxTr.x - tx;
      dty =  p_CurrentStateItem->m_vtxTr.y + m_vtxTr.y - ty;
      dtz =  p_CurrentStateItem->m_vtxTr.z + m_vtxTr.z - tz;
      drx =  p_CurrentStateItem->m_vtxRot.x + m_vtxRot.x - rx;
      dry =  p_CurrentStateItem->m_vtxRot.y + m_vtxRot.y - ry;
      drz =  p_CurrentStateItem->m_vtxRot.z + m_vtxRot.z - rz;

      dtx /= steps;	dty /= steps;	dtz /= steps;
      drx /= steps;	dry /= steps;	drz /= steps;
      this->getParent()->addTransform(dtx, dty, dtz, drx, dry, drz);
    }
  }
}

void StateBehavior::setState(StateItem* stateitem) {
  EmAssert(p_CurrentStateItem != NULL, "Current state item NULL");
  if (stateitem == NULL) return;
  StateItem* previtem = p_CurrentStateItem;
  p_CurrentStateItem = stateitem;

  // set properties
  this->getParent()->unsetUserProperty(previtem->m_iUserProperty);
  this->getParent()->setUserProperty(p_CurrentStateItem->m_iUserProperty);
  // defaults to shape 0
  int sindex = 0;
  Shape3D* shape = this->getParent()->getShape3D(sindex);   
  while (shape != NULL) {
    shape->unsetProperty(previtem->m_iShapeProperty);
    shape->setProperty(p_CurrentStateItem->m_iShapeProperty);
    ++sindex;
    shape = this->getParent()->getShape3D(sindex);
  }
  // apply texcoords
  if (m_bTexCoord) {
    // default to shape 0 TODO remove this thing, we can use shapes instead   
    Shape3D* shape = this->getParent()->getShape3D(0);   
    if (shape != NULL) {         
      // only apply to fisrst polygon TODO
      Polygon3D* poly = shape->getPolygon(0);     
      if (poly != NULL) {                       
	vector<TexCoord>::iterator iter1 = p_CurrentStateItem->m_vTexCoord.begin();               
	vector<TexCoord>::iterator end1 = p_CurrentStateItem->m_vTexCoord.end();
	vector<TexCoord>::iterator iter2 = shape->m_vTexCoord.begin();  
	vector<TexCoord>::iterator end2 = shape->m_vTexCoord.end();     
	for (; iter1 != end1 && iter2 != end2; ++iter1, ++iter2) {       
	  (*iter2) = (*iter1);                  
	}                                       
      }                                         
    }
  }
  // look out shapes conflicts with shape properties
  if (m_bShape) {
    vector<bool>::iterator iter = p_CurrentStateItem->m_vShapeEnable.begin();
    vector<bool>::iterator end = p_CurrentStateItem->m_vShapeEnable.end();
    int index = 0;
    Shape3D* shape = this->getParent()->getShape3D(index);
    while (shape != NULL && iter != end) {
      if (*iter) {
	shape->unsetProperty(EM_SHAPE3D_HIDDEN);
      } else {
	shape->setProperty(EM_SHAPE3D_HIDDEN);
      }
      ++index;
      ++iter;
      shape = this->getParent()->getShape3D(index);
    }
  }
  // start music
  if (p_CurrentStateItem->m_iMusic >= 0) {
    SoundUtil::getInstance()->playMusic(p_CurrentStateItem->m_iMusic, true);
  }
  // play sound
  if (p_CurrentStateItem->m_iSound >= 0) {
    SoundUtil::getInstance()->playSample(p_CurrentStateItem->m_iSound, false);
  }
  // apply light
  SetLightOn(p_CurrentStateItem->m_bLight);
  // zero counter
  p_CurrentStateItem->m_iTick = 0;
}

void StateBehavior::StdOnSignal() {
  EmAssert(this->getParent() != NULL, "Parent NULL");
  if (p_CurrentStateItem == NULL && m_vStateItem.size() == 0) return;

  // initialize origo, this is a ugle hack
  if (m_iTick == 0) {
    this->getParent()->getTranslation(m_vtxTr.x, m_vtxTr.y, m_vtxTr.z);
    this->getParent()->getRotation(m_vtxRot.x, m_vtxRot.y, m_vtxRot.z);
  }

#if EM_DEBUG
	if (Loader::getInstance()->getSignal(GetSignal()) != NULL) {
		EM_COUT("Got signal " << Loader::getInstance()->getSignal(GetSignal()), 0);
	}
#endif

  OnSignal( PBL_SIG_RESET_ALL) {
    this->setState(m_vStateItem[0]);
    // do a fast move
    if (m_bMove) {
      this->getParent()->setTransform(m_vtxTr.x + p_CurrentStateItem->m_vtxTr.x,
				      m_vtxTr.y + p_CurrentStateItem->m_vtxTr.y,
				      m_vtxTr.z + p_CurrentStateItem->m_vtxTr.z,
				      m_vtxRot.x + p_CurrentStateItem->m_vtxRot.x,
				      m_vtxRot.y + p_CurrentStateItem->m_vtxRot.y,
				      m_vtxRot.z + p_CurrentStateItem->m_vtxRot.z);
			
    }
    Keyboard::clear();
  } else {
    vector<StateItem*>::iterator iter = m_vStateItem.begin();
    vector<StateItem*>::iterator end = m_vStateItem.end();
    for (; iter != end; iter++) {
      OnSignal((*iter)->m_iActSig) {
	this->setState(*iter);
	break;
      }
    }
  }
}

void StateBehavior::StdOnCollision() {
  if (p_CurrentStateItem == NULL && m_vStateItem.size() == 0) return;

  OnCallerProperty( PBL_BALL ) {
    // this is to avaid one collision to generate more than one signal
    if (m_iTick <= m_iCollisionSafe) {
      m_iCollisionSafe = m_iTick + 5;
      return;
    }
		
    m_iOwnerBall = ((BallGroup*)em_group)->getBall();;
    m_iCollisionSafe = m_iTick + 5;

    //	if (p_CurrentStateItem->m_iSound >= 0) {
    //	SoundUtil::getInstance()->playSample(p_CurrentStateItem->m_iSound, false);
    //	}
    if (p_CurrentStateItem->m_iCollSig >= 0) {
      SendSignal(p_CurrentStateItem->m_iCollSig, p_CurrentStateItem->m_iSigDelay, 
		 this->getParent(), NULL);
    }
    if (p_CurrentStateItem->m_iCollState >= 0) {
      this->setState(this->getStateItem(p_CurrentStateItem->m_iCollState));
    }
    EM_COUT("StateBehavior::StdOnCollision ball", 0);
  }
}
