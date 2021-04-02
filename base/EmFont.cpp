/***************************************************************************
                          Font.cpp  -  description
                             -------------------
    begin                : Mon Jul 23 2001
    copyright            : (C) 2001 by Henrik Enqvist IB
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "EmFont.h"
#include "TextureUtil.h"
#include "Config.h"
#include <cstring>
#include <string>
#include <iostream>

// extern int em_width_;
// extern int em_height_;
// extern int em_width_div2_;
// extern int em_height_div2_;


EmFont * EmFont::p_EmFont = NULL;

EmFont::EmFont() {
  p_Config = Config::getInstance();
  m_Texture = NULL;
  m_iSize = 0;
}

EmFont::~EmFont() {
}

EmFont * EmFont::getInstance() {
  if (p_EmFont == NULL) {
    p_EmFont = new EmFont();
  }
  return p_EmFont;
}

/* All fonts must be 8x8 character bitmaps, where each character is SIZExSIZE pixels */
void EmFont::loadFont(const char * filename) {
  m_Texture = TextureUtil::getInstance()->loadTexture(filename);
  if (m_Texture == NULL) {
    cerr << "Font file not found: " << filename << endl;
    return;
  }
#if EM_USE_SDL
  m_iSize = 32;
#endif
#if EM_USE_ALLEGRO
  m_iSize = (m_Texture->w >> 3);
#endif
}

/*
float EmFont::getRelativeSizeX() {
#if EM_USE_SDL
return 0.06f;
#endif
#if EM_USE_ALLEGRO
return 0.05f;
#endif
}

float EmFont::getRelativeSizeY() {
#if EM_USE_SDL
return 0.08f;
#endif
#if EM_USE_ALLEGRO
return 0.08f;
#endif
}
*/

void EmFont::printRow(const char * buffer, float row) {
  if (buffer == NULL) return;
  if (m_Texture == NULL) {
    cerr << "No font defined" << endl;
    return;
  }
#if EM_USE_SDL
  if (row > -0.0001f) {
    // count rows from top of screen
    this->print(buffer, -EM_RIGHT, EM_UP - row*EM_FONTSIZE_Y);
  } else {
    // count rows from bottom of screen
    this->print(buffer, -EM_RIGHT, -EM_UP - row*EM_FONTSIZE_Y);
  }
#endif
#if EM_USE_ALLEGRO
  if (row > -0.0001f) {
    this->print(buffer, 0, (int)(row*(float)p_Config->getHeight()*EM_FONTSIZE_Y));
  } else {
    this->print(buffer, 0, (int)(p_Config->getHeight() +
				 row*(float)p_Config->getHeight()*EM_FONTSIZE_Y));
  }
#endif
}

void EmFont::printRowCenter(const char * buffer, float row) {
  if (buffer == NULL) return;
  if (m_Texture == NULL) {
    cerr << "No font defined" << endl;
    return;
  }
#if EM_USE_SDL
  if (row > -0.0001f) {
    this->print(buffer, -(float)strlen(buffer)*EM_FONTSIZE_X/2, EM_UP - row*EM_FONTSIZE_Y);
  } else {
    this->print(buffer, -(float)strlen(buffer)*EM_FONTSIZE_X/2, -EM_UP - row*EM_FONTSIZE_Y);
  }
#endif
#if EM_USE_ALLEGRO
  if (row > -0.0001f) {
    this->print(buffer, p_Config->getWidthDiv2() - 
		(int)((float)strlen(buffer)*(float)p_Config->getHeightDiv2()*EM_FONTSIZE_Y), 
		(int)(row*(float)p_Config->getHeight()*EM_FONTSIZE_Y));
  } else {
    this->print(buffer, p_Config->getWidthDiv2() - 
		(int)((float)strlen(buffer)*(float)p_Config->getHeightDiv2()*EM_FONTSIZE_Y),
		(int)(p_Config->getHeight() + 
		      (int)(row*(float)p_Config->getHeight()*EM_FONTSIZE_Y)));
  }
#endif
}

