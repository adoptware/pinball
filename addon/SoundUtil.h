/***************************************************************************
                          SoundUtil.h  -  description
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 by Henrik Enqvist, GPL
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef SOUNDUTIL_H
#define SOUNDUTIL_H

#include "Private.h"
#include <string>
#include <map>

/** Singleton class for loading and playing samples */
class SoundUtil {
 protected:
	SoundUtil();
 public:
	~SoundUtil();
	static SoundUtil* getInstance();
	int loadSample(char* fileName);
	void play(int, bool loop);
	vector<EmSample*> m_vEmSample;
 private:
 	map<string, int> m_hEmSample;
	static SoundUtil* p_SoundUtil;
};

#endif
