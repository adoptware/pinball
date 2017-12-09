//#ident "$Id: Pinball.cpp,v 1.44 2003/06/18 10:43:44 henqvist Exp $"
/***************************************************************************
                          Pinball.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000-2016 by Henrik Enqvist
    email                : henqvist@excite.com


    ========================= Modifications =========================

        Dec. 9, 2017:
            Reset key is no longer hard-coded. (c30zD)

***************************************************************************/

#include <fstream>
#include <string>
#include <iostream>

//#include <sstream>
#include <strstream> //TODO:

#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <libintl.h>
#include <locale.h>

#ifdef _MSC_VER //!+rzr : maybe we can use a portable IO lib such as physicfs
#include <io.h> //find
#include <direct.h> //getcwd
#else
#include <dirent.h>
#endif
#include "Private.h" // macro flags defined here

#ifdef HAVE_UNISTD_H
#include <unistd.h> // not in msvc (replaced by io.h)
#endif //!-rzr

#include "Pinball.h"
#include "Keyboard.h"
#include "Menu.h"
#include "Engine.h"
#include "Camera.h"
#include "Light.h"
#include "BigSphere.h"
#include "Cylinder.h"
#include "BounceBehavior.h"
#include "KeyBehavior.h"
#include "Grid.h"
#include "KeyRotBehavior.h"
#include "Polygon.h"
#include "TextureUtil.h"
#include "SoundUtil.h"
#include "Score.h"
#include "CollisionBounds.h"
#include "StateMachine.h"
//#include "EyeBehavior.h"
#include "Config.h"
#include "EmFont.h"
#include "Loader.h"
#include "BallGroup.h"
#include "Table.h"

#if EM_USE_SDL
#include <SDL.h>
#endif

#define AMBIENT 0.05f

void get_config(void);

float g_RatioArray[] 
= { 1./2. , 5./9. , 9./16. , 10./16. , 2./3. , 3./4. , 4./5. , 1./1. , 5./4. , 4./3. , 3./2. , 16./10. , 16./9. , 9./5., 2./1. };

int g_WidthArray[] 
= { 320, 400, 512, 640, 720, 800 , 864, 900, 1024, 1080, 1280 , 1680 , 1920};

/****************************************************************************
 * Menus
 ***************************************************************************/

MenuChoose* menusnd = NULL;
MenuChoose* menumusic = NULL;
MenuChoose* menubright = NULL;
MenuChoose* menuscreen = NULL;
MenuChoose* menusize = NULL;
MenuChoose* menuratio = NULL;
MenuChoose* menuview = NULL;
MenuChoose* menufilter = NULL;
MenuChoose* menufps = NULL;
MenuChoose* menufire = NULL;
MenuChoose* menulights = NULL;

/****************************************************************************
 * Define some menu classes
 ***************************************************************************/

// Menu item to display high scores - pnf
class MyMenuHighScores : public MenuFct {
public:
  MyMenuHighScores(const char * name, int (*fct)(void), Engine *e) : MenuFct(name, name, fct, e) {};
protected:
  int perform() {
    p_Engine->clearScreen();

    if (p_Texture != NULL)
      p_Engine->drawSplash(p_Texture);

    p_EmFont->printRowCenter(string(string("- ") + gettext("High Scores") +  string(" -")).c_str(), 2);
    string sHeader = string(gettext("table: ")) + Table::getInstance()->getTableName();
    p_EmFont->printRowCenter(sHeader.c_str(), 3);

    int nStartRow = 5;
    string sRow = "X";

    list<string> listHighScores;

    Table::getInstance()->getHighScoresData(listHighScores);

    if (listHighScores.size() > 0) {
      list<string>::iterator iter = listHighScores.begin();
      list<string>::iterator end = listHighScores.end();
      for (int i=0; i<EM_MAX_HIGHSCORES && iter != end; ++i, ++iter) {
        sRow = (*iter);
        p_EmFont->printRowCenter(sRow.c_str(), nStartRow + i);
      }
    } else {
      p_EmFont->printRowCenter(gettext("No Table Loaded!"), 10);
    }

    p_EmFont->printRowCenter(gettext("Hit any key for main menu..."), 20);

    p_Engine->swap();
    Keyboard::waitForKey();

    return EM_MENU_NOP;
  }
};

