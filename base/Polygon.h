/***************************************************************************
                          Polygon.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef POLYGON_H
#define POLYGON_H

//#define EM_POLY_FLAT 1
//#define EM_POLY_SMOOTH 2
//#define EM_POLY_HIDDEN 4
#define EM_WIREFRAME 8
#define EM_ALLWAYS_LIT 16
//#define EM_DOUBLE_SIDED 32 // not in use (yet?)
//#define EM_POLY_TRANS 64

#include "Private.h"
#include "EMath.h"

class Shape3D;

/**
 * <p>Polygons are part of Shape3Ds. Polygons are built by first creating
 * a Polygon object and then adding the vertices which define the
 * polygon.</p>
 * <p> The Shape3D is finished by calling countNormals.</p>
 * @see Shape3D
 * @see VertexArray
 * @see PolygonArray
 */
class Polygon {
	public:
		Polygon(Shape3D* s, int size = 3);
		~Polygon();
		/** <p>add(index).</p> */
		void add(int index);
		void add(int index,  float u, float v, float a, float r, float g, float b );
		bool includes(int index);
		void setProperty(int);
		void unsetProperty(int);
		int getProperties();
		void setColor(float a, float r, float g, float b);

		void countNormal();

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
		// vector<PolygonEdge> m_vPolygonEdge;
		vector<unsigned int> m_vIndex;
		vector<Color> m_vColor;
		vector<TexCoord> m_vTexCoord;
		int m_iProperties;
		Shape3D* p_Shape3D;
};

#endif // POLYGON_H
