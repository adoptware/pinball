/***************************************************************************
                          Font.cpp  -  description
                             -------------------
    begin                : Mon Jul 23 2001
    copyright            : (C) 2001 by Henrik Enqvist IB
    email                : henqvist@excite.com
 ***************************************************************************/

#include "EmFont.h"
#include "TextureUtil.h"
#include "Config.h"
#include <stdio.h>
#include <string>


extern int em_width_;
extern int em_height_;

EmFont * EmFont::p_EmFont = NULL;

EmFont::EmFont() {
	m_Texture = NULL;
	//m_Font = NULL;
	m_Size = 0;
}

EmFont::~EmFont() {
}

EmFont * EmFont::getInstance() {
	if (p_EmFont == NULL) {
		p_EmFont = new EmFont();
	}
	return p_EmFont;
}

void EmFont::loadFont(char * filename) {
	m_Texture = TextureUtil::getInstance()->loadTexture(filename);
	if (m_Texture == NULL) {
		throw string("File not found ");
	}
// 	m_Font = TextureUtil::getInstance()->loadImage(filename);
// 	if (m_Font == NULL) {
// 		cerr << "Font: " << filename << " not found" << endl;
// 		throw string("File not found");
// 	}
//	m_Size = m_Font->width;
	m_Size = 32;
}

/* -1,1 is top left 1,-1 is bottom right */
void EmFont::print(char * buffer, float x, float y) {
	if (buffer == NULL) return;

// 	glBegin(GL_POLYGON);
// 	glColor3f(1.0f, 0.9f, 0.8f);
// 	glVertex3f(-0.1f, 0.1f, -0.9f);
// 	glColor3f(1.0f, 0.9f, 0.8f);
// 	glVertex3f(0.1f, 0.1f, -0.9f);
// 	glColor3f(1.0f, 0.9f, 0.8f);
// 	glVertex3f(-0.1f, -0.1f, -0.9f);
// 	glEnd();

	//return;
	//	glDisable(GL_DEPTH_TEST);
	if (m_Texture != NULL) {
		
		int filter = Config::getInstance()->getGLFilter();
		if (filter == -1) filter = GL_NEAREST;

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
			float u = (float)(c & 0x7)/8;
			float v = (float)(c >> 3)/8;
			//cerr << b <<" "<< c <<" "<< (char)b <<" "<< u <<" "<< v << endl;

			glBegin(GL_QUADS);
			glTexCoord2f(u, v);
			glVertex3f(x + a*EM_FONTSIZE_X , y, -1); 
			glTexCoord2f(u+0.125, v);
			glVertex3f(x + (a+1)*EM_FONTSIZE_X, y, -1); 
			glTexCoord2f(u+0.125, v+0.125);
			glVertex3f(x + (a+1)*EM_FONTSIZE_X, y-EM_FONTSIZE_Y, -1);
			glTexCoord2f(u, v+0.125);
			glVertex3f(x + a*EM_FONTSIZE_X, y-EM_FONTSIZE_Y, -1);
			glEnd();

			//			cerr << "sork" << buffer[a] << endl;
		}
	} else {
		cerr << "No font defined" << endl;
	}
	// 	glEnable(GL_DEPTH_TEST);
}