/** Menu item for changing a key binding. */
class MyMenuKey : public MenuFct {
public:
  MyMenuKey(const char * name, const char* nameDisplay, int (*fct)(void), Engine *e) : MenuFct(name, nameDisplay, fct, e) {};
protected:
  int perform () {
    p_Engine->clearScreen();
    if (p_Texture != NULL) p_Engine->drawSplash(p_Texture);
    p_EmFont->printRowCenter(gettext("press a key"), 10);
    p_Engine->swap();
    EMKey key = Keyboard::waitForKey();
    string name(m_Name);
    Config::getInstance()->setKey(name, key);
    return EM_MENU_NOP;
  }
  const char * getText() {
    ostrstream stm; //TODO
    stm.clear();
    string name(m_Name);
    const char * keyname = Config::getInstance()->getKeyCommonName(Config::getInstance()->getKey(name));
    string key;
    if (keyname != NULL) {
      key = string(keyname);
    } else {
      key = string(gettext("unknown"));
    }
    name = string(m_NameDisplay) + ":";
    while (name.size() < 12) {
      name = name + " ";
    }
    while (key.size() < 12) {
      key = " " + key;
    }
    stm << name << key << '\0';
    string text = stm.str();
    return text.c_str();
  }
};


/** The apply meny item. */
class MyMenuApply : public MenuFct {
public:
  MyMenuApply(const char * name, int (*fct)(void), Engine *e) : MenuFct(name, name, fct, e) {};
protected:
  int perform () {
    Config* config = Config::getInstance();
    TextureUtil * textureutil = TextureUtil::getInstance();
    // sound
    switch (menusnd->getCurrent()) {
    case 0: config->setSound(0); break;
    case 1: config->setSound(1); break;
    case 2: config->setSound(2); break;
    case 3: config->setSound(3); break;
    case 4: config->setSound(4); break;
    case 5: config->setSound(5); break;
    case 6: config->setSound(6); break;
    case 7: config->setSound(7); break;
    case 8: config->setSound(8); break;
    }
    switch (menumusic->getCurrent()) {
    case 0: config->setMusic(0); break;
    case 1: config->setMusic(1); break;
    case 2: config->setMusic(2); break;
    case 3: config->setMusic(3); break;
    case 4: config->setMusic(4); break;
    case 5: config->setMusic(5); break;
    case 6: config->setMusic(6); break;
    case 7: config->setMusic(7); break;
    case 8: config->setMusic(8); break;
    }
    SoundUtil::getInstance()->applyConfigVolume();
    // fullscreen
    if (menuscreen->getCurrent() == 0) {
      if (config->useFullScreen() == false) {
#if EM_USE_SDL
        SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
#endif
      }
      config->setFullScreen(true);
    } else {
      if (config->useFullScreen() == true) {
#if EM_USE_SDL
        SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
#endif
      }
      config->setFullScreen(false);
    }
    // brightness
    switch (menubright->getCurrent()) {
    case 0:
      menubright->getEngine()->setLightning(0.3f, AMBIENT);
      Config::getInstance()->setBrightness(0.3); break;
    case 1:
      menubright->getEngine()->setLightning(0.4f, AMBIENT);
      Config::getInstance()->setBrightness(0.4f); break;
    case 2:
      menubright->getEngine()->setLightning(0.5f, AMBIENT);
      Config::getInstance()->setBrightness(0.5f); break;
    case 3:
      menubright->getEngine()->setLightning(0.6f, AMBIENT);
      Config::getInstance()->setBrightness(0.6f); break;
    case 4:
      menubright->getEngine()->setLightning(0.7f, AMBIENT);
      Config::getInstance()->setBrightness(0.7f); break;
    case 5:
      menubright->getEngine()->setLightning(0.8f, AMBIENT);
      Config::getInstance()->setBrightness(0.8f); break;
    default:
      menubright->getEngine()->setLightning(0.5f, AMBIENT);
      Config::getInstance()->setBrightness(0.5f); break;
    }

    {
      // scren ratio
      float ratio=4./3.;
      {
	int index = menuratio->getCurrent();
	if ( index < ( sizeof(g_RatioArray) / sizeof( g_RatioArray[0]) ) ) {
	  ratio = g_RatioArray[ index ];
	}
	config->setRatio( ratio );
      }
     
      // screen size
      int w=640, h=480;
      {
      
	int index = menusize->getCurrent();
	if ( index < ( sizeof(g_WidthArray) / sizeof(g_WidthArray[0]) ) ) {
	  w = g_WidthArray[ index ];
	}
	h=w/ratio;
      }
      
      if (  (config->getWidth() != w)  || (config->getHeight() != h) ) {
#ifdef  EM_USE_SDL
	SDL_SetVideoMode(w, h, config->getBpp(),
			 SDL_OPENGL
			 | (config->useFullScreen() ? SDL_FULLSCREEN : 0));
#endif // SDL
	TextureUtil::getInstance()->resizeView(w, h);
	
#ifdef WIN32 //~rzr:{  //cout<<("Workround bug (for WIN32) + macosx etc");
	//TextureUtil::getInstance()->reloadTextures(); //TODO: fix the w32 bug
	TextureUtil::getInstance()->freeTextures(); // "hide" the w32 bug
	string filename =
	  Config::getInstance()->getDataDir() + string("/font_34.png");
	EmFont::getInstance()->loadFont(filename.c_str());
	//cout<<"may not be  driver bug cos it also happends under wine"<<endl;
	// unload level and textures //TODO: Reload Splash Screen
	Table::getInstance()->clear(Engine::getCurrentEngine() );
#endif //~rzr:} //cout<<"@w32 / resizing unreference textures (mipmaping?)"<<endl;
      }

      config->setSize(w, h);
    }

    switch (menuview->getCurrent()) {
    case 1: config->setView(1); break;
    case 2: config->setView(2); break;
    case 3: config->setView(3); break;
    case 4: config->setView(4); break;
    case 5: config->setView(5); break;
    default: config->setView(0);
    }
    // texture filter
    if (menufilter->getCurrent() == 0) {
      config->setGLFilter(EM_LINEAR);
    } else if (menufilter->getCurrent() == 1) {
      config->setGLFilter(EM_NEAREST);
    } else {
      config->setGLFilter(-1);
    }
    // fps
    if (menufps->getCurrent() == 0) {
      config->setShowFPS(false);
    } else {
      config->setShowFPS(true);
    }
    // fire
    if (menufire->getCurrent() == 0) {
      config->setFire(false);
      for (int a=0; a<MAX_BALL; ++a) {
        BallGroup * bg = Table::getInstance()->getBall(a);
        if (bg != NULL) {
          Behavior * beh = bg->getBehavior();
          EmAssert(beh != NULL, "MyMenuApply::perform behavior NULL");
          EmAssert(beh->getType() == PBL_TYPE_BOUNCEBEH,
                   "MyMenuApply::perform behavior not bouncebehavior");
          ((BounceBehavior*)beh)->setFire(false);
        }
      }
    } else {
      config->setFire(true);
      for (int a=0; a<MAX_BALL; ++a) {
        BallGroup * bg = Table::getInstance()->getBall(a);
        if (bg != NULL) {
          Behavior * beh = bg->getBehavior();
          EmAssert(beh != NULL, "MyMenuApply::perform behavior NULL");
          EmAssert(beh->getType() == PBL_TYPE_BOUNCEBEH,
                   "MyMenuApply::perform behavior not bouncebehavior");
          ((BounceBehavior*)beh)->setFire(true);
        }
      }
    }
    // dynamic lights
    if (menulights->getCurrent() == 0) {
      config->setLights(true);
    } else {
      config->setLights(false);
    }
    get_config();
    return EM_MENU_NOP;
  }
};

