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
#include <string.h>

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
	int getWidth() { return m_iWidth; };
	int getHeight() { return m_iHeight; };
	int getBpp() { return m_iBpp; };
	int getGLFilter() { return m_iGLFilter; };
	int getView() { return m_iView; };
	float getBrightness() { return m_fBrightness; };
	bool useFullScreen() { return m_bFullScreen; };
	bool useSound() { return m_bSound; };
	bool useExternGL() { return m_bExternGL; };
	bool useLights() { return m_bLights; };
	void setSize(int w, int h);
	void setBpp(int bpp) { m_iBpp = bpp; };
	void setGLFilter(int filter) { m_iGLFilter = filter; };
	void setView(int view) { m_iView = view; };
	/** This will NOT set the engine to full screen. You have to do
	 * that manually. */
	void setFullScreen(bool f) { m_bFullScreen = f; };
	void setSound(bool s) { m_bSound = s; };
	void setLights(bool l) { m_bLights = l; };
	/** This will NOT set the brightness in the engine. You
	 * have to do that manually. */
	void setBrightness(float b) { m_fBrightness = b; };
	// TODO: is this a memory leak
	void setDataDir(const char * ch);
	void setSubDir(const char * ch);
	const char * getDataDir() { return m_sDataDir.c_str(); };
	const char * getDataSubDir() { return m_sDataSubDir.c_str(); };
	void saveConfig();
	void loadConfig();
	void setDefault();
 private:
	int m_iWidth;
	int m_iHeight;
	int m_iBpp;
	int m_iGLFilter;
	int m_iView;
	bool m_bLights;
	bool m_bExternGL;
	bool m_bSound;
	bool m_bFullScreen;
	string m_sDataDir;
	string m_sSubDir;
	string m_sDataSubDir;
	float m_fBrightness;
	static Config* p_Instance;
};

#endif // CONFIG_H
