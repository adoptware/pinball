//#ident "$Id: Behavior.h,v 1.7 2003/05/27 11:53:24 rzr Exp $"
/***************************************************************************
                          Behavior.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist GPL
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "EMath.h"

class Group;
class Light;

/** You add behavior to your objects by creating a subclass to
 * the Behavior class and adding it to a group. All your AI-code, 
 * keyboard-logic should be in a behavior object (or be called from).
 * Important! On each engine tick the 'onTick' function is called first,
 * then the eventual 'onCollision' functions are called. And at last
 * the signals in the signal queue is sent to the 'onSignal' function.
 * Remember the order in which these functions are called when you debug
 * your behaviors. */
class Behavior {
 public:
  /** Simple constructor. You are supposed to create subclasses
   * and add the subclasses to the group. */
  Behavior();
  virtual ~Behavior() {}
  /** reset (needed for static libs) */
  virtual void clear() { m_iType = 0; }
  /** Override this method to create behavior that occurs on each render loop. */
  virtual void onTick() = 0;
  /** Override this method to create behavior that occurs on a collision.
   * @see Shape3D
   * @see CollisionBounds */
  virtual void onCollision(const Vertex3D & vtxWall, const Vertex3D & vtxOwn, Group * g) = 0;
  /** Called each time a signal i sent through the system.
   * Override this method to create behavior that occurs on a signal. */
  virtual void onSignal(int signal, Group * sender) = 0;
  /** Adds a light for subclasses to use. The light must before be added to a group.
   * and to the engine. */
  inline void setLight(Light * l) { p_Light = l; };
  /** Returns to light added the behavior. Returns NULL if no light added. */
  inline Light * getLight() { return p_Light; };
  /** Gets the user defined type for this behavior. Subclasses should set
   * type to a unique id. Defaults to 0. */ 
  inline int getType() { return m_iType; };
  /** Gets the parent group for this behavior. The behavior must be added to
   * a group before we can use this. */
  inline Group * getParent() { return p_Parent; };
 protected:
  /** Subclasses uses this funcition to set the behavior type. */
  inline void setType(int t) { m_iType = t; };
 private:
  friend class Group;
  /** Only for internal use. The Group class uses this when a
   * behavior is added to the group. */
  inline void setParent(Group * g) { p_Parent = g;	};
  Light * p_Light;
  Group * p_Parent;
  int m_iType;
};

#endif // BEHAVIOR_H