/** The back button. */
class MyMenuCancel : public MenuFct {
public:
  MyMenuCancel(const char * name, int (*fct)(void), Engine *e) : MenuFct(name, name, fct, e) {};
protected:
  int perform () {
    get_config();
    return EM_MENU_BACK;
  }
};

/** Button for loading a table. */
class MyMenuLoad : public MenuFct {
public:
  MyMenuLoad(const char * name, int (*fct)(void), Engine *e) : MenuFct(name, name, fct, e) {

  };
protected:
  int perform () {
    p_Engine->clearScreen();
    if (p_Texture != NULL) p_Engine->drawSplash(p_Texture);
    p_EmFont->printRowCenter(gettext("LOADING"), 10);
    p_Engine->swap();

    // Save the high scores of current table, if any - pnf
    Table::getInstance()->writeHighScoresFile();

    if (Table::getInstance()->loadLevel(p_Engine, m_Name) == 0) {
      // Load high scores for this table - pnf
      Table::getInstance()->readHighScoresFile();

      p_Engine->clearScreen();
      if (p_Texture != NULL) p_Engine->drawSplash(p_Texture);
      p_EmFont->printRowCenter(gettext("OK"), 10);
      p_Engine->swap();
      p_Engine->delay(500);
    } else {
      p_Engine->clearScreen();
      if (p_Texture != NULL) p_Engine->drawSplash(p_Texture);
      p_EmFont->printRowCenter(gettext("ERROR"), 10);
      p_Engine->swap();
      p_Engine->delay(1000);
    }
    return EM_MENU_BACK;
  }
};


