/***************************************************************************
                          SoundUtil.cpp  -  description
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#include "SoundUtil.h"
#include "EMath.h"

vector<EmSample*> g_vSample;

void fctFillAudio(void *udata, Uint8 *stream, int len) {
	vector<EmSample*>::iterator sampleIter = g_vSample.begin();
	vector<EmSample*>::iterator sampleEnd = g_vSample.end();
	
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
		Uint32 real_len = EM_MIN(len, audio_len);
		SDL_MixAudio(stream, &((*sampleIter)->wave[(*sampleIter)->pos]), real_len, SDL_MIX_MAXVOLUME);
		(*sampleIter)->pos += real_len;
	}
}

EmSound SoundUtil::loadSample(char* fileName) {
	EmSample* sample = (EmSample*) malloc(sizeof(EmSample));
	/* Load the wave file into memory */
	if ( SDL_LoadWAV(fileName, &(sample->spec), &(sample->wave), &(sample->length)) == NULL ) {
		cerr << "Couldn't load " << fileName <<" "<< SDL_GetError() << endl;
		return -1;
	}
	sample->spec.callback = fctFillAudio;
	sample->pos = sample->length; // set pos to end to prevent playback
	sample->loop = false;

	g_vSample.push_back(sample);
	return g_vSample.size()-1;
}

void SoundUtil::play(EmSound sound, bool loop) {
	if (sound < 0 || sound >= g_vSample.size()) return;

	SDL_LockAudio();
	g_vSample[sound]->pos = 0;
	g_vSample[sound]->loop = loop;
	SDL_UnlockAudio();

	SDL_PauseAudio(0);
}
