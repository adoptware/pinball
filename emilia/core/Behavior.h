/***************************************************************************
                          Behavior.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "Private.h"
#include "EMath.h"

class Group;
class Light;

/**
 * <p>You add behavior to your objects by creating subclasses to
 * behavior and adding it to a group. All your AI-code, keyboard-logic
 * should be in a behavior object ( or be called from ).</p>
 *
 * <h2>Usage.</h2>
 * <p>Under construction.</p>
 */
class Behavior {
 public:
	/** Simple constructor. You are supposed to create subclasses
	 * and add the subclasses to the group. */
	Behavior();
	/** <p>Destructor.</p> */
	virtual ~Behavior() {};
	/** Override this method to create behavior that occurs on each render loop. */
	virtual void onTick() = 0;
	/** <p>Override this method to create behavior that occurs on a collision.</p>
	 * @see Shape3D
	 * @see CollisionBounds */
	virtual void onCollision(const Vertex3D & vtxWall, const Vertex3D & vtxOwn, Group * g) = 0;
	/** Called each time a signal i sent through the system.
	 * Override this method to create behavior that occurs on a signal. */
	virtual void onSignal(int signal, Group * sender) = 0;
	/** Adds a light for subclasses to use. */
	void setLight(Light * l) { p_Light = l; };
 protected:
	Light* p_Light;
	Group* p_Parent;
	int m_iState[256];
	private:
	friend class Group;
	/** Only for internal use. The Group class uses this when a
	 * behavior is added to the group. */
	void setParent(Group*);
};

#endif // BEHAVIOR_H
