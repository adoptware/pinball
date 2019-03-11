//#ident "$Id: Config.cpp,v 1.29 2003/06/13 13:39:44 rzr Exp $"
/***************************************************************************
                          Config.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000-2016 by Henrik Enqvist
    email                : henqvist@excite.com
***************************************************************************/

#include "Private.h"
#include "Config.h"
#include "EMath.h"

#include <cstdio>
#include <fstream>
#include <iostream>

#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_UNISTD_H // !+rzr: not in msvc
#include <unistd.h>
#endif

#ifdef WIN32
//#include <io.h> 
#include <direct.h>  // mkdir @ msvc+mingw32
//#define mkdir(name, modes) mkdir(name)
#endif //!-rzr

#ifndef MAX_PATH
# define MAX_PATH 255
#endif


Config * Config::p_Instance = NULL;

Config::Config() {
  m_sDataDir = "";
  m_sSubDir = "";
  m_sDataSubDir = "";
  m_sExeDir = "";
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
  this->setSize( CONFIG_LOCAL_WIDTH_DEFAULT, CONFIG_LOCAL_HEIGHT_DEFAULT );
  this->setSound(8);
  this->setMusic(8);
  this->setBpp(16);
  this->setGLFilter(EM_LINEAR);
  this->setView(0);
  this->setFullScreen(false);
  m_bExternGL = false;
#ifndef RZR_PATHRELATIVE
  this->setDataDir(EM_DATADIR);  //!rzr is it usefull, ! belongs to user cfg?
#endif
  this->setLights(true);
  this->setBrightness(0.5f);
  this->setShowFPS(false);
  this->setFire(false);

  string const leftflip("leftflip");
  string const rightflip("rightflip");
  string const bottomnudge("bottomnudge");
  string const leftnudge("leftnudge");
  string const rightnudge("rightnudge");
  string const launch("launch");
  string const reset("reset");

  this->setKey(leftflip, SDLK_LSHIFT);
  this->setKey(rightflip, SDLK_RSHIFT);
  this->setKey(bottomnudge, SDLK_SPACE);
  this->setKey(leftnudge, SDLK_LCTRL);
  this->setKey(rightnudge, SDLK_RCTRL);
  this->setKey(launch, SDLK_RETURN);
  this->setKey(reset, SDLK_r); // !rzr why not use Return
}

void Config::setDataDir(const char* ch) { 
  m_sDataDir = string(ch); 
  m_sDataSubDir = m_sDataDir + "/" + m_sSubDir;
}

void Config::setSubDir(const char* ch) { 
  m_sSubDir = string(ch); 
  m_sDataSubDir = m_sDataDir + "/" + m_sSubDir;
}

EMKey Config::getKey(string const & str) {
  if (m_hKey.find(str) != m_hKey.end()) {
    map<string, EMKey>::iterator element = m_hKey.find(str);
    return (*element).second;
  }
  // failed, just return anything
  return SDLK_HOME;
}

void Config::setKey(string const & str, EMKey key) {
  if (m_hKey.find(str) != m_hKey.end()) {
    m_hKey.erase(m_hKey.find(str));
  }
  m_hKey.insert(pair<string, EMKey>(str, key));
}

char const * const Config::getKeyCommonName(EMKey key) {
#if EM_USE_SDL
  return SDL_GetKeyName(key);
#endif // EM_USESDL
#if EM_USE_ALLEGRO
  switch(key) {
  case SDLK_a: return "a";
  case SDLK_b: return "b";
  case SDLK_c: return "c";
  case SDLK_d: return "d";
  case SDLK_e: return "e";
  case SDLK_f: return "f";
  case SDLK_g: return "g";
  case SDLK_h: return "h";
  case SDLK_i: return "i";
  case SDLK_j: return "j";
  case SDLK_k: return "k";
  case SDLK_l: return "l";
  case SDLK_m: return "m";
  case SDLK_n: return "n";
  case SDLK_o: return "o";
  case SDLK_p: return "p";
  case SDLK_q: return "q";
  case SDLK_r: return "r";
  case SDLK_s: return "s";
  case SDLK_t: return "t";
  case SDLK_u: return "u";
  case SDLK_v: return "v";
  case SDLK_w: return "w";
  case SDLK_x: return "x";
  case SDLK_y: return "y";
  case SDLK_z: return "z";
  case SDLK_0: return "0";
  case SDLK_1: return "1";
  case SDLK_2: return "2";
  case SDLK_3: return "3";
  case SDLK_4: return "4";
  case SDLK_5: return "5";
  case SDLK_6: return "6";
  case SDLK_7: return "7";
  case SDLK_8: return "8";
  case SDLK_9: return "9";
  case SDLK_RETURN: return gettext("return");
  case SDLK_SPACE: return gettext("space");
  case SDLK_LSHIFT: return gettext("left shift");
  case SDLK_RSHIFT: return gettext("right shift");
  case SDLK_LCTRL: return gettext("left ctrl");
  case SDLK_RCTRL: return gettext("right ctrl");
  }
  return gettext("unknown");
#endif
}

