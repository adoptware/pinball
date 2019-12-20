/***************************************************************************
                          Profiler.cpp  -  description
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <iostream>
#include "Private.h"
#include "Profiler.h"
#include <cstdlib>

#if EM_USE_SDL
#include <SDL.h>
#endif

#if EM_USE_ALLEGRO
extern volatile unsigned int g_iMSeconds;
#endif

Profiler* Profiler::p_Profiler = NULL;

Profiler::Profiler() {
	for (int a=0; a<256; a++) {
		m_aProfile[a] = 0;
	}
	m_iCurrentProfile = 0;
#if EM_USE_SDL
	m_iCurrentTime = m_iStart = SDL_GetTicks();
#endif
#if EM_USE_ALLEGRO
	m_iCurrentTime = m_iStart = g_iMSeconds;
#endif
}

Profiler::~Profiler() {
	p_Profiler = NULL;
}

Profiler* Profiler::getInstance() {
	if (p_Profiler == NULL) {
		p_Profiler = new Profiler();
	}
	return p_Profiler;
}

void Profiler::startProfile(int i) {
	if (i<0 || i>255) i = 0;
	m_iCurrentProfile = i;
#if EM_USE_SDL
	m_iCurrentTime = SDL_GetTicks();
#endif
#if EM_USE_ALLEGRO
	m_iCurrentTime = g_iMSeconds;
#endif
}

void Profiler::stopProfile() {
#if EM_USE_SDL
	m_aProfile[m_iCurrentProfile] += (SDL_GetTicks() - m_iCurrentTime);
#endif
#if EM_USE_ALLEGRO
	m_aProfile[m_iCurrentProfile] += (g_iMSeconds - m_iCurrentTime);
#endif
}

void Profiler::printProfile() {
#if EM_USE_SDL
	unsigned int all = SDL_GetTicks() - m_iStart;
#endif
#if EM_USE_ALLEGRO
	unsigned int all = g_iMSeconds - m_iStart;
#endif
	unsigned int total = 0;
	for (int a=0; a<256; a++) {
		total += m_aProfile[a];
	}

	cerr << "Simple profile output *****************" << endl;
	cerr << "Function null      " << (float)m_aProfile[0]*100.0f/total << endl;
	cerr << "Function align     " << (float)m_aProfile[ALIGN]*100.0f/total << endl;
	cerr << "Function beh       " << (float)m_aProfile[BEH]*100.0f/total << endl;
	cerr << "Function collision " << (float)m_aProfile[COLLISION]*100.0f/total << endl;
	cerr << "Function draw      " << (float)m_aProfile[DRAW]*100.0f/total << endl;
	cerr << "Function glight    " << (float)m_aProfile[GLIGHT]*100.0f/total << endl;
	cerr << "Function plight    " << (float)m_aProfile[PLIGHT]*100.0f/total << endl;
	cerr << "Function pnormal   " << (float)m_aProfile[PNORMAL]*100.0f/total << endl;
	cerr << "Function render    " << (float)m_aProfile[RENDER]*100.0f/total << endl;
	cerr << "Function sound     " << (float)m_aProfile[SOUND]*100.0f/total << endl;
	cerr << "Function signal    " << (float)m_aProfile[SIG]*100.0f/total << endl;
	cerr << "Function trans     " << (float)m_aProfile[TRANS]*100.0f/total << endl;
	cerr << "Function swap      " << (float)m_aProfile[SWAP]*100.0f/total << endl;
	cerr << "Function keyboard  " << (float)m_aProfile[KEY]*100.0f/total << endl;
	cerr << "Function render    " << (float)m_aProfile[RENDER_OUT]*100.0f/total << endl;
	cerr << "Function swap out  " << (float)m_aProfile[SWAP_OUT]*100.0f/total << endl;
	cerr << "Function tick out  " << (float)m_aProfile[TICK_OUT]*100.0f/total << endl;
	cerr << "Function waiting   " << (float)m_aProfile[WAIT]*100.0f/total << endl;
	cerr << "Unprofiled time    " << (float)(all-total)*100.0f/all << " %" << endl;
	cerr << all <<" "<< total << endl;
}
