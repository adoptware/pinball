/***************************************************************************
                          Node.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef NODE_H
#define NODE_H

#include "EMath.h"

/** The Node represents the transformation of an object. Group inherits this class. */
class Node {
 public:
  Node();
  ~Node();
  void setTransform(float tx, float ty, float tz , float rx, float ry, float rz);
  void addTransform(float tx, float ty, float tz , float rx, float ry, float rz);
  void setTranslation(float x, float y, float z);
  void getTranslation(float & x, float & y, float & z);
  void addTranslation(float x, float y, float z);
  void getTranslation(Vertex3D & vtx) {
    this->getTranslation(vtx.x, vtx.y, vtx.z);
  };
  void setTranslation(Vertex3D & vtx) {
    this->setTranslation(vtx.x, vtx.y, vtx.z);
  };
  inline void addTranslation(Vertex3D & vtx) {
    m_vtxT.x += vtx.x;
    m_vtxT.y += vtx.y;
    m_vtxT.z += vtx.z;
    m_mtxSrc.t[0] = m_vtxT.x;
    m_mtxSrc.t[1] = m_vtxT.y;
    m_mtxSrc.t[2] = m_vtxT.z;
  };			     
  void setRotation(float x, float y, float z);
  void getRotation(float & x, float & y, float & z);
  void addRotation(float x, float y, float z);
  void setScale(float x, float y, float z);
  void getScale(float & x, float & y, float & z);
  void addScale(float x, float z, float z);
  
  Matrix m_mtxSrc;
  Matrix m_mtxTrans;
 private:
  Vertex3D m_vtxT;
  Vertex3D m_vtxR;
  Vertex3D m_vtxS;
};

#endif // NODE_H