void Config::saveConfig() {

  string dirname = string(".config/emilia/"); 
  string filename = string(PACKAGE_NAME);

#if HAVE_SYS_STAT_H && HAVE_SYS_TYPES_H
  char const * const home = getenv("HOME");
  if (home != NULL) {
    // TODO unsafe
    dirname = string(home) + '/' + dirname;
    mkdir(dirname.c_str(), S_IRUSR | S_IWUSR |S_IXUSR);
  } else {
#ifdef RZR_PATHRELATIVE //!rzr: check w32 config save (TODO)
    dirname = m_sExeDir + '/' ;
    filename = string(PACKAGE_NAME) + ".cfg";
#else
    cerr << "Could not find environment variable HOME." << endl;
    cerr << "Not able to read or write config file" << endl;
    return;
#endif
  }
#endif

  filename = dirname  + filename;

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
  file << "showfps: " << (m_bShowFPS ? "1" : "0") << endl;
  file << "fire: " << (m_bFire ? "1" : "0") << endl;

  map<string, EMKey>::iterator iter = m_hKey.begin();
  map<string, EMKey>::iterator end = m_hKey.end();
  for (; iter != end; ++iter) {
    file << "keyboard: " << (*iter).first <<" "<< (*iter).second << endl;
  }  

  file << "ratio: " << m_fRatio<< endl;
}

void Config::loadConfig() {
  // loading default fixes possible problems with missing values in config file
  this->setDefault();

  string dirname = string(".config/emilia/"); 
  string filename = string(PACKAGE_NAME);

#if HAVE_SYS_STAT_H && HAVE_SYS_TYPES_H
  char const * const home = getenv("HOME");
  if (home != NULL) {    // TODO unsafe
    dirname = string(home) + '/' + dirname;
  } else {
#ifdef RZR_PATHRELATIVE //!rzr: check w32 config save (TODO)
    dirname = m_sExeDir + '/' ;
    filename = string(PACKAGE_NAME) + ".cfg";
#else
    cerr << "Could not find environment variable HOME." << endl;
    cerr << "Not able to read or write config file" << endl;
    return;
#endif
  }
#endif

  filename = dirname  + filename;

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
      int vol=0;
      file >> vol;
      this->setSound(vol);
    } else if (str == "music:") {
      int vol=0;
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
    } else if (str == "showfps:") {
      file >> str;
      if (str == "0") this->setShowFPS(false);
      else this->setShowFPS(true);
    } else if (str == "fire:") {
      file >> str;
      if (str == "0") this->setFire(false);
      else this->setFire(true);
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
      float bright=0;
      file >> bright;
      this->setBrightness(bright);
    } else if (str == "keyboard:") {
      string keyname;
      int key=0;
      file >> keyname;
      file >> key;
      this->setKey(keyname, (EMKey)key);
    } else if (str == "ratio:") {
      file >> m_fRatio;
    }
  }
  //EM_CERR("- Config::loadConfig");
}

void Config::setSize(int const w, int h) { 
  if ( h == 0 ) { h = w; }
  m_iWidth = EM_MIN(8000, EM_MAX(100,w)); 
  m_iHeight = EM_MIN(4000, EM_MAX(100,h)); 
  m_iWidthDiv2 = m_iWidth/2;
  m_iHeightDiv2 = m_iHeight/2;
}

