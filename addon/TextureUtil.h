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

/**
 */

class TextureUtil {
	public:
  	static EmTexture* loadTexture(char* file);
	  static EmImage* loadImage(char* file);
};

#endif // TEXTUREUTIL_H
