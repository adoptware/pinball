/***************************************************************************
                          Visitor.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef VISITOR_H
#define VISITOR_H

class Group;
/**
 * <p><b>Only for internal use.</b></p>
 * <h2>Description.<h2>
 * <p>Class visitor is base class for all visitors.</p>
 * <p>The purpose of Visitors are the decouple structure form functionality.
 * The structure is represented by objets like Groups Lights Shape3DS etc,
 * these objects only include information of the specific object ( the data).
 * The functionality for manipulation the objets is found found
 * in the Visitor objects.</p>
 * <p>For example the code in the Shape3D class should only deal with
 * storing data for vertices, polygons, colors and textures.
 * The Shape3D class has no functions for drawing its
 * context to the screen. The drawing functionality is encapsulated in the
 * RenderVisitor class.</p>
 * <h2>Implementation.</h2>
 * <p> Visitors are traversed through the context tree ( the engine and
 * all its groups ) to perform a specific task on the objects in the tree.
 * For example transformin vertices, drawing to the screen, collision
 * detection.</p>
 * <p>Traversing is done by calling the 'accept' function of
 * the Group with the Visitor as a parameter. The Group calls the
 * Visitors 'visit' funtion with itself as parameter. The visitor performs
 * its functionality on the Group. The Group has now been transformed, draw
 * or w.</p>
 * <p>The Group then calls all its
 * childrens ( Groups, Shape3Ds ... ) 'accept' function with the same
 * Visitor as parameter. Then children.</p>
 *
 * @see AlignVisitor
 * @see AnimationVisitor
 * @see BehaviorVisitor
 * @see CollisionVisitor
 * @see GLightVisitor
 * @see PNormalVisitor
 * @see VNormalVisitor
 */
class Visitor {
	public:
		Visitor(){};
		virtual ~Visitor(){};
		virtual void visit(Group*){};
		virtual void empty(){};
};

#endif // VISITOR_H
