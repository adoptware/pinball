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

#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <cstdio>

#if HAVE_UNISTD_H // !+rzr: not in msvc
#include <unistd.h>
#endif

#ifdef WIN32
//#include <io.h> 
#include <direct.h>  // mkdir @ msvc+mingw32
#define mkdir(name, modes) mkdir(name)
#endif //!-rzr


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
  this->setMusic(4);
  this->setBpp(16);
  this->setGLFilter(EM_LINEAR);
  this->setView(0);
  this->setFullScreen(false);
  m_bExternGL = false;
  this->setDataDir(EM_DATADIR);
  this->setLights(true);
  this->setBrightness(0.1f);
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
  this->setKey(reset, SDLK_r);
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
  return NULL;
#endif
}

void Config::saveConfig() {
  string filename;

#if HAVE_SYS_STAT_H && HAVE_SYS_TYPES_H
  char const * const home = getenv("HOME");
  if (home != NULL) {
    // TODO unsafe
    filename = string(home) + string("/.emilia");
    mkdir(filename.c_str(), S_IRUSR | S_IWUSR |S_IXUSR);
    filename = string(home) + string("/.emilia/") + string(PACKAGE_NAME); 
  } else {
    cerr << "Could not find environment variable HOME." << endl;
    cerr << "Not able to read or write config file" << endl;
    return;
  }
#endif

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
}

void Config::loadConfig() {
  // loading default fixes possible problems with missing values in config file
  this->setDefault();

  string filename;
  char const * const home = getenv("HOME");
  if (home != NULL) {
    // TODO unsafe
    filename = string(home) + string("/.emilia");
    mkdir(filename.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
    filename = string(home) + string("/.emilia/") + string(PACKAGE_NAME);
  } else {
    cerr << "Could not find environment variable HOME." << endl;
    cerr << "Not able to read or write config file" << endl;
    return;
  }

  ifstream file(filename.c_str());
  if (!file) {
    //cerr << "Couldn't open config file: " << filename << endl;
    //cerr << "Using default values" <<  endl;
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
    }
  }
}

void Config::setSize(int const w, int const h) { 
  m_iWidth = EM_MIN(1600, EM_MAX(100,w)); 
  m_iHeight = EM_MIN(1200, EM_MAX(100,h)); 
  m_iWidthDiv2 = m_iWidth/2;
  m_iHeightDiv2 = m_iHeight/2;
  //   em_width_ = m_iWidth;
  //   em_height_ = m_iHeight;
  //   em_width_div2_ = m_iWidth/2;
  //   em_height_div2_ = m_iHeight/2;
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

  //   em_width_ = m_iWidth;
  //   em_height_ = m_iHeight;
  //   em_width_div2_ = m_iWidth/2;
  //   em_height_div2_ = m_iHeight/2;
#undef REMOVEARG
}

///!+rzr this workaround Full path to relative ones, usefull for windows port
bool isAbsolutePath(char const * const argv0 ) //!+rzr
{
  //EM_COUT(" check root drive c:\\ // absolute path -  check for wine ?", 42);
#ifdef WIN32
 // assert (strlen (argv0) > 3 );
  if ( ( *(argv0 +1) == ':' ) && ( *(argv0 +2)  == '\\' ) )
    return  true;
#endif 
  if ( *argv0  == '/' )  // WIN32 @ unix wine/ cygwine
    return true;
  // check for macs, amigas  etc
  return false;
}
/// TODO; make it more robust for stranges paths 
/// (ie "c:\\d/i//r\like\\\\this/\\/") , wine virtual pc etc
void Config::setPaths(char const * const argv0) {
  EM_COUT("+ Config::getFullPath",0); 
  //!+rzr : make it work also in relative paths use
  // and "/long path/brackets" etc
  //EM_COUT( argv0 , 0);
  m_sDataDir = EM_DATADIR;
  
  if ( *( m_sDataDir.c_str() ) != '/' ) {
    char* ptr=0; 
    EM_COUT("relative to exe file",42);
#ifdef WIN32
    ptr = (strrchr(argv0,'\\')); 
#endif
    if ( ptr == 0 ) ptr = (strrchr(argv0,'/')); // unix /cygwin / check win32 
    //    assert( (*ptr != 0) );
    string path( argv0 , ptr - argv0 );
    EM_COUT( path , 42);    
    if ( isAbsolutePath( argv0 ) ) {
      m_sDataDir = path + "/" + EM_DATADIR; 
    } else {  
      EM_COUT("relative path from cwd",42);
      char cwd[256]; 
			getcwd(cwd,256); // TODO check for buffer overflow
      m_sDataDir = string(cwd) + "/" +  path + "/" + EM_DATADIR;
    }
  } else { m_sDataDir =  EM_DATADIR; }
  m_sDataSubDir = m_sDataDir + "/" + m_sSubDir;
#ifdef WIN32 // !+rzr Path are backlashed 
  // but works fine that way on wine and win98
  // m_sDataSubDir.replace (  m_sDataSubDir.find(/,0) , 1,   \\  );
  // m_sDataDir.replace (  m_sDataDir.find(\\,0) , 1,   /  );
#endif 
  EM_COUT( m_sDataDir, 42);
  EM_COUT("- Config::getFullPath",0);
} //!-rzr


