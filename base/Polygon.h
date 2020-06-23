/***************************************************************************
                          Polygon.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef POLYGON_H
#define POLYGON_H

//#define EM_POLY_FLAT 1
//#define EM_POLY_SMOOTH 2
//#define EM_POLY_HIDDEN 4
//#define EM_WIREFRAME 8
//#define EM_ALLWAYS_LIT 16
//#define EM_POLY_DOUBLE 32
#define EM_POLY_TRANS 64
//#define EM_POLY_CCW_VIEW 128

using namespace std;

#include <vector>

#include "EMath.h"

class Shape3D;

/**
 * Polygons are part of Shape3Ds. Polygons are built by first creating
 * a Polygon object and then adding the vertices which define the polygon.
 *
 * Look out i can be a bit confusing to handle two types of indices. Will
 * call the vertex index in the shape for shindex and the index for the edge in the
 * polygon for polyindex. A polyindex edge always has a connection to one shindex vertex.
 * A shindex vertex may be refered by several polygons ie. polyindex edges.
 * @see Shape3D
 */
class Polygon3D {
	public:
		Polygon3D(Shape3D* s, int size = 3);
		~Polygon3D();
		void copy(Polygon3D * poly);
		/** Add add a shape vertex into this polygon- */
		void add(int shindex);
		/** Returns the polyindex if the vertex is included. Else -1. */
		int includes(int shindex);
		void setProperty(int p);
		void unsetProperty(int p);
		int getProperties();
		/** Get shape vertex index for edge in polygon. */
		int getIndex(int polyindex);
		int getIndexSize();
		void setColor(float r, float g, float b, float a);
		Color * getColor(int polyindex);
		void setColor(int polyindex, float r, float g, float b, float a);
		TexCoord * getTexCoord(int polyindex);
		void setTexCoord(int polyindex, float u, float v);
		Shape3D * getShape3D() { return p_Shape3D; };
		void countNormal();
		void decrement(int shindex);
		/** Move vertex at index one step up in polygon order. */
		void moveUp(int polyindex);
		void moveDown(int polyindex);
		/** Are the vertices follow each other in this polygon. */
		bool connected(int shindexA, int shindexB);

/*	private:
		friend class Shape3D;
		friend class RendererVisitor;
		friend class PNormalVisitor;
		friend class PointLightVisitor;
		friend class NCollisionVisitor;
		friend class GLightVisitor;
		friend class NestedBounds;
		friend class ClipperVisitor;
		friend class PaintersSorter;
		friend class OpenGLVisitor;*/
		
		Vertex3D m_nmlSrc;
		Vertex3D m_nmlTrans;
		Color m_colFlatLight;
		vector<int> m_vIndex;
		int m_iProperties;
		Shape3D* p_Shape3D;
};

#endif // POLYGON_H
