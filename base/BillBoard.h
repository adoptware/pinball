/***************************************************************************
                          BillBoard.h  -  description
                             -------------------
    begin                : Sun Dec 16 2001
    copyright            : (C) 2001 by henqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef BILLBOARD_H
#define BILLBOARD_H

#define EM_BILLBOARD_TRANS 1
#define EM_BILLBOARD_ALPHATEST 2

#include "TextureUtil.h"
#include "EMath.h"

class Group;

/** A billboard is a flat polygon that always will face the viewer.
 * An easy speed hack is to use a billboards instead of a complex 
 * shapes, e.g. the monsters in Doom. */
class BillBoard {
 public:
	BillBoard(EmTexture * tex, float sizex, float sizey);
	~BillBoard();
	void setTexture(EmTexture * tex);
	void setZOffset(float z) { m_fZOffset = z; };
	void setSize(float x, float y) { m_fSizexD2 = x/2; m_fSizeyD2 = y/2; };
	void setParent(Group*);
	void setUV(float u, float v, int i) { m_aTexCoord[i].u = u; m_aTexCoord[i].v = v; };
	void setProperty(int property);
	void unsetProperty(int property);

	Vertex3D m_vtxSrc;
	Vertex3D m_vtxTrans;
	Vertex3D m_vtxAlign;
 private:
	friend class OpenGLVisitor;
	friend class AllegroVisitor;
	
	EmTexture* m_Texture;
	Group* p_Parent;
	int m_iProperties;
	float m_fSizexD2, m_fSizeyD2;
	float m_fZOffset;
	TexCoord m_aTexCoord[4];
};


#endif // BILLBOARD_H
