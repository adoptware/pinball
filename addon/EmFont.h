/***************************************************************************
                          Font.h  -  description
                             -------------------
    begin                : Mon Jul 23 2001
    copyright            : (C) 2001 by Henrik Enqvist IB
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef EMFONT_H
#define EMFONT_H

#include "Private.h"

#define EM_FONTSIZE_X 0.06f
#define EM_FONTSIZE_Y 0.08f

#define EM_FONT_X_2 17
#define EM_FONT_Y_2 13

/** A singleton utility for fonts. */

class EmFont {
	public:
		~EmFont();
		/** Get singleton object. */
		static EmFont * getInstance();
		void loadFont(char * fileName);
		/** -1, 1 is upper left, 1, -1 is lower right */
		void print(char * buffer, float x, float y);
		int getSize() { return m_Size; };
	protected:
		EmFont();
	private:
		//EmImage * m_Font;
		EmTexture * m_Texture;
		int m_Size;
		static EmFont * p_EmFont;
};

#endif // EMFONT_H
