/***************************************************************************
                          Font.cpp  -  description
                             -------------------
    begin                : Mon Jul 23 2001
    copyright            : (C) 2001 by Henrik Enqvist IB
    email                : henqvist@excite.com
 ***************************************************************************/

#include "EmFont.h"
#include "TextureUtil.h"
#include <stdio.h>

extern int _em_width_;
extern int _em_height_;

EmFont * EmFont::p_Instance = NULL;

EmFont::EmFont() {
	m_Font = NULL;
	m_Size = 0;
	m_Offset = 0;
}

EmFont::~EmFont() {
}

EmFont * EmFont::getInstance() {
	if (p_Instance == NULL) {
		p_Instance = new EmFont();
	}
	return p_Instance;
}

void EmFont::loadFont(char * fileName) {
	m_Font = TextureUtil::loadImage(fileName);
	m_Size = m_Font->w;
	m_Offset = m_Size * m_Size * 3;
	if (m_Font == NULL) {
		cerr << "Font not found" << endl;
	}
}

void EmFont::print(char * buffer, int x, int y) {
	if (buffer == NULL) return;

	glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

    glLoadIdentity();
    // TODO: true width and height
    glOrtho(0, _em_width_, 0, _em_height_, -1.0, 1.0);

		for (int a=0; a<255 && buffer[a] != 0 && buffer[a] != 10; a++) {
			int b=buffer[a];
			int c=36;
			// a number
			if (47<b && b<58) {
				c = b - 48;
			// a big character
			} else if (64<b && b<91) {
				c = b - 65 + 10;
			// a small character
			} else if (96<b && b<123) {
				c = b - 97 + 10;
			} else {
				c=36;
			}
			//		cerr << b <<" "<< c <<" "<< (char)b << endl;
			if (m_Font != NULL) {
		  	glMatrixMode(GL_MODELVIEW);
	  	  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		    glRasterPos2i(x, y);
				glPixelZoom(1.0, -1.0); // GL has 0,0 at bottom left, images must be flipped
				glDrawPixels(m_Size, m_Size, GL_RGB, GL_UNSIGNED_BYTE, 
										 &(((GLubyte*)m_Font->pixels)[m_Offset*c]));
			} else {
				cerr << "No font defined" << endl;
			}
			x += m_Size;
		}

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}
