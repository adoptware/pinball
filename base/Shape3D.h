/***************************************************************************
                          Shape3D.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef SHAPE3D_H
#define SHAPE3D_H

//#define E_ALLWAYS_LIT 1
#define EM_SHAPE3D_HIDDEN 1
#define EM_SHAPE3D_TRANS 2
#define EM_SHAPE3D_ALPHATEST 4
#define EM_SHAPE3D_SPECULAR 8
#define EM_SHAPE3D_FLAT 16

#include "Private.h"

#include "EMath.h"

class Group;
class Polygon;

/** A Shape3D represents the visual part of a object.
 * First vertices are added the the shape with the
 * 'add(x, y, z)' method. Then polygons separately created and added
 * with the 'add(Polygon*)' method. The Shape3D is finished off by calling
 * countNormals().
 * @see Polygon */
class Shape3D {
	public:
		Shape3D(int v = 6, int p = 2);
		~Shape3D();
		int add(float x, float y, float z);
		void add(Polygon*);
		void countNormals();
		void setColor(float a, float r, float g, float b);
		void setPolygonProperty(int property);
		void unsetPolygonProperty(int property);
		void setProperty(int property);
		void unsetProperty(int property);
		void setTexture(EmTexture * tex);
		Vertex3D * getVertex3D(int index);
		int find(float x, float y, float z, float diff);
		float getCollisionSize();
		void setParent(Group*);

		vector<Polygon*> m_vPolygon;
		vector<Vertex3D> m_vVtxSrc;
		vector<Vertex3D> m_vVtxTrans;
		vector<Vertex3D> m_vVtxAlign;
		vector<Vertex3D> m_vNmlSrc;
		vector<Vertex3D> m_vNmlTrans;
		vector<Vertex3D> m_vNmlAlign;
		vector<Color> m_vLight;
		vector<Color> m_vSpecular;

		EmTexture* m_Texture;
		Group* p_Parent;
		int m_iProperties;
};

#endif // SHAPE3D_H