/****************************************************************************
 * Menu functions
 ***************************************************************************/

/** Update the current menu with the configuration. */
void get_config(void) 
{
  // sound
 
  menusnd->setCurrent( Config::getInstance()->getSound() );

  switch (Config::getInstance()->getMusic()) {
  case 0: menumusic->setCurrent(0); break;
  case 1: menumusic->setCurrent(1); break;
  case 2: menumusic->setCurrent(2); break;
  case 3: menumusic->setCurrent(3); break;
  case 4: menumusic->setCurrent(4); break;
  case 5: menumusic->setCurrent(5); break;
  case 6: menumusic->setCurrent(6); break;
  case 7: menumusic->setCurrent(7); break;
  case 8: menumusic->setCurrent(8); break;
  }
  // fullscreen
  if (Config::getInstance()->useFullScreen()) {
    menuscreen->setCurrent(0);
  } else {
    menuscreen->setCurrent(1);
  }
  // brightness
  if (Config::getInstance()->getBrightness() < 0.35f) {
    menubright->setCurrent(0);
  } else if (Config::getInstance()->getBrightness() < 0.45f) {
    menubright->setCurrent(1);
  } else if (Config::getInstance()->getBrightness() < 0.55f) {
    menubright->setCurrent(2);
  } else if (Config::getInstance()->getBrightness() < 0.65f) {
    menubright->setCurrent(3);
  } else if (Config::getInstance()->getBrightness() < 0.75f) {
    menubright->setCurrent(4);
  } else {
    menubright->setCurrent(5);
  }

  { //cerr<<"// screen ratio="<<Config::getInstance()->getRatio()<<endl;
    int array_size = sizeof( g_RatioArray ) / sizeof( g_RatioArray[0] );
    menuratio->setCurrent(9);
    for (int i=array_size-1; (i>=0) ; i-- ) {
      if (Config::getInstance()->getRatio() == g_RatioArray[i]) {
	menuratio->setCurrent(i);
	i=0;
      }
    }
  }

  { // screen size
    int array_size = sizeof( g_WidthArray ) / sizeof( g_WidthArray[0] );
    menusize->setCurrent(0);
    for (int i=array_size-1; (i>=0) ; i-- ) {
      if (Config::getInstance()->getWidth() == g_WidthArray[i]) {
	menusize->setCurrent(i);
	i=0;
      }    
    }
  }

  // view mode
  switch(Config::getInstance()->getView()) {
  case 1: menuview->setCurrent(1); break;
  case 2: menuview->setCurrent(2); break;
  case 3: menuview->setCurrent(3); break;
  case 4: menuview->setCurrent(4); break;
  case 5: menuview->setCurrent(5); break;
  default: menuview->setCurrent(0);
  }
  // texture filter
  if (Config::getInstance()->getGLFilter() == EM_LINEAR) {
    menufilter->setCurrent(0);
  } else if (Config::getInstance()->getGLFilter() == EM_NEAREST) {
    menufilter->setCurrent(1);
  } else {
    menufilter->setCurrent(2);
  }
  // show fps
  if (Config::getInstance()->getShowFPS()) {
    menufps->setCurrent(1);
  } else {
    menufps->setCurrent(0);
  }
  // fire
  if (Config::getInstance()->getFire()) {
    menufire->setCurrent(1);
  } else {
    menufire->setCurrent(0);
  }
  // dynamic lights
  if (Config::getInstance()->useLights()) {
    menulights->setCurrent(0);
  } else {
    menulights->setCurrent(1);
  }
}

