/***************************************************************************
                          SoundUtil.cpp  -  description
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#include "SoundUtil.h"
#include "EMath.h"
#include "Config.h"

void fctFillAudio(void *udata, Uint8 *stream, int len) {
	vector<EmSample*>::iterator sampleIter = SoundUtil::getInstance()->m_vEmSample.begin();
	vector<EmSample*>::iterator sampleEnd = SoundUtil::getInstance()->m_vEmSample.end();
	
	for ( ; sampleIter != sampleEnd; sampleIter++) {
		Uint32 audio_len = (*sampleIter)->length - (*sampleIter)->pos;
		// only play if we have data left
		if (audio_len < 1) {
			if ((*sampleIter)->loop == true) {
				(*sampleIter)->pos = 0;
			}
			// TODO: looped sounds pause
			continue;
		}
	
		// TODO: the volume of the first waves will be mixed down too much
		// putting wave in the buffer stream will play the wave
		Uint32 real_len = EM_MIN((unsigned)len, audio_len);
		SDL_MixAudio(stream, &((*sampleIter)->wave[(*sampleIter)->pos]), real_len, SDL_MIX_MAXVOLUME);
		(*sampleIter)->pos += real_len;
	}
}


SoundUtil* SoundUtil::p_SoundUtil = NULL;

SoundUtil::SoundUtil() {
}

SoundUtil::~SoundUtil() {
}

SoundUtil* SoundUtil::getInstance() {
	if (p_SoundUtil == NULL) {
		p_SoundUtil = new SoundUtil();
	}
	return p_SoundUtil;
}

int SoundUtil::loadSample(char* filename) {
	// look if the texture is already loaded
	if (m_hEmSample.count(string(filename)) != 0) {
		EM_COUT("Found sample " << filename << " in cache", 1);
		map<string, int>::iterator element = m_hEmSample.find(string(filename));
		return (*element).second;
	}

	EmSample* sample = (EmSample*) malloc(sizeof(EmSample));
	/* Load the wave file into memory */
	if ( SDL_LoadWAV(filename, &(sample->spec), &(sample->wave), &(sample->length)) == NULL ) {
		cerr << "Couldn't load " << filename <<" "<< SDL_GetError() << endl;
		return -1;
	}
	sample->spec.callback = fctFillAudio;
	sample->pos = sample->length; // set pos to end to prevent playback
	sample->loop = false;

	m_vEmSample.push_back(sample);
	int sound = m_vEmSample.size()-1;
	m_hEmSample.insert(pair<string, int>(string(filename), sound));
	return sound;
}

void SoundUtil::play(int sound, bool loop) {
	if (sound < 0 || sound >= (signed)m_vEmSample.size()) return;
	if (!(Config::getInstance()->useSound())) return;

	cerr << "play " << sound << endl;

	SDL_LockAudio();
	m_vEmSample[sound]->pos = 0;
	m_vEmSample[sound]->loop = loop;
	SDL_UnlockAudio();

	SDL_PauseAudio(0);
}
