/***************************************************************************
                          Group.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef GROUP_H
#define GROUP_H

// Define some properties
#define EM_GROUP_TRANSFORM_ONCE  1
#define EM_GROUP_NO_TRANSFORM    2
#define EM_GROUP_LIGHT_ONCE      4
#define EM_GROUP_NO_LIGHT        8
#define EM_GROUP_NO_SIGNAL      16

#include "Private.h"
#include "Node.h"

class Shape3D;
class Visitor;
class Behavior;
class CollisionBounds;
class Sound;
class Camera;
class Light;
class BillBoard;

/** Groups are a essential element when building the 3D world.</p>
 ** All Shape3Ds, Sounds, Lights, Behavoir etc must be added to
 ** Groups ( the Camera is a execption, it is recommended that it is added
 ** to a Group, but this is not requirement for the context to be render ). Groups
 ** are then added to the Engine or added to other Groups that are in
 ** the Engine. Engine itself is the matter a fact a Group.
 ** Groups must form a tree structure, loops will cause the program to hang. There
 ** is currently no check for illegal loop structures, it is up to the programmer
 ** to avoid cyclic graphs.
 **
 ** Example:
 ** <pre>
 ** Legal tree:              Illegal tree:
 **
 **   engine                   engine
 **   /    \                   /   \
 ** group  group           group  group
 **        /   \               \  /
 **     group group           group
 ** </pre>
 **
 ** Usage.
 ** Groups are used to group elements together. Each Group represent a transform that
 ** is applied to all its children.
 ** Example: A butterfly with two wings. The butterfly as a whole is reprented by a Group ( bfGroup ).
 ** To this group we have attached two Groups ( lwGroup and rwGroup ), one for each wing. By adding
 ** a Animation to lwGroup and an other for rwGroup we can make the butterfly flap with the wings.
 ** We can then apply transformation to the bfGroup and make the butterfly to move. We have created
 ** a nice little butterfly who flaps with its wing and flies around</p>
 ** <pre>
 **                  bfGroup
 **                  /    \
 **                 /      \
 ** Animation -- lwGroup   rwGroup -- Animation
 ** </pre> */
class Group : public Node {
 public:
	Group();
	virtual ~Group();
	
	void accept(Visitor * v);
	void add(Group * g);
	
	void addBehavior(Behavior * b, bool signal=true);
	Behavior * getBehavior(int i);
	int getBehaviorSize();
	
	void setBillBoard(BillBoard * b);
	BillBoard * getBillBoard();
	
	void setCamera(Camera * c);
	Camera * getCamera();
	
	void setLight(Light * l);
	Light * getLight();
	
	void setCollisionBounds(CollisionBounds * cb);
	CollisionBounds * getCollisionBounds();
	
	Group * getParent();
	int getProperties();
	
	void addShape3D(Shape3D * s);
	Shape3D * getShape3D(int i);
	int getShape3DSize();
	
	void setSound(Sound * s);
	Sound * getSound();
	
	//		virtual void sendSignal(int signal, Group * sender);
	/** Properties */
	void setProperty(int p);
	void setPropertyRecursive(int p);
	void setUserProperty(int p);
	int getUserProperties();
	
	void unsetProperty(int p);
	void unsetPropertyRecursive(int p);
	void unsetUserProperty(int p);
	
 private:
	friend class BehaviorVisitor;
	friend class AlignVisitor;
	friend class PNormalVisitor;
	friend class TransformVisitor;
	friend class SoundVisitor;
	friend class OctTree;
	friend class RendererVisitor;
	friend class PointLightVisitor;
	friend class CollisionBounds;
	friend class CollisionVisitor;
	friend class AmbientLightVisitor;
	friend class OpenGLVisitor;
	friend class OpenGLTransVisitor;
	friend class SignalSender;
	
	BillBoard* p_BillBoard;
	Camera* p_Camera;
	Light* p_Light;
	Sound* p_Sound;
	CollisionBounds* p_CollisionBounds;
	
	Group* p_Parent;
	vector<Group*> m_vChildren;
	vector<Shape3D*> m_vShape3D;
	vector<Behavior*> m_vBehavior;
	
	int m_iProperties;
	int m_iUserProperties;
	
	//		void propagateSignal(int signal, Group* sender);
	void setParent(Group* g);
};

#endif // GROUP_H
