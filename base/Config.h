/***************************************************************************
                          Engine.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <iostream>
#include <string>

#include "Private.h"

/**
 */
class Config {
 protected:
	Config();
 public:
	~Config();
	static Config * getInstance();
	void loadArgs(int & argc, char* argv[]);
	bool useSound() { return m_bSound; };
	bool useExternGL() { return m_bExternGL; };
	int getWidth() { return m_iWidth; };
	int getHeight() { return m_iHeight; };
	int getBpp() { return m_iBpp; };
	int getGLFilter() { return m_iGLFilter; };
	char* getDataDir() { return m_sDataDir; };
	bool useFullScreen() { return m_bFullScreen; };
	void setSize(int w, int h);
	void setBpp(int bpp) { m_iBpp = bpp; };
	void setGLFilter(int filter) { m_iGLFilter = filter; };
	void setFullScreen(bool f) { m_bFullScreen = f; };
	void setSound(bool s) { m_bSound = s; };
	void setDataDir(char* ch) { m_sDataDir = ch; };
	void saveConfig();
	void loadConfig();
	void setDefault();
 private:
	int m_iWidth;
	int m_iHeight;
	int m_iBpp;
	int m_iGLFilter;
	bool m_bExternGL;
	bool m_bSound;
	bool m_bFullScreen;
	char* m_sDataDir;
	static Config* p_Instance;
};

#endif // CONFIG_H
