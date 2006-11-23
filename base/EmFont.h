/***************************************************************************
                          EmFont.h  -  description
                             -------------------
    begin                : Mon Jul 23 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef EMFONT_H
#define EMFONT_H

//#define EM_FONT_X_2 17
//#define EM_FONT_Y_2 13

#ifndef PRIVATE_H
#error Must include Private.h before EmFont.h
#endif

#if EM_USE_SDL
#define EM_FONTSIZE_X 0.06f*EM_RIGHT
#define EM_FONTSIZE_Y 0.08f*EM_UP
#endif

#if EM_USE_ALLEGRO
#define EM_FONTSIZE_X 0.03f
#define EM_FONTSIZE_Y 0.04f
#endif

#include "TextureUtil.h"

class Config;

/** A singleton utility for fonts. */
class EmFont {
 public:
  ~EmFont();
  /** Get singleton object. */
  static EmFont * getInstance();
  void loadFont(const char * fileName);
  /** If row is negative the row will be counted from bottom of screen. */
  void printRow(const char * buffer, float row);
  /** If row is negative the row will be counted from bottom of screen. */
  void printRowCenter(const char * buffer, float row);
 protected:
#if EM_USE_SDL
  /** With OpenGL -1,1 is upper left. 1,-1 is lower right. */
  void print(const char * buffer, float x, float y);
#endif
#if EM_USE_ALLEGRO
  /** With allegro 0,0 is upper lefter. The screen size determines the lower right. */
  void print(const char * buffer, int x, int y);
#endif
  EmFont();
 private:
  //EmImage * m_Font;
  EmTexture * m_Texture;
  int m_iSize;
  float m_fFontSizeX;
  float m_fFontSizeY;
  Config * p_Config;
  static EmFont * p_EmFont;
};

#endif // EMFONT_H