/* Create some menus */
MenuItem* createMenus(Engine * engine) {
  // Create the meny
  MenuSub* menu = new MenuSub(gettext("main menu"), engine);
  menu->setBottomText("github.com/sergiomb2/pinball");
  MenuSub* menuresume = new MenuSub(gettext("play"), engine);
  menuresume->setAction(EM_MENU_RESUME);
  menu->addMenuItem(menuresume);

  MenuSub* menuload = new MenuSub(gettext("load table"), engine);
  menu->addMenuItem(menuload);

  // Show high scores for current loadad table - pnf
  MyMenuHighScores* menuhighscores = new MyMenuHighScores(gettext("High Scores"), NULL,
                                                          engine);
  menu->addMenuItem(menuhighscores);

  MenuSub* menucfg = new MenuSub(gettext("config"), engine);
  menu->addMenuItem(menucfg);

  MenuSub* menuexit = new MenuSub(gettext("exit"), engine);
  menuexit->setAction(EM_MENU_EXIT);
  menu->addMenuItem(menuexit);

  MenuSub* menugfx = new MenuSub(gettext("graphics"), engine);
  menucfg->addMenuItem(menugfx);

  MenuSub* menuaudio = new MenuSub(gettext("audio"), engine);
  menucfg->addMenuItem(menuaudio);

  MenuSub* menukey = new MenuSub(gettext("keyboard"), engine);
  menukey->setBottomText(gettext("shortcuts for view change F5-F8"));
  menucfg->addMenuItem(menukey);

  string filename = string(Config::getInstance()->getDataSubDir()) + "/splash.png";
#if EM_USE_ALLEGRO
  filename += ".pcx";
#endif
  EmTexture * tex = TextureUtil::getInstance()->loadTexture(filename.c_str());
  if (tex != NULL) {
    menu->setBackground(tex);
    menuload->setBackground(tex);
    menucfg->setBackground(tex);
    menugfx->setBackground(tex);
    menuaudio->setBackground(tex);
    menukey->setBackground(tex);
    menuhighscores->setBackground(tex);
  } else {
    cerr << "Error loading data/splash.png" << endl;
  }

  // create one entry for each directory
  // TODO scrolling text if too many tables
#if ( HAVE_UNISTD_H ) // __GNUC__ //!+rzr
  DIR * datadir = opendir(Config::getInstance()->getDataDir());
  char cwd[256];
  if (datadir != NULL && getcwd(cwd, 256) != NULL) {
    struct dirent * entry;
    struct stat statbuf;
    //cerr<<  Config::getInstance()->getDataDir() <<endl; //!+rzr
    int status = chdir(Config::getInstance()->getDataDir());
    if ( status == 0 ) {
      while ((entry = readdir(datadir)) != NULL) {
	lstat(entry->d_name, &statbuf);
	if (S_ISDIR(statbuf.st_mode) &&
	    strcmp(".", entry->d_name) != 0 &&
	    strcmp("..", entry->d_name) != 0) {
	  MenuFct * menufct = new MyMenuLoad(entry->d_name, NULL, engine);
	  menuload->addMenuItem(menufct);
	  if (tex != NULL) {
	    menufct->setBackground(tex);
	  }
	}
      }
      chdir(cwd);
    } else {
      cerr<<"error: io: datadir "<<endl;
    }
    closedir(datadir);
  }
#else
#ifdef _MSC_VER //!+rzr : thanx to ramlaid ;)
  struct _finddata_t dirFile;
  long hFile=0;
  char cwd[256];
  if (getcwd(cwd,256) != NULL)  {
    chdir(Config::getInstance()->getDataDir());
    if( (hFile = _findfirst( "*", &dirFile )) != NULL ) {
      do {
        if ((dirFile.attrib & _A_SUBDIR) != 0)  {
          if (strcmp(".", dirFile.name) != 0
              && strcmp("..", dirFile.name) != 0) {
            MenuFct * menufct = new MyMenuLoad(dirFile.name, NULL, engine);
            menuload->addMenuItem(menufct);
            if (tex != NULL) {
              menufct->setBackground(tex);
            }
          }
        }
      }
      while( _findnext( hFile, &dirFile ) == 0 );
      chdir(cwd);
      _findclose( hFile );
    }
  }
#else
#warning "check your compiler here"
#endif
#endif //!+rzr

  menuview = new MenuChoose(engine);
  menuview->addText(gettext("view:         classic"));
  menuview->addText(gettext("view:   softly moving"));
  menuview->addText(gettext("view:          moving"));
  menuview->addText(gettext("view:             top"));
  menuview->addText(  "view:        top-down");
  menuview->addText(  "view: top-down-rotated");
  menugfx->addMenuItem(menuview);

  menuscreen = new MenuChoose(engine);
  menuscreen->addText(gettext("screen:    fullscreen"));
  menuscreen->addText(gettext("screen:      windowed"));
  menugfx->addMenuItem(menuscreen);

  menubright = new MenuChoose(engine);
  menubright->addText(gettext("brightness:    =....."));
  menubright->addText(gettext("brightness:    ==...."));
  menubright->addText(gettext("brightness:    ===..."));
  menubright->addText(gettext("brightness:    ====.."));
  menubright->addText(gettext("brightness:    =====."));
  menubright->addText(gettext("brightness:    ======"));
  menugfx->addMenuItem(menubright);

  {
    menusize = new MenuChoose(engine);
    menusize->addText(gettext("screen width:     320"));
    menusize->addText(gettext("screen width:     400"));
    menusize->addText(gettext("screen width:     512"));
    menusize->addText(gettext("screen width:     640"));
    menusize->addText(gettext("screen width:     720"));
    menusize->addText(gettext("screen width:     800"));
    menusize->addText(gettext("screen width:     864"));
    menusize->addText(gettext("screen width:     900"));
    menusize->addText(gettext("screen width:    1024"));
    menusize->addText(gettext("screen width:    1080"));
    menusize->addText(gettext("screen width:    1280"));
    menusize->addText(gettext("screen width:    1680"));
    menusize->addText(gettext("screen width:    1920"));
    menugfx->addMenuItem(menusize);
  }
  {
    menuratio = new MenuChoose(engine);
    menuratio->addText(gettext("ratio:      0.5 (1/2)"));
    menuratio->addText( "ratio:  0.55 (5/9)");
    menuratio->addText( "ratio:  0.56 (9/16)");
    menuratio->addText( "ratio:  0.62 (10/16)");
    menuratio->addText( "ratio:  0.66 (2/3)");
    menuratio->addText( "ratio:  0.75 (3/4)");
    menuratio->addText( "ratio:  0.8 (4/5)");
    menuratio->addText(gettext("ratio:        1 (1/1)"));
    menuratio->addText(gettext("ratio:      1.2 (5/4)"));
    menuratio->addText(gettext("ratio:      1.3 (4/3)"));
    menuratio->addText(gettext("ratio:      1.5 (3/2)"));
    menuratio->addText(gettext("ratio:    1.6 (16/10)"));
    menuratio->addText(gettext("ratio:     1.7 (16/9)"));
    menuratio->addText(gettext("ratio:      1.8 (9/5)"));
    menuratio->addText(gettext("ratio:        2 (2/1)"));
    menugfx->addMenuItem(menuratio);
  }

  menufilter = new MenuChoose(engine);
  menufilter->addText(gettext("texture:       nicest")); //was linear // gamers  //!rzr
  menufilter->addText(gettext("texture:      fastest")); //was nearest // vocabulary :)
  menufilter->addText(gettext("texture:         none"));
  menugfx->addMenuItem(menufilter);

  menufps = new MenuChoose(engine);
  menufps->addText(gettext("show fps:          no"));
  menufps->addText(gettext("show fps:         yes"));
  menugfx->addMenuItem(menufps);

  menufire = new MenuChoose(engine);
  menufire->addText(gettext("fire effect:       no"));
  menufire->addText(gettext("fire effect:      yes"));
  menugfx->addMenuItem(menufire);

  menulights = new MenuChoose(engine);
  menulights->addText(gettext("dynamic lights:   yes"));
  menulights->addText(gettext("dynamic lights:    no"));
  menugfx->addMenuItem(menulights);

  //   menumaxfps = new MenuChoose(engine);
  //   menumaxfps->addText(gettext("max fps            50"));
  //   menumaxfps->addText(gettext("max fps           100"));
  //   menugfx->addMenuItem(menumaxfps);

  menusnd = new MenuChoose(engine);
  menusnd->addText(gettext("sound:            off"));
  menusnd->addText(gettext("sound:       =......."));
  menusnd->addText(gettext("sound:       ==......"));
  menusnd->addText(gettext("sound:       ===....."));
  menusnd->addText(gettext("sound:       ====...."));
  menusnd->addText(gettext("sound:       =====..."));
  menusnd->addText(gettext("sound:       ======.."));
  menusnd->addText(gettext("sound:       =======."));
  menusnd->addText(gettext("sound:       ========"));
  menuaudio->addMenuItem(menusnd);

  menumusic = new MenuChoose(engine);
  menumusic->addText(gettext("music:            off"));
  menumusic->addText(gettext("music:       =......."));
  menumusic->addText(gettext("music:       ==......"));
  menumusic->addText(gettext("music:       ===....."));
  menumusic->addText(gettext("music:       ====...."));
  menumusic->addText(gettext("music:       =====..."));
  menumusic->addText(gettext("music:       ======.."));
  menumusic->addText(gettext("music:       =======."));
  menumusic->addText(gettext("music:       ========"));
  menuaudio->addMenuItem(menumusic);

  MyMenuKey * menuleftflip = new MyMenuKey("leftflip", gettext("leftflip"), NULL, engine);
  menukey->addMenuItem(menuleftflip);
  MyMenuKey * menurightflip = new MyMenuKey("rightflip", gettext("rightflip"), NULL, engine);
  menukey->addMenuItem(menurightflip);
  MyMenuKey * menuleftnudge = new MyMenuKey("leftnudge", gettext("leftnudge"), NULL, engine);
  menukey->addMenuItem(menuleftnudge);
  MyMenuKey * menurightnudge = new MyMenuKey("rightnudge", gettext("rightnudge"), NULL, engine);
  menukey->addMenuItem(menurightnudge);
  MyMenuKey * menubottomnudge = new MyMenuKey("bottomnudge", gettext("bottomnudge"), NULL, engine);
  menukey->addMenuItem(menubottomnudge);
  MyMenuKey * menulaunch = new MyMenuKey("launch", gettext("launch"), NULL, engine);
  menukey->addMenuItem(menulaunch);
  MyMenuKey * menureset = new MyMenuKey("reset", gettext("reset"), NULL, engine);
  menukey->addMenuItem(menureset);

  MenuFct* menuapply = new MyMenuApply(gettext("apply"), NULL, engine);
  menuaudio->addMenuItem(menuapply);
  //menucfg->addMenuItem(menuapply);
  menugfx->addMenuItem(menuapply);
  //menukey->addMenuItem(menuapply);

  MenuFct* menucancel = new MyMenuCancel(gettext("back"), NULL, engine);
  menucfg->addMenuItem(menucancel);
  menugfx->addMenuItem(menucancel);
  menuaudio->addMenuItem(menucancel);
  menuload->addMenuItem(menucancel);
  menukey->addMenuItem(menucancel);

  get_config();
  return menu;
}


