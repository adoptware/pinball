/***************************************************************************
                          Node.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "Node.h"

Node::Node() {
	m_mtxSrc = EMath::identityMatrix;
	m_mtxTrans = EMath::identityMatrix;
	m_vtxT.x = 0;
	m_vtxT.y = 0;
	m_vtxT.z = 0;
	m_vtxR.x = 0;
	m_vtxR.y = 0;
	m_vtxR.z = 0;
}

Node::~Node() {
}

void Node::setTransform(float tx, float ty, float tz, float rx, float ry, float rz) {
	m_vtxT.x = tx;
	m_vtxT.y = ty;
	m_vtxT.z = tz;
	m_vtxR.x = rx;
	m_vtxR.y = ry;
	m_vtxR.z = rz;
	EMath::getTransformationMatrix(m_mtxSrc, m_vtxT, m_vtxR);
}

void Node::addTransform(float tx, float ty, float tz, float rx, float ry, float rz) {
	m_vtxT.x += tx;
	m_vtxT.y += ty;
	m_vtxT.z += tz;
	m_vtxR.x += rx;
	m_vtxR.y += ry;
	m_vtxR.z += rz;
	EMath::getTransformationMatrix(m_mtxSrc, m_vtxT, m_vtxR);
}

void Node::setRotation(float x, float y, float z) {
	m_vtxR.x = x;
	m_vtxR.y = y;
	m_vtxR.z = z;
	EMath::getTransformationMatrix(m_mtxSrc, m_vtxT, m_vtxR);
}

void Node::getRotation(float & x, float & y, float & z) {
	x = m_vtxR.x;
	y = m_vtxR.y;
	z = m_vtxR.z;
}

void Node::addRotation(float x, float y, float z) {
	m_vtxR.x += x;
	m_vtxR.y += y;
	m_vtxR.z += z;
	EMath::getTransformationMatrix(m_mtxSrc, m_vtxT, m_vtxR);
}

void Node::setTranslation(float x, float y, float z) {
	m_vtxT.x = x;
	m_vtxT.y = y;
	m_vtxT.z = z;
	m_mtxSrc.t[0] = x;
	m_mtxSrc.t[1] = y;
	m_mtxSrc.t[2] = z;
}

void Node::getTranslation(float & x, float & y, float & z) {
	x = m_vtxT.x;
	y = m_vtxT.y;
	z = m_vtxT.z;
}

void Node::addTranslation(float x, float y, float z) {
	m_vtxT.x += x;
	m_vtxT.y += y;
	m_vtxT.z += z;
	m_mtxSrc.t[0] = m_vtxT.x;
	m_mtxSrc.t[1] = m_vtxT.y;
	m_mtxSrc.t[2] = m_vtxT.z;
}
