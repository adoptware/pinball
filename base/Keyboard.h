/***************************************************************************
                          Keyboard.h  -  description
                             -------------------
    begin                : Thu Feb 1 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Private.h"

#define KEY_MAX               512

/** @author Henrik Enqvist */

class Keyboard {
	public:
		Keyboard();
		~Keyboard();
		static void clear();
		static void poll();
		static bool isKeyDown(int piKey);
		static SDLKey waitForKey();
};

#endif // KEYBOARD_H
