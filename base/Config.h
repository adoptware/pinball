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
  inline int getWidth() { return m_iWidth; };
  inline int getHeight() { return m_iHeight; };
  inline int getWidthDiv2() { return m_iWidthDiv2; };
  inline int getHeightDiv2() { return m_iHeightDiv2; };
  inline int getBpp() { return m_iBpp; };
  inline int getGLFilter() { return m_iGLFilter; };
  inline int getView() { return m_iView; };
  inline float getBrightness() { return m_fBrightness; };
  inline bool useFullScreen() { return m_bFullScreen; };
  inline int getSound() { return m_iSound; };
  inline int getMusic() { return m_iMusic; };
  inline bool getFire() { return m_bFire; };
  inline bool useExternGL() { return m_bExternGL; };
  inline bool useLights() { return m_bLights; };
  void setSize(int w, int h);
  inline void setBpp(int bpp) { m_iBpp = bpp; };
  inline void setGLFilter(int filter) { m_iGLFilter = filter; };
  inline void setView(int view) { m_iView = view; };
  inline void setShowFPS(bool fps) { m_bShowFPS = fps; };
  inline bool getShowFPS() { return m_bShowFPS; };
  /** This will NOT set the engine to full screen. You have to do
   * that manually. */
  inline void setFullScreen(bool f) { m_bFullScreen = f; };
  inline void setSound(int s) { m_iSound = EM_MIN( EM_MAX(s, 0), 8); };
  inline void setMusic(int m) { m_iMusic = EM_MIN( EM_MAX(m, 0), 8); };
  inline void setFire(bool f) { m_bFire = f; };
  inline void setLights(bool l) { m_bLights = l; };
  /** This will NOT set the brightness in the engine. You
   * have to do that manually. */
  inline void setBrightness(float b) { m_fBrightness = b; };
  /** Gets the key code from a key identifier. (e.g. "leftnudge" -> 306)*/
  EMKey getKey(string & str);
  /** Gets the common name from a key code. (e.g. 306 -> "left ctrl") */
  const char * getKeyCommonName(EMKey);
  void setKey(string & str, EMKey key);
  void setDataDir(const char * ch);
  void setSubDir(const char * ch);
  inline const char * getDataDir() { return m_sDataDir.c_str(); };
  inline const char * getDataSubDir() { return m_sDataSubDir.c_str(); };
  void saveConfig();
  void loadConfig();
  void setDefault();
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
  map<string, EMKey> m_hKey;
};

#endif // CONFIG_H
