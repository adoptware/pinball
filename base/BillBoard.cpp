/***************************************************************************
                          BillBoard.cpp  -  description
                             -------------------
    begin                : Sun Dec 16 2001
    copyright            : (C) 2001 by henqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "BillBoard.h"

BillBoard::BillBoard(EmTexture * tex, float sizex, float sizey) {
	EmAssert(tex != NULL, "texture not allowed to be null");

	m_iProperties = 0;
	p_Parent = NULL;
	m_Texture = tex;
	m_fSizexD2 = sizex / 2;
	m_fSizeyD2 = sizey / 2;
	m_fZOffset = 0;

	m_aTexCoord[0].u = 0;
	m_aTexCoord[0].v = 0;
	m_aTexCoord[1].u = 1;
	m_aTexCoord[1].v = 0;
	m_aTexCoord[2].u = 1;
	m_aTexCoord[2].v = 1;
	m_aTexCoord[3].u = 0;
	m_aTexCoord[3].v = 1;

	m_vtxSrc.x = 0;
	m_vtxSrc.y = 0;
	m_vtxSrc.z = 0;
}

BillBoard::~BillBoard() {
}

void BillBoard::setProperty(int p) {
	m_iProperties |= p;
}

void BillBoard::unsetProperty(int p) {
	m_iProperties -= (m_iProperties & p);
}

void BillBoard::setParent(Group* p) {
	p_Parent = p;
}

void BillBoard::setTexture(EmTexture* tex) {
	m_Texture = tex;
}

