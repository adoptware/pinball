/***************************************************************************
                          SoundUtil.cpp  -  description
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "SoundUtil.h"
#include "EMath.h"
#include "Config.h"

SoundUtil* SoundUtil::p_SoundUtil = NULL;

SoundUtil::SoundUtil() {
	m_bInited = false;
}

SoundUtil::~SoundUtil() {
}

int SoundUtil::initSound() {
#if EM_USE_SDL
	int audio_rate = EM_AUDIO_FREQ;
	Uint16 audio_format = EM_AUDIO_FORMAT;
	int audio_channels = EM_AUDIO_CHANNELS;

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
		cerr << "Couldn't init SDL aduio: " << SDL_GetError() << endl;
		return -1;
	}

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, 1024) < 0) {
		cerr << "Couldn't open audio mixer: " << SDL_GetError() << endl;
		return -1;
	}
	
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	cerr << "Opened audio at " << audio_rate << " Hz " 
			 << (audio_format & 0xFF) << " bit "
			 << (audio_channels > 1 ? "stereo" : "mono") << endl << endl;

	m_bInited = true;
	return 0;
#endif

#if EM_USE_ALLEGRO
  if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0) {
		cerr << "Couldn't open audio: " << allegro_error << endl;
		return -1;
	}
	cerr << "Opened audio" << endl << endl;
	m_bInited = true;
	return 0;
#endif
}

void SoundUtil::stopSound() {
	cerr << "Stopping sound...";
	if (!m_bInited) return;
#if EM_USE_SDL
	Mix_CloseAudio();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
#endif
	cerr << "ok." << endl;
}

void SoundUtil::applyConfigVolume() {
	if (Config::getInstance()->getSound() == 0 && Config::getInstance()->getMusic() == 0) {
		if (m_bInited) this->stopSound();
	} else {
		if (!m_bInited) this->initSound();
#if EM_USE_SDL
		Mix_Volume(-1, Config::getInstance()->getSound()*8);
		Mix_VolumeMusic(Config::getInstance()->getMusic()*8);
#endif
	}
}

SoundUtil* SoundUtil::getInstance() {
	if (p_SoundUtil == NULL) {
		p_SoundUtil = new SoundUtil();
	}
	return p_SoundUtil;
}

int SoundUtil::loadSample(const char* filename) {
	//	if (!m_bInited) {
	//		EM_COUT("SoundUtil::loadSample sound not initialized, not loading sample.", 1);
	//		return -1;
	//	}
	// look if the sound is already loaded
	if (m_hEmSample.find(string(filename)) != m_hEmSample.end()) {
		EM_COUT("SoundUtil::loadSample Found sample " << filename << " in cache", 1);
		map<string, int>::iterator element = m_hEmSample.find(string(filename));
		return (*element).second;
	}

#if EM_USE_SDL
	Mix_Chunk * wave = Mix_LoadWAV(filename);
#endif
#if EM_USE_ALLEGRO
	SAMPLE* wave = load_sample(filename);
#endif

	if (wave == NULL) {
		cerr << "Unable to load " << filename << endl;
		return -1;
	}

	m_vEmSample.push_back(wave);
	int sound = m_vEmSample.size()-1;
	m_hEmSample.insert(pair<string, int>(string(filename), sound));
	m_hSoundName.insert(pair<int, string>(sound, string(filename)));

	return sound;
}

const char * SoundUtil::getSoundName(int sound) {
	map<int, string>::iterator element = m_hSoundName.find(sound);
	if (element != m_hSoundName.end()) {
		return (*element).second.c_str();
	}
	cerr << "SoundUtil::getSoundName could not find sound name" << endl;
	return NULL;
}

const char * SoundUtil::getMusicName(int music) {
	map<int, string>::iterator element = m_hMusicName.find(music);
	if (element != m_hMusicName.end()) {
		return (*element).second.c_str();
	}
	cerr << "SoundUtil::getMusicName could not find music name" << endl;
	return NULL;
}

int SoundUtil::loadMusic(const char * filename) {
	if (!m_bInited) return -1;
	// look if the sound is already loaded
	if (m_hEmMusic.find(string(filename)) != m_hEmMusic.end()) {
		EM_COUT("Found music " << filename << " in cache", 1);
		map<string, int>::iterator element = m_hEmMusic.find(string(filename));
		return (*element).second;
	}

#if EM_USE_SDL
	Mix_Music * music = Mix_LoadMUS(filename);
#endif
#if EM_USE_ALLEGRO
	MIDI * music = load_midi(filename);
#endif

	if (music == NULL) {
		cerr << "Unable to load " << filename << endl;
		return -1;
	}

	m_vEmMusic.push_back(music);
	int sound = m_vEmMusic.size()-1;
	m_hEmMusic.insert(pair<string, int>(string(filename), sound));
	m_hMusicName.insert(pair<int, string>(sound, string(filename)));

	return sound;
}

void SoundUtil::playSample(int sound, bool loop) {
	if (!m_bInited) return;
	if (Config::getInstance()->getSound() == 0) return;
	if (sound < 0 || sound >= (signed)m_vEmSample.size()) return;
	cerr << "Play sample " << sound << endl;
#if EM_USE_SDL
	Mix_PlayChannel(-1, m_vEmSample[sound], (loop ? -1 : 0));
#endif // EM_USE_SDL
#if EM_USE_ALLEGRO
	play_sample(m_vEmSample[sound], 255, 127, 1000, (int)loop);
#endif
}

void SoundUtil::playMusic(int music, bool loop) {
	if (!m_bInited) return;
	if (Config::getInstance()->getMusic() == 0) return;
	if (music < 0 || music >= (signed)m_vEmMusic.size()) return;
	cerr << "Play music " << music << endl;
#if EM_USE_SDL
	Mix_HaltMusic();
	Mix_PlayMusic(m_vEmMusic[music], (loop ? -1 : 0));
#endif
#if EM_USE_ALLEGRO
	play_midi(m_vEmMusic[music], (int)loop);
#endif
}

///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////

#if 0
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
#endif // EM_USE_SDL

#if 0
// playSample
	if (sound < 0 || sound >= (signed)m_vEmSample.size()) return;
	if (!(Config::getInstance()->useSound())) return;

	SDL_LockAudio();
	m_vEmSample[sound]->pos = 0;
	m_vEmSample[sound]->loop = loop;
	SDL_UnlockAudio();

	SDL_PauseAudio(0);
#endif // EM_USE_SDL

#if 0
// loadSample
	SDL_PauseAudio(1);
	// look if the sound is already loaded
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

	// build conversion
	SDL_AudioCVT wav_cvt;
	if (SDL_BuildAudioCVT(&wav_cvt, 
												sample->spec.format, sample->spec.channels, sample->spec.freq,
												m_AudioSpec.format, m_AudioSpec.channels, m_AudioSpec.freq) == -1) {
		cerr << "*********************************************************************"  << endl;
		cerr << "Couldn't build audio conversion table" << endl;
		cerr << "*********************************************************************"  << endl;
		return -1;
	}
	
	// setup for conversion
	wav_cvt.buf = (Uint8 *)malloc(sample->length *  wav_cvt.len_mult);
	wav_cvt.len = sample->length;
	memcpy(wav_cvt.buf, sample->wave, sample->length); 
	// We can delete to original WAV data now
	SDL_FreeWAV(sample->wave); 
	// And now we're ready to convert
	SDL_ConvertAudio(&wav_cvt);	

	sample->wave = wav_cvt.buf;
	sample->length = (int)((double)sample->length * wav_cvt.len_ratio);
	sample->spec.callback = fctFillAudio;
	sample->pos = sample->length; // set pos to end to prevent playback
	sample->loop = false;

	SDL_LockAudio();
	m_vEmSample.push_back(sample);
	int sound = m_vEmSample.size()-1;
	m_hEmSample.insert(pair<string, int>(string(filename), sound));
	SDL_UnlockAudio();
	return sound;
#endif // EM_USE_SDL

#if 0
// initSound
	SDL_AudioSpec wanted;
	extern void fctFillAudio(void *udata, Uint8 *stream, int len);
	
	/* Set the audio format */
	wanted.freq = 22050;
	wanted.format = AUDIO_S16;
	wanted.channels = 2;    /* 1 = mono, 2 = stereo */
	wanted.samples = 512;  /* Good low-latency value for callback */
	wanted.callback = fctFillAudio;
	wanted.userdata = NULL;
	
	/* Open the audio device, forcing the desired format */
	if ( SDL_OpenAudio(&wanted, &m_AudioSpec) < 0 ) {
		cerr << "*********************************************************************"  << endl;
		cerr << "Couldn't open audio: " << SDL_GetError() << endl;
		cerr << "Make sure that no other application has occupied the audio resources."  << endl;
		cerr << "If your're running KDE you might try to kill the 'artsd' process." << endl;
		cerr << "*********************************************************************"  << endl;
		return -1;
	}

	cerr << endl;
	cerr << "Freq (requested " << wanted.freq         << "hz) : " << m_AudioSpec.freq << endl;
	cerr << "Format (requested " << wanted.format     << ") : " << m_AudioSpec.format << endl;
	cerr << "Channels (requested " << wanted.channels << ") : " << m_AudioSpec.channels << endl;
	cerr << "Samples (requested  " << wanted.samples  << ") : " << m_AudioSpec.samples << endl;

	m_bInited = true;
	return 0;
#endif // EM_USE_SDL
