/***************************************************************************
                          Shape3D.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef SHAPE3D_H
#define SHAPE3D_H

#define EM_SHAPE3D_HIDDEN 1
#define EM_SHAPE3D_USE_TRANS 2
#define EM_SHAPE3D_ALPHATEST 4
#define EM_SHAPE3D_SPECULAR  8
#define EM_SHAPE3D_BEHIND    16
#define EM_SHAPE3D_BEHIND2   32
#define EM_SHAPE3D_ALLWAYSLIT 64
//#define EM_SHAPE3D_FLAT 16
//#define EM_SHAPE3D_DOUBLE 32

#include <vector>

#include "TextureUtil.h"
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
  virtual ~Shape3D();
  /** Creates a new vertex and returns the index. The index is used when 
   * creating polygons. @see Polygon */
  int add(float x, float y, float z);
  int add(float x, float y, float z, float r, float g, float b, float a, float u, float v);
  int addAt(int index, float x, float y, float z,
	    float r, float g, float b, float a, float u, float v);
  /** Adds a polygon. @see Polygon */
  void add(Polygon*);
  /** Counts the polygon normals used for lightning. This must be called when
   * all vertices and polygons are added to the shape. */ 
  void countNormals();
  /** Sets the color of all polygons to rgba. */
  void setColor(float r, float g, float b, float a);
  /** Applies a a property to all polygons. See Polygon class for
   * properties. @see Polygon */
  void setPolygonProperty(int property);
  void unsetPolygonProperty(int property);
  void setProperty(int property);
  void unsetProperty(int property);
  int getProperties() { return m_iProperties; };
  void setTexture(EmTexture * tex);
  EmTexture * getTexture() { return m_Texture; };
  /** Gets the vertex at position index. It is a bit unsafe to reference pointers
   * to vertices as the adding new vertices may change allocation position of
   * vertex. */
  Vertex3D * getVertex3D(int index);
  /** Warning, this function is slow. */
  int getVertex3DIndex(Vertex3D * vtx);
  int getVertex3DIndex(TexCoord * tex);
  int getVertex3DSize();
  /** To be able to remove vertices they polygons first using them must be
   * removed. This function removes the vertex only if it is not used
   * by any polygon. */
  bool removeLooseVertex3D(int vtxindex);
  Polygon* getPolygon(int index); 
  int getPolygonSize();
  /** Warning, this function is slow. */
  int getPolygonIndex(Polygon * poly);
  void removePolygon(Polygon * poly);
  int find(float x, float y, float z, float diff);
  float getCollisionSize();
  void setParent(Group*);
  Group * getParent() { return p_Parent; };
  Color * getColor(int index);
  void setColor(int index, float r, float g, float b, float a);
  TexCoord * getTexCoord(int index);
  void setTexCoord(int index, float u, float v);
  
  vector<Polygon*> m_vPolygon;
  vector<Vertex3D> m_vVtxSrc;
  vector<Vertex3D> m_vVtxTrans;
  vector<Vertex3D> m_vVtxAlign;
  vector<Vertex3D> m_vNmlSrc;
  vector<Vertex3D> m_vNmlTrans;
  vector<Vertex3D> m_vNmlAlign;
  vector<Color> m_vColor;
  vector<Color> m_vLitColor;
  vector<TexCoord> m_vTexCoord;
  vector<Color> m_vLight;
  vector<Color> m_vSpecular;
  
  EmTexture* m_Texture;
  Group* p_Parent;
  int m_iProperties;
};

#endif // SHAPE3D_H
