/***************************************************************************
                          SoundUtil.h  -  description
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef SOUNDUTIL_H
#define SOUNDUTIL_H

#include "Private.h"

/**
  *@author Henrik Enqvist
  */

class SoundUtil {
	public:
		/** */
		static EmSound loadSample(char* fileName);
		static void play(EmSound, bool loop);
 private:
		// EmSample* p_Sample;
};

#endif
