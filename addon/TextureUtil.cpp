/***************************************************************************
                          TexUtil.cpp  -  description
                             -------------------
    begin                : Mon Nov 27 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#include "TextureUtil.h"
#include "SDL/SDL_image.h"

EmImage* TextureUtil::loadImage(char* fileName) {
	Uint8 * rowhi, * rowlo;
  Uint8 * tmpbuf, tmpch;
  SDL_Surface * image = IMG_Load(fileName);
  if ( image == NULL ) {
		cerr << "Unable to load "<< fileName <<" "<< SDL_GetError() << endl;
    return NULL;
	}
	return image;
}

EmTexture* TextureUtil::loadTexture(char* fileName) {
	// Load Texture
	SDL_Surface *image;
    
	image = TextureUtil::loadImage(fileName);
	if (!image) {
		cerr << "error loading file " << fileName << endl;
		return NULL;
	}

	EmTexture* texture = (EmTexture*) malloc(sizeof(EmTexture));

	// Create Texture
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), 
	// x size from image, y size from image, 
	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
	
	int comp;
	int type;

	switch (image->format->BitsPerPixel) {
	case 24: type = GL_UNSIGNED_BYTE; break;
	case 32: type = GL_UNSIGNED_BYTE; break;
	default: type = GL_UNSIGNED_BYTE; cerr << "not 32 bits per pixel" << endl; 
	}
	
	switch (image->format->BytesPerPixel) {
	case 3:	comp = GL_RGB; break;
	case 4: comp = GL_RGBA; break;
	default: comp = GL_RGB; cerr << "unknown image format" << endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, comp, image->w, image->h, 0, comp, type, image->pixels);

	cerr << "loaded texture" << endl;
	cerr << "size " << image->w <<" "<< image->h << endl;
	cerr << "bytes per pixel " << (int)image->format->BytesPerPixel << endl;
	cerr << "bits per pixel " << (int)image->format->BitsPerPixel << endl;

	return texture;
}