//
// HighScores
// pnf
//

#define HIGH_SCORES_FILENAME      "/highscores"

bool Config::readHighScoresFile()
{
  // This is the current table's name
  if (m_sSubDir.length() == 0)
  {
    cerr << "No current table name!" << endl;
    return false;
  }

  // Clear old high scores
  m_mapHighScores.clear();

  string sFileName = m_sDataSubDir + HIGH_SCORES_FILENAME;

  ifstream file(sFileName.c_str());

  if (!file)
  {
    cerr << "Couldn't open high scores file: " << sFileName << endl;
    cerr << "Using default values!" << endl;

    for (int i=0; i<10; i++)
      m_mapHighScores.insert(pair<int, string>(10 - i, "lia"));

    return false;
  }

  int nCounter = 0;
  int nScore   = 0;

  string sName;

  while (file)
  {
    file >> nScore;
    file >> sName;

    m_mapHighScores.insert(pair<int, string>(nScore, sName));

    nCounter++;

    // We only read 10 scores from the file!
    if (nCounter > 10)
      break;
  }

  // If we read less then 10 scores
  for (int i=m_mapHighScores.size(); i<10; i++)
    m_mapHighScores.insert(pair<int, string>(10 - i, "lia"));

  return true;
}

bool Config::writeHighScoresFile()
{
  // This is the current table's name
  if (m_sSubDir.length() == 0)
  {
    cerr << "No current table name!" << endl;
    return false;
  }

  string sFileName = m_sDataSubDir + HIGH_SCORES_FILENAME;

  ofstream file(sFileName.c_str(), ios_base::out | ios_base::trunc);

  if (!file)
  {
    cerr << "Couldn't open high scores file: " << sFileName << endl;
    cerr << "Can't save high scores!" <<  endl;

    return false;
  }

  int nCounter = 0;
  int nScore   = 0;

  string sName;

  for (multimap<int, string>::iterator it = m_mapHighScores.begin();
       it != m_mapHighScores.end(); ++it)
  {
    nScore = (*it).first;
    sName  = (*it).second;

    file << nScore << " " << sName << endl;

    nCounter++;

    // We only write 10 scores to file! (for safety...)
    if (nCounter > 10)
      break;
  }

  return true;
}

void Config::getHighScores(multimap<int, string>& mapHighScores)
{
  copyMaps(m_mapHighScores, mapHighScores);
}

void Config::setHighScores(multimap<int, string>& mapHighScores)
{
  copyMaps(mapHighScores, m_mapHighScores);
}

void Config::copyMaps(multimap<int, string>& mapOrig,
                      multimap<int, string>& mapDest)
{
  // Clear old map
  mapDest.clear();

  int nScore   = 0;
  string sName;

  for (multimap<int, string>::iterator it = mapOrig.begin();
       it != mapOrig.end(); ++it)
  {
    nScore = (*it).first;
    sName  = (*it).second;

    mapDest.insert(pair<int, string>(nScore, sName));
  }
}
