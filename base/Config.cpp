/***************************************************************************
                          Engine.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

//#include <time.h>
//#include <stdlib.h>
//#include <math.h>
//#include <string.h>

#include "Config.h"

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
	m_bFullScreen = false;
	m_bExternGL = false;
}

void Config::saveConfig(const char * filename) {
	this->setDefault();

	ofstream file(filename);
	if (!file) {
		cerr << "Couldn't open config file: " << filename << endl;
		cerr << "Can't save config" <<  endl;
		return;
	}
	file << "size: " << m_iWidth <<" "<< m_iHeight << endl;
	file << "sound: " << (m_bSound ? "1" : "0") << endl;
	file << "bpp: " << m_iBpp << endl;
	file << "fullscreen: " << (m_bFullScreen ? "1" : "0") << endl;
}

void Config::loadConfig(const char * filename) {
	this->setDefault();

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
		} else if (str == "bpp:") {
			file >> m_iBpp;
		} else if (str == "fullscreen:") {
			file >> str;
			if (str == "0") m_bFullScreen = false;
			else m_bFullScreen = true;
		}
	}
}


void Config::loadArgs(int & argc, char *argv[]) {
	// Parse and remove arguments
#define REMOVEARG(a, argc, argv) if (a < argc-1) argv[a] = argv[argc-1]; argc--;
	int a = 1;
	while (a < argc) { 
		//  for (int a=1; a<argc; a++) {
		if (strcmp(argv[a], "-fullscreen") == 0) {
			m_bFullScreen = true;
			EM_COUT("Using fullscreen", 1);
			REMOVEARG(a, argc, argv);
		} else if (strcmp(argv[a], "-size") == 0) {
			if (argc > a+1) {
				m_iWidth = atoi(argv[a+1]);
				m_iHeight = atoi(argv[a+2]);
			}
			EM_COUT("Using size = " << m_iWidth <<","<< m_iHeight, 1);
			REMOVEARG(a, argc, argv);
 		} else if (strcmp(argv[a], "-bpp") == 0) {
	 		if (argc > a) {
	    	m_iBpp = atoi(argv[a+1]);
	 		}
			EM_COUT("Using " << m_iBpp << " bpp", 1);
			REMOVEARG(a, argc, argv);
   	} else if (strcmp(argv[a], "-nosound") == 0) {
			m_bSound = false;
			EM_COUT("Disabling sound", 1);
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