void Config::loadArgs(int & argc, char *argv[]) {
  // Parse and remove arguments, arguments are removed so the main program does not see them
  // E.g. if a program wants to load a file the file name will always be arg 1
  // regardless of 'emilia' arguments.
#define REMOVEARG(a, argc, argv) { for (int aa=a ;aa < argc-1; aa++) argv[aa] = argv[aa+1]; argc--; } //!+rzr added scope for msvc


#ifdef RZR_PATHRELATIVE
  //!+rzr this workaround Full path to relative ones, usefull for windows port
  setPaths( argv[0] );
#endif //!-rzr

  int a = 1;
  while (a < argc) { 
    //  for (int a=1; a<argc; a++) {
    if (strcmp(argv[a], "-dir") == 0) {
      cout << EM_DATADIR << endl;
      exit(0);
    } else if (strcmp(argv[a], "-fullscreen") == 0) {
      m_bFullScreen = true;
      EM_COUT("Using fullscreen", 1);
      REMOVEARG(a, argc, argv);
    } else if (strcmp(argv[a], "-size") == 0) {
      if (argc > a+2) {
        int const w= atoi(argv[a+1]);
        int const h = atoi(argv[a+2]);
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

  //   em_width_ = m_iWidth;
  //   em_height_ = m_iHeight;
  //   em_width_div2_ = m_iWidth/2;
  //   em_height_div2_ = m_iHeight/2;
#undef REMOVEARG
  //EM_CERR("- Config::loadArgs");
}

///!+rzr this workaround Full path to relative ones, usefull for windows port
bool isAbsolutePath(char const * const argv0 ) ; 
bool isAbsolutePath(char const * const argv0 ) 
{
  //EM_COUT(" check root drive c:\\ // absolute path -  check for wine ?", 42);
  bool t = false;
#ifdef WIN32
  // assert (strlen (argv0) > 3 );
  if ( ( *(argv0 +1) == ':' ) && ( *(argv0 +2)  == '\\' ) )
    return  true;
#endif 
  if ( *argv0  == '/' )  // WIN32 @ unix wine/ cygwine
    t = true;
  // check for macs, amigas  etc
  //cout<<"- isAbsolutePath"<<endl;
  return t;
}
/// TODO; make it more robust for stranges paths 
/// (ie "c:\\d/i//r\like\\\\this/\\/") , wine virtual pc etc
void Config::setPaths(char const * const argv0) {
  // EM_CERR("+ Config::setPath"); 
  //!+rzr : make it work also in relative paths use
  // and "/long path/quoted/paths/" etc
  //EM_COUT( argv0 , 0);
  m_sDataDir = string(EM_DATADIR) + "/";
  m_sExeDir = "./";
  if ( *( m_sDataDir.c_str() ) != '/' ) {
    const char* ptr=0;
    char* ptrw = 0;
    //cout<<"relative to exe file"<<endl;
    ptr = (strrchr(argv0,'/')); // unix /cygwin / check win32 
#ifdef WIN32
    ptrw = (strrchr(argv0,'\\')); 
#endif //TODO: MacOS file sep ':'   
    if ( ptrw > ptr ) ptr = ptrw;
    //    assert( (*ptr != 0) );
    string path( argv0 , ptr - argv0 );
    //EM_COUT( path , 42);    
    if ( isAbsolutePath( argv0 ) ) {
      m_sExeDir = path ;
    } else {  
      //EM_COUT("relative path from cwd",42);
      char cwd[MAX_PATH];  // TODO check for buffer overflow
      char* status = getcwd(cwd,MAX_PATH); 
      m_sExeDir = string(cwd) + '/' +  path ;
    }
    m_sDataDir = m_sExeDir + '/' + string(EM_DATADIR) ;
  } else {  // cout<<"absolute path"<<endl;
    m_sDataDir =  string(EM_DATADIR) ;
  }
  m_sDataSubDir = m_sDataDir  + "/"  + m_sSubDir ;
  
#ifdef WIN32 // !+rzr Path are backlashed 
  // but works fine that way on wine and win98
  // m_sDataSubDir.replace (  m_sDataSubDir.find(/,0) , 1,   \\  );
  // m_sDataDir.replace (  m_sDataDir.find(\\,0) , 1,   /  );
#endif 
 
  // EM_CERR("- Config::setPath"); // EM_CERR( m_sExeDir); EM_CERR( m_sDataDir);
} //!-rzr

//EOF:$Id: Config.cpp,v 1.29 2003/06/13 13:39:44 rzr Exp $
