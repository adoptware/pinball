/***************************************************************************
                          Sound.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include <stddef.h>

#include "Sound.h"
#include "Group.h"


Sound::Sound(EmSample* sample, float dist, bool b3D = true) {
	m_b3D = b3D;
	m_fDistance = dist;
	m_Sample = sample;
	m_vtxSrc.x = 0;
	m_vtxSrc.y = 0;
	m_vtxSrc.z = 0;
	p_Parent = NULL;
}

void Sound::setParent(Group* g) {
	p_Parent = g;
}


void Sound::play(bool loop = true) {
	m_bLoop = loop;
	if (m_Sample == NULL) return;
}

void Sound::adjust() {
	EM_COUT("Sound::adjust()" << endl, 1);
//	int vol, pan;
//	float r2;

	if (!m_b3D) return;
	if (m_Sample == NULL) return;
	
	// Volume.
/*	r2 = (vtxAlPos.x*vtxAlPos.x + vtxAlPos.y*vtxAlPos.y + vtxAlPos.z*vtxAlPos.z)/100;
	r2 /= fDistance;
	r2 = EM_MAX(1, r2);
	vol = (int)((float)255/r2);
	vol = (int)EM_MIN(vol, 255);

	// Pan
	if ( EM_ZERO(vtxAlPos.z)) {
		if (vtxAlPos.x > 0) pan = 255;
		else pan = 0;
	}	else {
		pan = (int) EMath::emAtan (vtxAlPos.x/ABS(vtxAlPos.z))*2;
		pan += 127;
		pan = EM_MAX(0, pan);
	}*/
}