/** Main */
int main(int argc, char *argv[]) {
  //cerr<<"+ Pinball::main"<<endl;

  setlocale(LC_ALL, "");
  bindtextdomain("pinball", EM_LOCALIZATION_DIR);
  textdomain("pinball");
  // No support for UTF-8 implemented.
  //bind_textdomain_codeset("pinball", "UTF-8");

  cerr << "table: " << endl;

  try {
    // Create a engine and parse emilia arguments
    Config::getInstance()->loadConfig();
    Engine * engine = new Engine(argc, argv);

    float direct = 0.0f;
    if (Config::getInstance()->getBrightness() < 0.35f) {
      direct = 0.3f;
    } else if (Config::getInstance()->getBrightness() < 0.45f) {
      direct = 0.4f;
    } else if (Config::getInstance()->getBrightness() < 0.55f) {
      direct = 0.5f;
    } else if (Config::getInstance()->getBrightness() < 0.65f) {
      direct = 0.6f;
    } else if (Config::getInstance()->getBrightness() < 0.75f) {
      direct = 0.7f;
    } else {
      direct = 0.8f;
    }
    engine->setLightning(direct, AMBIENT);

#if EM_USE_SDL
    string filename = Config::getInstance()->getDataDir() + string("/font_34.png");
#endif
#if EM_USE_ALLEGRO
    string filename = Config::getInstance()->getDataDir() + string("/font_35.pcx");
#endif

    EmFont::getInstance()->loadFont(filename.c_str());

    // Add a score board and a menu.
    MenuItem* menu = createMenus(engine);

    // Draw to the screen.
    int all = 0;

    engine->resetTick();

    while (! (  SDL_QuitRequested()  //cout<<"catch close win"<<endl; //!rzr
                || Keyboard::isKeyDown(SDLK_INSERT)))  {
#if EM_DEBUG
      if (Keyboard::isKeyDown(SDLK_p)) {
        Keyboard::waitForKey();
        Keyboard::clear();
        engine->resetTick();
      }
#endif
      if (Keyboard::isKeyDown(SDLK_ESCAPE) || all == 0) {
        SoundUtil::getInstance()->pauseMusic();
        if (menu->perform() == EM_MENU_EXIT) {
          break;
        }
        engine->resetTick();
        SoundUtil::getInstance()->resumeMusic();
      }

      if (Keyboard::isKeyDown(Config::getInstance()->getKey("reset"))) {
        SendSignal(PBL_SIG_RESET_ALL, 0, engine, NULL);
      }

      if (engine->nextTickFPS(200)) {
        engine->tick();
      } else {
        engine->render();
        if (Table::getInstance()->getScore() != NULL) {
          Table::getInstance()->getScore()->draw();
        }
        if (engine->getGroup(0) == NULL) {
          EmFont::getInstance()->printRowCenter(gettext("no table loaded"), 6);
          EmFont::getInstance()->printRowCenter(gettext("press esc"), 8);
        }
        engine->swap();
      }

      all++;
      //engine->limitFPS(100);
    }

    Config::getInstance()->saveConfig();

    // Write high scores to disk - pnf
    Table::getInstance()->writeHighScoresFile();

    delete(engine);
  } catch (string str) {
    cerr << "EXCEPTION: " << str << endl;
  }
  return EXIT_SUCCESS;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif


/// entry point function (main) for w32 codewarrior
#if( (defined WIN32 ) && ( defined __MWERKS__ ) )

extern "C" {
  /**
   * @author: www.Philippe.COVAL.free.FR - $rev: $author
   * Was need to convert win32 args to ansi way :
   * int argc; char** argv;
   * convertStringWords( GetCommandLine(), &argc, &argv);
   * TODO : check if quotes are wanted or not ?
   **/

  void convertStringWords( char * arg,
                           int *argc, char*** argv);


  void convertStringWords( char * arg,
                           int *argc, char*** argv)
  {
    char *b, *e, *q;
    int i=0;
    int n;
    b = arg;
    q = e  = b ;
    //debug("+ convertStringWords");  debug(arg);
    *argc=0;
    *argv = (char**) malloc( sizeof(char*));

    do {
      while ( *b == ' ' ) b++;
      q=e=b-1;

      do { q = strchr( q + 1 , '"'); }
      while ( (q != NULL) && ( *(q-1) == '\\' ) );

      do { e = strchr( e + 1 , ' '); }
      while ( (e != NULL) && ( *(e-1) == '\\' ) );
      //debugf("%u<%u ?\n",&q,&e);

      if ( (q != NULL) && ( e != NULL) && ( q < e ) ) {
        //debug("quoted");
        do { q = strchr( q + 1 , '"'); }
        while ( (q != NULL) && ( *(q-1) == '\\' ) );
        e = ++q;
      }

      if ( e != NULL) n = (e) - b; else n = strlen(b);
      //debugf("n=%d=%s;\n", n,b);


      *argv = (char**) realloc( *argv, ( (*argc) + 1 ) * sizeof(char*) );
      (*argv)[ *argc ] = (char*) malloc ( n +1);
      strncpy( (*argv)[ *argc ], b , n);
      *( (*argv)[ *argc ] + n ) = '\0';
      //debug( (*argv)[ *argc ] );

      b = e;
      (*argc)++;

    } while ( e != NULL );
    //debug("- convertStringWords");
  }

} //extern "C"

int WINAPI WinMain( HINSTANCE hInst,  HINSTANCE hPreInst,
                    LPSTR lpszCmdLine,  int nCmdShow )
{
  int argc=0; char** argv = 0;
  convertStringWords( GetCommandLine(), &argc, &argv);

  //int argc = 1;   char* argv[1] = { 0 };
  //argv[0] = GetCommandLine(); //cut on space
  return main(argc,argv);
}
#endif
// EOF $Id: Pinball.cpp,v 1.44 2003/06/18 10:43:44 henqvist Exp $
