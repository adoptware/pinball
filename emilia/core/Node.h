/***************************************************************************
                          Node.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef NODE_H
#define NODE_H

#include "Private.h"

#include "EMath.h"
/**
 * <p>The Node represents the transformation of an object. Classes Camera
 * Light and Group inherits this class.</p>
 */
class Node {
	public:
		Node();
		~Node();
		/** <p>.</p> */
		void setTransform(float tx, float ty, float tz , float rx, float ry, float rz);
		/** <p>.</p> */
		void setTranslation(float x, float y, float z);
		/** <p>.</p> */
		void getTranslation(float & tx, float & ty, float & tz);
		/** <p>.</p> */
		void addTranslation(float x, float y, float z);
		/** <p>.</p> */
		void setRotation(float x, float y, float z);

		Matrix m_mtxSrc;
		Matrix m_mtxTrans;
	private:
		/* Temporary vertices used for caclulation */
		Vertex3D t_vtxT;
		Vertex3D t_vtxR;

};

#endif // NODE_H
