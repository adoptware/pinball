/***************************************************************************
                          Config.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "Config.h"
#include "EMath.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int em_width_ = 640;
int em_height_ = 480;
int em_width_div2_ = 320;
int em_height_div2_ = 240;

Config * Config::p_Instance = NULL;

Config::Config() {
	m_sDataDir = "";
	m_sSubDir = "";
	m_sDataSubDir = "";
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
	this->setSize(640, 480);
	this->setSound(8);
	this->setMusic(5);
	this->setBpp(16);
	this->setGLFilter(EM_LINEAR);
	this->setView(0);
	this->setFullScreen(false);
	m_bExternGL = false;
	this->setDataDir(EM_DATADIR);
	this->setLights(true);
	this->setBrightness(0.1f);
}

void Config::setDataDir(const char* ch) { 
	m_sDataDir = string(ch); 
	m_sDataSubDir = m_sDataDir + "/" + m_sSubDir;
}

void Config::setSubDir(const char* ch) { 
	m_sSubDir = string(ch); 
	m_sDataSubDir = m_sDataDir + "/" + m_sSubDir;
}

void Config::saveConfig() {
	string filename;
	char* home = getenv("HOME");
	if (home != NULL) {
		// TODO unsafe
		filename = string(home) + string("/.emilia");
		mkdir(filename.c_str(), S_IRUSR | S_IWUSR |S_IXUSR);
		filename = string(home) + string("/.emilia/") + string(PACKAGE); 
	} else {
		cerr << "Could not find environment variable HOME." << endl;
		cerr << "Not able to read or write config file" << endl;
		return;
	}

	ofstream file(filename.c_str());
	if (!file) {
		cerr << "Couldn't open config file: " << filename << endl;
		cerr << "Can't save config" <<  endl;
		return;
	}
	file << "size: " << m_iWidth <<" "<< m_iHeight << endl;
	file << "sound: " << m_iSound << endl;
	file << "music: " << m_iMusic << endl;
	file << "view: " << m_iView << endl;
	file << "bpp: " << m_iBpp << endl;
	file << "fullscreen: " << (m_bFullScreen ? "1" : "0") << endl;
	file << "lights: " << (m_bLights ? "1" : "0") << endl;
	file << "brightness: " << m_fBrightness << endl;
	if (m_iGLFilter == EM_LINEAR) {
		file << "texture_filter: " << "0" << endl;
	} else if (m_iGLFilter == EM_NEAREST) {
		file << "texture_filter: " << "1" << endl;
	} else {
		file << "texture_filter: " << "-1" << endl;
	}
}

void Config::loadConfig() {
	this->setDefault();

	string filename;
	char* home = getenv("HOME");
	if (home != NULL) {
		// TODO unsafe
		filename = string(home) + string("/.emilia");
		mkdir(filename.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
		filename = string(home) + string("/.emilia/") + string(PACKAGE);
	} else {
		cerr << "Could not find environment variable HOME." << endl;
		cerr << "Not able to read or write config file" << endl;
		return;
	}

	ifstream file(filename.c_str());
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
			int vol;
			file >> vol;
			this->setSound(vol);
		} else if (str == "music:") {
			int vol;
			file >> vol;
			this->setMusic(vol);
		} else if (str == "view:") {
			file >> m_iView;
		} else if (str == "bpp:") {
			file >> m_iBpp;
		} else if (str == "fullscreen:") {
			file >> str;
			if (str == "0") this->setFullScreen(false);
			else this->setFullScreen(true);
		} else if (str == "lights:") {
			file >> str;
			if (str == "0") this->setLights(false);
			else this->setLights(true);
		} else if (str == "texture_filter:") {
			file >> str;
			if (str == "0") {
				m_iGLFilter = EM_LINEAR;
			} else if (str == "1") {
				m_iGLFilter = EM_NEAREST;
			} else {
				m_iGLFilter = -1;
			}
		} else if (str == "brightness:") {
			float bright;
			file >> bright;
			this->setBrightness(bright);
		}
	}
}

void Config::setSize(int w, int h) { 
	m_iWidth = EM_MIN(1600, EM_MAX(100,w)); 
	m_iHeight = EM_MIN(1200, EM_MAX(100,h)); 
  em_width_ = m_iWidth;
  em_height_ = m_iHeight;
  em_width_div2_ = m_iWidth/2;
  em_height_div2_ = m_iHeight/2;
}

void Config::loadArgs(int & argc, char *argv[]) {
	// Parse and remove arguments, arguments are removed so the main program does not see them
	// E.g. if a program wants to load a file the file name will always be arg 1
	// regardless of 'emilia' arguments.
#define REMOVEARG(a, argc, argv) for (int aa=a ;aa < argc-1; aa++) argv[aa] = argv[aa+1]; argc--;

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
				EM_COUT("Using datapath: " << argv[a+1], 1);
	    	this->setDataDir(argv[a+1]);
				REMOVEARG(a, argc, argv);
	 		}
			REMOVEARG(a, argc, argv);
   	} else if (strcmp(argv[a], "-nosound") == 0) {
			this->setSound(0);
			this->setMusic(0);
			EM_COUT("Disabling sound", 1);
			REMOVEARG(a, argc, argv);
   	} else if (strcmp(argv[a], "-nolights") == 0) {
			this->setLights(false);
			EM_COUT("Disabling lights", 1);
			REMOVEARG(a, argc, argv);
   	} else if (strcmp(argv[a], "-nearest") == 0) {
			this->setGLFilter(EM_NEAREST);
			EM_COUT("Using nearest for texture mapping", 1);
			REMOVEARG(a, argc, argv);
   	} else if (strcmp(argv[a], "-externgl") == 0) {
			m_bExternGL = true;
			EM_COUT("Using extern GL, disabling SDL", 1);
			REMOVEARG(a, argc, argv);
		} else {
			EM_COUT("Unknown argument: " << argv[a], 1);
			a++;
		}
	}

  em_width_ = m_iWidth;
  em_height_ = m_iHeight;
  em_width_div2_ = m_iWidth/2;
  em_height_div2_ = m_iHeight/2;
#undef REMOVEARG
}

