/***************************************************************************
                          TextureUtil.h  -  description
                             -------------------
    begin                : Mon Nov 27 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef TEXTUREUTIL_H
#define TEXTUREUTIL_H

#ifndef PRIVATE_H
#error Must include Private.h before TextureUtil.h
#endif

#if EM_USE_SDL
#include <GL/gl.h>

typedef struct {
  unsigned int width;
  unsigned int height;
  unsigned int channels;
  unsigned char* pixels;
} struct_image;

#define EmImage struct_image
#define EmTexture GLuint
#endif // EM_USE_SDL

#if EM_USE_ALLEGRO
#define EmImage BITMAP
#define EmTexture BITMAP

#include <allegro.h>
extern BITMAP * backbuffer;
extern ZBUFFER * zbuffer;
#endif // EM_USE_ALLEGRO

#define EM_RIGHT 0.8f
#define EM_UP    0.6f
#define EM_NEAR  1.0f
#define EM_FAR   1000.0f

#include <map>
#include <string>

#include "EMath.h"

/** Singleton class for initializing graphics and loading textures. */
class TextureUtil {
 protected:
  TextureUtil();
 public:
  ~TextureUtil();
  static TextureUtil* getInstance();
  void initGrx();
  void stopGrx();
  // TODO 
  /// just unload the textures from main mem (needed w32)
  void freeTextures(); 
  void resizeView(unsigned int w, unsigned int h);
  EmTexture* loadTexture(const char* file);
  EmImage* loadImage(const char* file);
  const char * getTextureName(EmTexture * tex);
  void setClearColor(float r, float g, float b, float a);
 private:
  map<EmTexture*, string> m_hImageName;
  map<string, EmTexture*> m_hEmTexture;
  static TextureUtil* p_TextureUtil;
  Color m_colClear;
};

#endif // TEXTUREUTIL_H
