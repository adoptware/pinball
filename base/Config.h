/***************************************************************************
                          Config.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <map>

#include "EMath.h"
#include "Keyboard.h"

#ifndef PRIVATE_H
#error Must include Private.h before Config.h
#endif

#if EM_USE_SDL
#include <GL/gl.h>
#define EM_LINEAR GL_LINEAR
#define EM_NEAREST GL_NEAREST
#else
#define EM_LINEAR 0
#define EM_NEAREST 1
#endif

/** */
class Config {
 protected:
  Config();
 public:
  ~Config();
  static Config * getInstance();
  void loadArgs(int & argc, char* argv[]);
  inline const int getWidth() { return m_iWidth; };
  inline const int getHeight() { return m_iHeight; };
  inline const int getWidthDiv2() { return m_iWidthDiv2; };
  inline const int getHeightDiv2() { return m_iHeightDiv2; };
  inline const int getBpp() { return m_iBpp; };
  inline const int getGLFilter() { return m_iGLFilter; };
  inline const int getView() { return m_iView; };
  inline const float getBrightness() { return m_fBrightness; };
  inline const bool useFullScreen() { return m_bFullScreen; };
  inline const int getSound() { return m_iSound; };
  inline const int getMusic() { return m_iMusic; };
  inline const bool getFire() { return m_bFire; };
  inline const bool useExternGL() { return m_bExternGL; };
  inline const bool useLights() { return m_bLights; };
  void setSize(int const w, int const h);
  inline void setBpp(int const bpp) { m_iBpp = bpp; };
  inline void setGLFilter(int const filter) { m_iGLFilter = filter; };
  inline void setView(int const view) { m_iView = view; };
  inline void setShowFPS(bool const fps) { m_bShowFPS = fps; };
  inline bool getShowFPS() { return m_bShowFPS; };
  /** This will NOT set the engine to full screen. You have to do
   * that manually. */
  inline void setFullScreen(bool const f) { m_bFullScreen = f; };
  inline void setSound(int const s) { m_iSound = EM_MIN( EM_MAX(s, 0), 8); };
  inline void setMusic(int const m) { m_iMusic = EM_MIN( EM_MAX(m, 0), 8); };
  inline void setFire(bool const f) { m_bFire = f; };
  inline void setLights(bool const l) { m_bLights = l; };
  /** This will NOT set the brightness in the engine. You
   * have to do that manually. */
  inline void setBrightness(float const b) { m_fBrightness = b; };
  /** Gets the key code from a key identifier. (e.g. "leftnudge" -> 306)*/
  EMKey getKey(string const & str);
  /** Gets the common name from a key code. (e.g. 306 -> "left ctrl") */
  char const *  const getKeyCommonName(EMKey);
  void setKey(string const & str, EMKey key);
  void setDataDir(const char * ch);
  void setSubDir(const char * ch);
  inline const char * getDataDir() { return m_sDataDir.c_str(); };
  inline const char * getDataSubDir() { return m_sDataSubDir.c_str(); };
  void saveConfig();
  void loadConfig();
  void setDefault();
  /// set RELATIVE path according to current work directory of exec
  void setPaths(char const * const argv0);  //!+rzr

  // Read high scores from given file - pnf
  bool readHighScoresFile();
  // Write high scores to a given file - pnf
  bool writeHighScoresFile();
  // Used to get the high scores from memory - pnf
  void getHighScores(multimap<int, string>& mapHighScores);
  // Used to set the high scores from memory - pnf
  void setHighScores(multimap<int, string>& mapHighScores);
  // Copy data betwen two high score maps - pnf
  void copyMaps(multimap<int, string>& mapOrig,
                multimap<int, string>& mapDest);

 private:
  int m_iWidth;
  int m_iHeight;
  int m_iWidthDiv2;
  int m_iHeightDiv2;
  int m_iBpp;
  int m_iGLFilter;
  int m_iView;
  bool m_bFire;
  bool m_bLights;
  bool m_bExternGL;
  int m_iSound;
  int m_iMusic;
  bool m_bShowFPS;
  bool m_bFullScreen;
  string m_sDataDir;
  string m_sSubDir;
  string m_sDataSubDir;
  float m_fBrightness;
  static Config* p_Instance;
  /// prototypes of Static Modules
  map<string, EMKey> m_hKey;

  // Keep the high scores for the current table
  multimap<int, string> m_mapHighScores;
};

#endif // CONFIG_H
