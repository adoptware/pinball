/***************************************************************************
                          TextureUtil.h  -  description
                             -------------------
    begin                : Mon Nov 27 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef TEXTUREUTIL_H
#define TEXTUREUTIL_H

#include "Private.h"
#include <map>
#include <string>

/**
 */

class TextureUtil {
 public:
	~TextureUtil();
	static TextureUtil* getInstance();
	EmTexture* loadTexture(char* file);
	EmImage* loadImage(char* file);
 protected:
	TextureUtil();
 private:
	map<string, EmTexture*> m_hEmTexture;
	static TextureUtil* p_TextureUtil;
};

#endif // TEXTUREUTIL_H
