/***************************************************************************
                          SoundUtil.h  -  description
                             -------------------
    begin                : Thu Jan 11 2001
    copyright            : (C) 2001 by Henrik Enqvist, GPL
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef SOUNDUTIL_H
#define SOUNDUTIL_H

#include <string>
#include <map>
#include <vector>

#ifndef PRIVATE_H
#error Must include Private.h before SoundUtil.h
#endif

#if EM_USE_SDL
#include <SDL.h>
#include <SDL_mixer.h>

#define EmSample Mix_Chunk
#define EmMusic Mix_Music

#define EM_AUDIO_FREQ 22050
#define EM_AUDIO_FORMAT AUDIO_S16
#define EM_AUDIO_CHANNELS 2
#endif // EM_USE_SDL

#if EM_USE_ALLEGRO
#include <allegro.h>
#define EmSample SAMPLE
#define EmMusic MIDI
#endif

/** Singleton class for loading and playing samples */
class SoundUtil {
 protected:
  SoundUtil();
 public:
  ~SoundUtil();
  static SoundUtil* getInstance();
  /** Initializes the sound system. Used by Engine constructor. 
   ** You don't need this use the Config class to turn on/off sound.
   ** @see Config */
  int initSound();
  /** Shuts down sound system. Used by Engine::stopEngine. 
   ** You don't need this use the Config class to turn on/off sound.
   ** @see Config */
  void stopSound();
  void applyConfigVolume();
  // TODO
  void freeSounds();
  /** Loads a wave file. Returns the identifier of the sound, used for playback.
   ** Returns -1 if it fails to load the wave file. */
  int loadSample(const char * filename);
  /** Loads a midi file. Returns the identifier of the sound, used for playback.
   ** Returns -1 if it fails to load the midi file. */
  int loadMusic(const char * filename);
  void playSample(int sound, bool loop);
  void playMusic(int sound, bool loop);
  void stopMusic();
  void pauseMusic();
  void resumeMusic();
  const char * getSoundName(int sound);
  const char * getMusicName(int music);
 private:
  bool m_bInited;
  int m_iLoopingMusic;
  vector<EmSample*> m_vEmSample;
  vector<EmMusic*> m_vEmMusic;
  map<string, int> m_hEmSample;
  map<string, int> m_hEmMusic;
  map<int, string> m_hSoundName;
  map<int, string> m_hMusicName;
  static SoundUtil* p_SoundUtil;
};

#endif
