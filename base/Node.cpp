/***************************************************************************
                          Node.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "Node.h"

/*
 */
Node::Node() {
	m_mtxSrc = identityMatrix;
	m_mtxTrans = identityMatrix;
}

/*
 */
Node::~Node() {
}

/*
 */
void Node::setTransform(float tx, float ty, float tz, float rx, float ry, float rz) {
	t_vtxR.x = rx;
	t_vtxR.y = ry;
	t_vtxR.z = rz;
	t_vtxT.x = tx;
	t_vtxT.y = ty;
	t_vtxT.z = tz;
	EMath::getTransformationMatrix(m_mtxSrc, t_vtxT, t_vtxR);
}

/*
 */
void Node::setRotation(float x, float y, float z) {
	t_vtxR.x = x;
	t_vtxR.y = y;
	t_vtxR.z = z;
	t_vtxT.x = m_mtxSrc.t[0];
	t_vtxT.y = m_mtxSrc.t[1];
	t_vtxT.z = m_mtxSrc.t[2];
	EMath::getTransformationMatrix(m_mtxSrc, t_vtxT, t_vtxR);
}

/*
 */
void Node::setTranslation(float x, float y, float z) {
	m_mtxSrc.t[0] = x;
	m_mtxSrc.t[1] = y;
	m_mtxSrc.t[2] = z;
}

/*
 */
void Node::getTranslation(float & x, float & y, float & z) {
	x = m_mtxSrc.t[0];
	y = m_mtxSrc.t[1];
	z = m_mtxSrc.t[2];
}

/*
 */
void Node::addTranslation(float x, float y, float z) {
	m_mtxSrc.t[0] += x;
	m_mtxSrc.t[1] += y;
	m_mtxSrc.t[2] += z;
}