#if EM_USE_SDL
void EmFont::print(const char * buffer, float x, float y) {
  // TODO don't need this check as the function is protected
  if (buffer == NULL) return;
  if (m_Texture == NULL) {
    cerr << "No font defined" << endl;
    return;
  }
  
  int filter = Config::getInstance()->getGLFilter();
  if (filter == -1) filter = GL_NEAREST;
  
	glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, *(m_Texture));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  #ifdef HAVE_OPENGLES
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  float verts[20];
  glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), NULL);
  glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (const float *)NULL + 2);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  #endif
  for (int a=0; a<255 && buffer[a] != 0 && buffer[a] != 10; a++) {
    int b = buffer[a];
    int c = 36;
    if (47<b && b<58) { // a number
      c = b - 48;
    } else if (64<b && b<91) { // a big character
      c = b - 65 + 10;
    } else if (96<b && b<123) {	// a small character
      c = b - 97 + 10;
    } else if (32<b && b<48) { // special character
      c = b - 32 + 35;
    } else if (57<b && b<65) { // special character
      c = b - 57 + 50;
    } else if (b==32) {	// space
      c=63;
    } else {
      c=56;
    }
    float u = (float)(c & 0x7)/8;
    float v = (float)(c >> 3)/8;
    //cerr << b <<" "<< c <<" "<< (char)b <<" "<< u <<" "<< v << endl;

    float ratio = Config::getInstance()->getRatio();
    if (ratio >= 1 || ratio <=0) { ratio = 1.; }
    float xbottom = (x + a*EM_FONTSIZE_X)*ratio;
    float xtop = (x + (a+1)*EM_FONTSIZE_X)*ratio;
    #ifdef HAVE_OPENGLES
    verts[0]  = u; verts[1] = v;
    verts[2]  = xbottom; verts[3] = y; verts[4] = -1;
    verts[5]  = u+0.125; verts[6] = v;
    verts[7]  = xtop; verts[8] = y; verts[9] = -1;
    verts[10] = u+0.125; verts[11] = v+0.125;
    verts[12] = xtop; verts[13] = y-EM_FONTSIZE_Y; verts[14] = -1;
    verts[15] = u; verts[16] = v+0.125;
    verts[17] = xbottom; verts[18] = y-EM_FONTSIZE_Y; verts[19] = -1;
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    #else
    glBegin(GL_QUADS);
    glTexCoord2f(u, v);
    glVertex3f(xbottom, y, -1); 
    glTexCoord2f(u+0.125, v);
    glVertex3f(xtop, y, -1); 
    glTexCoord2f(u+0.125, v+0.125);
    glVertex3f(xtop, y-EM_FONTSIZE_Y, -1);
    glTexCoord2f(u, v+0.125);
    glVertex3f(xbottom, y-EM_FONTSIZE_Y, -1);
    glEnd();
    #endif
    //			cerr << "sork" << buffer[a] << endl;
  }
  #ifdef HAVE_OPENGLES
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDeleteBuffers(1, &vbo);
  #endif
}
#endif // EM_USE_SDL

#if EM_USE_ALLEGRO
void EmFont::print(const char * buffer, int x, int y) {
  // TODO don't need this check as the function is protected
  if (buffer == NULL) return;
  if (m_Texture == NULL) {
    cerr << "No font defined" << endl;
    return;
  }
  int realsizeX = (int)((float)p_Config->getWidth()*EM_FONTSIZE_X);
  int realsizeY = (int)((float)p_Config->getHeight()*EM_FONTSIZE_Y);
  for (int a=0; a<255 && buffer[a] != 0 && buffer[a] != 10; a++) {
    int b=buffer[a];
    int c=36;
    if (47<b && b<58) { // a number
      c = b - 48;
    } else if (64<b && b<91) { // a big character
      c = b - 65 + 10;
    } else if (96<b && b<123) {	// a small character
      c = b - 97 + 10;
    } else if (32<b && b<48) { // special character
      c = b - 32 + 35;
    } else if (57<b && b<65) { // special character
      c = b - 57 + 50;
    } else if (b==32) {	// space
      c=63;
    } else {
      c=56;
    }
    // (c & 0x7) is the column and (c >> 3) is the row, multiply them with font size
    int u = (c & 0x7) * m_iSize;
    int v = (c >> 3) * m_iSize;

    //cerr << "x " << u << " y " << v << endl;
    masked_stretch_blit(m_Texture, backbuffer, u, v, m_iSize, m_iSize,
			x + a*realsizeX, y, realsizeX, realsizeY);
  }	
}
#endif // EM_USE_ALLEGRO
