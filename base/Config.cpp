/***************************************************************************
                          Config.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Config.h"
#include "EMath.h"

#include <sys/stat.h>
#include <sys/types.h>

int em_width_ = 640;
int em_height_ = 480;
int em_width_div2_ = 320;
int em_height_div2_ = 240;

Config * Config::p_Instance = NULL;

Config::Config() {
}

Config::~Config() {
}

Config * Config::getInstance() {
	if (p_Instance == NULL) {
		p_Instance = new Config();
		p_Instance->setDefault();
	}

	return p_Instance;
}

void Config::setDefault() {
	// Default values
	m_iWidth = 640;
	m_iHeight = 480;
	m_bSound = true;
	m_iBpp = 16;
	m_iGLFilter = GL_LINEAR;
	m_iView = 0;
	m_bFullScreen = false;
	m_bExternGL = false;
	m_sDataDir = EM_DATADIR;
}

void Config::saveConfig() {
#if HAVE_SYS_TYPES_H
#if HAVE_SYS_STAT_H
	char filename[256];
	char* home = getenv("HOME");
	if (home != NULL) {
		// TODO unsafe
		sprintf(filename, "%s/.emilia", home);
		mkdir(filename, S_IRUSR | S_IWUSR |S_IXUSR);
		sprintf(filename, "%s/.emilia/%s", home, PACKAGE);
	} else {
		cerr << "Could not find environment variable HOME." << endl;
		cerr << "Not able to read or write config file" << endl;
		return;
	}


	ofstream file(filename);
	if (!file) {
		cerr << "Couldn't open config file: " << filename << endl;
		cerr << "Can't save config" <<  endl;
		return;
	}
	file << "size: " << m_iWidth <<" "<< m_iHeight << endl;
	file << "sound: " << (m_bSound ? "1" : "0") << endl;
	file << "view: " << m_iView << endl;
	file << "bpp: " << m_iBpp << endl;
	file << "fullscreen: " << (m_bFullScreen ? "1" : "0") << endl;
	file << "texture_nearest: " << ((m_iGLFilter == GL_NEAREST) ? "1" : "0") << endl;

#else
	cerr << "Unable save config file because some header files were missing";
	return;
#endif
#else
	cerr << "Unable save config file because some header files were missing";
	return;
#endif
}

void Config::loadConfig() {
	this->setDefault();
#if HAVE_SYS_TYPES_H
#if HAVE_SYS_STAT_H

	char filename[256];
	char* home = getenv("HOME");
	if (home != NULL) {
		// TODO unsafe
		sprintf(filename, "%s/.emilia", home);
		mkdir(filename, S_IRUSR | S_IWUSR |S_IXUSR);
		sprintf(filename, "%s/.emilia/%s", home, PACKAGE);
	} else {
		cerr << "Could not find environment variable HOME." << endl;
		cerr << "Not able to read or write config file" << endl;
		return;
	}

	ifstream file(filename);
	if (!file) {
		cerr << "Couldn't open config file: " << filename << endl;
		cerr << "Using default values" <<  endl;
		return;
	}

	while (file) {
		string str;
		file >> str;
		if (str == "size:") {
			file >> m_iWidth;
			file >> m_iHeight;
		} else if (str == "sound:") {
			file >> str;
			if (str == "0") m_bSound = false;
			else m_bSound = true;
		} else if (str == "view:") {
			file >> m_iView;
		} else if (str == "bpp:") {
			file >> m_iBpp;
		} else if (str == "fullscreen:") {
			file >> str;
			if (str == "0") m_bFullScreen = false;
			else m_bFullScreen = true;
		} else if (str == "texture_nearest:") {
			file >> str;
			if (str == "0") {
				m_iGLFilter = GL_LINEAR;
			} else {
				m_iGLFilter = GL_NEAREST;
			}
		}
	}
#else
	cerr << "Unable load config file because some header files were missing";
	return;
#endif
#else
	cerr << "Unable load config file because some header files were missing";
	return;
#endif
}

void Config::setSize(int w, int h) { 
	m_iWidth = EM_MIN(1600, EM_MAX(100,w)); 
	m_iHeight = EM_MIN(1600, EM_MAX(100,h)); 
}

void Config::loadArgs(int & argc, char *argv[]) {
	// Parse and remove arguments, arguments are removed so the main program does not see them
	// E.g. if a program wants to load a file the file name will always be arg 1
	// regardless of 'emilia' arguments.
#define REMOVEARG(a, argc, argv) if (a < argc-1) argv[a] = argv[argc-1]; argc--;
	int a = 1;
	while (a < argc) { 
		//  for (int a=1; a<argc; a++) {
		if (strcmp(argv[a], "-fullscreen") == 0) {
			m_bFullScreen = true;
			EM_COUT("Using fullscreen", 1);
			REMOVEARG(a, argc, argv);
		} else if (strcmp(argv[a], "-size") == 0) {
			if (argc > a+2) {
				int w = atoi(argv[a+1]);
				int h = atoi(argv[a+2]);
				this->setSize(w, h);
				EM_COUT("Using size = " << m_iWidth <<","<< m_iHeight, 1);
				REMOVEARG(a, argc, argv);
				REMOVEARG(a, argc, argv);
				REMOVEARG(a, argc, argv);
			} else {
				REMOVEARG(a, argc, argv);
			}
 		} else if (strcmp(argv[a], "-bpp") == 0) {
	 		if (argc > a+1) {
	    	m_iBpp = atoi(argv[a+1]);
				REMOVEARG(a, argc, argv);
	 		}
			EM_COUT("Using " << m_iBpp << " bpp", 1);
			REMOVEARG(a, argc, argv);
 		} else if (strcmp(argv[a], "-data") == 0) {
	 		if (argc > a) {
	    	this->setDataDir(argv[a+1]);
				REMOVEARG(a, argc, argv);
	 		}
			EM_COUT("Using " << m_iBpp << " bpp", 1);
			REMOVEARG(a, argc, argv);
   	} else if (strcmp(argv[a], "-nosound") == 0) {
			this->setSound(false);
			EM_COUT("Disabling sound", 1);
			REMOVEARG(a, argc, argv);
   	} else if (strcmp(argv[a], "-nearest") == 0) {
			this->setGLFilter(GL_NEAREST);
			EM_COUT("Using nearest for texture mapping", 1);
			REMOVEARG(a, argc, argv);
   	} else if (strcmp(argv[a], "-externgl") == 0) {
			m_bExternGL = true;
			EM_COUT("Using extern GL, disabling SDL", 1);
			REMOVEARG(a, argc, argv);
		} else {
			a++;
		}
	}

  em_width_ = m_iWidth;
  em_height_ = m_iHeight;
  em_width_div2_ = m_iWidth/2;
  em_height_div2_ = m_iHeight/2;
#undef REMOVEARG
}

