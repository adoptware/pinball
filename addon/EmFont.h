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

/**
 * <p> A singleton utility for fonts.</p>
 */

class EmFont {
	public:
		~EmFont();
		/** <p>Get singleton object.</p> */
		static EmFont * getInstance();
		void loadFont(char * fileName);
		/** <p>As this is GL, 0,0 is lower left corner.</p> */
		void print(char * buffer, int x, int y);
		int getSize() { return m_Size; };
	protected:
		EmFont();
	private:
		EmImage * m_Font;
		int m_Size;
		int m_Offset;
		static EmFont * p_Instance;
};

#endif // EMFONT_H
