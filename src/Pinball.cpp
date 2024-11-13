// -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
// #ident "$Id: Pinball.cpp $"
/***************************************************************************
                          Pinball.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
***************************************************************************/

#include <fstream>
#include <string>
#include <iostream>

#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <cstdlib>
#include <ctime>

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

using namespace std;

#define AMBIENT 0.05f

void get_config(void);

/****************************************************************************
 * Menus
 ***************************************************************************/

MenuChoose* menusnd = NULL;
MenuChoose* menumusic = NULL;
MenuChoose* menubright = NULL;
MenuChoose* menuscreen = NULL;
MenuChoose* menusize = NULL;
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
  MyMenuHighScores(const char * name, int (*fct)(void), Engine *e) : MenuFct(name, fct, e) {};
protected:
  int perform() {
    p_Engine->clearScreen();

    if (p_Texture != NULL)
      p_Engine->drawSplash(p_Texture);

    p_EmFont->printRowCenter("- High Scores -", 2);
    string sHeader = string("table: ") + Table::getInstance()->getTableName();
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
      p_EmFont->printRowCenter("No Table Loaded!", 10);
    }

    p_EmFont->printRowCenter("Hit any key for main menu...", 20);

    p_Engine->swap();
    Keyboard::waitForKey();

    return EM_MENU_NOP;
  }
};

/** Menu item for changing a key binding. */
class MyMenuKey : public MenuFct {
public:
  MyMenuKey(const char * name, int (*fct)(void), Engine *e) : MenuFct(name, fct, e) {};
protected:
  int perform () {
    p_Engine->clearScreen();
    if (p_Texture != NULL) p_Engine->drawSplash(p_Texture);
    p_EmFont->printRowCenter("press a key", 10);
    p_Engine->swap();
    EMKey key = Keyboard::waitForKey();
    string name(m_Name);
    Config::getInstance()->setKey(name, key);
    return EM_MENU_NOP;
  }
  const char * getText() {
    ostringstream stm;
    stm.clear();
    string name(m_Name);
    const char * keyname = Config::getInstance()->getKeyCommonName(Config::getInstance()->getKey(name));
    string key;
    if (keyname != NULL) {
      key = string(keyname);
    } else {
      key = string("unknown");
    }

	if (name.length() + key.length() > 15){  
	    std::size_t found = name.find("leftflip");
	if (found!=std::string::npos)
	    name = "LF";
	found = name.find("rightflip");
	if (found!=std::string::npos)
	    name = "RF";
	found = name.find("leftnudge");
	if (found!=std::string::npos)
	     name = "LN";
	found = name.find("rightnudge");
	if (found!=std::string::npos)
	     name = "RN";
	found = name.find("bottomnudge");
	if (found!=std::string::npos)
	     name = "BN";
	}

    name = name + ":";
    /*while (name.size() < 12) {
      name = name + " ";
    }
    while (key.size() < 12) {
      key = " " + key;
    }*/
    stm << name << key << '\0';
    string text = stm.str();
    return text.c_str();
  }
};


/** The apply meny item. */
class MyMenuApply : public MenuFct {
public:
  MyMenuApply(const char * name, int (*fct)(void), Engine *e) : MenuFct(name, fct, e) {};
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
	SDL_SetWindowFullscreen(TextureUtil::getInstance()->getWindow(),
				SDL_WINDOW_FULLSCREEN);
#endif
      }
      config->setFullScreen(true);
    } else {
      if (config->useFullScreen() == true) {
#if EM_USE_SDL
	SDL_SetWindowFullscreen(TextureUtil::getInstance()->getWindow(), 0);
	SDL_SetWindowSize(TextureUtil::getInstance()->getWindow(),
			  config->getWidth(), config->getHeight());
        TextureUtil::getInstance()->resizeView(config->getWidth(), config->getHeight());
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
    // screen size
    int w, h;
    switch (menusize->getCurrent()) {
    case 0: w = 320; h = 240; break;
    case 1: w = 400; h = 300; break;
    case 2: w = 512; h = 384; break;
    case 3: w = 640; h = 480; break;
    case 4: w = 800; h = 600; break;
    case 5: w = 1024; h = 768; break;
    case 6: w = 1280; h = 1024; break;
    case 7: w = 1920; h = 1200; break;
    default: w = 640; h = 480;
    }
    if (config->getWidth() != w) {
#if  EM_USE_SDL
      SDL_SetWindowSize(TextureUtil::getInstance()->getWindow(), w, h);
#endif // SDL
      TextureUtil::getInstance()->resizeView(w, h);
      //!rzr!+   //cout<<("Workround bug (for WIN32) + macosx etc");
#ifdef WIN32 ////TODO : check @w32  //need help FINISH
      //TextureUtil::getInstance()->reloadTextures(); //TODO: fix the w32 bug
      TextureUtil::getInstance()->freeTextures(); // "hide" the w32 bug
      string filename =
        Config::getInstance()->getDataDir() + string("/font_34.png");
      EmFont::getInstance()->loadFont(filename.c_str());
      //cout<<"may not be  driver bug cos it also happends under wine"<<endl;
      // unload level and textures //TODO: Reload Splash Screen
      Table::getInstance()->clear(Engine::getCurrentEngine() );
      // SDL bug ?
#endif //!rzr!- //cout<<"@w32 / resizing unreference textures (mipmaping?)"<<endl;
    }//!rzr!-

    config->setSize(w, h);

    switch (menuview->getCurrent()) {
    case 1: config->setView(1); break;
    case 2: config->setView(2); break;
    case 3: config->setView(3); break;
    case 4: config->setView(4); break;
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
  MyMenuCancel(const char * name, int (*fct)(void), Engine *e) : MenuFct(name, fct, e) {};
protected:
  int perform () {
    get_config();
    return EM_MENU_BACK;
  }
};

/** Button for loading a table. */
class MyMenuLoad : public MenuFct {
public:
  MyMenuLoad(const char * name, int (*fct)(void), Engine *e) : MenuFct(name, fct, e) {

  };
protected:
  int perform () {
    p_Engine->clearScreen();
    if (p_Texture != NULL) p_Engine->drawSplash(p_Texture);
    p_EmFont->printRowCenter("LOADING", 10);
    p_Engine->swap();

    // Save the high scores of current table, if any - pnf
    Table::getInstance()->writeHighScoresFile();

    if (Table::getInstance()->loadLevel(p_Engine, m_Name) == 0) {
      // Load high scores for this table - pnf
      Table::getInstance()->readHighScoresFile();

      p_Engine->clearScreen();
      if (p_Texture != NULL) p_Engine->drawSplash(p_Texture);
      p_EmFont->printRowCenter("OK", 10);
      p_Engine->swap();
      p_Engine->delay(500);
    } else {
      p_Engine->clearScreen();
      if (p_Texture != NULL) p_Engine->drawSplash(p_Texture);
      p_EmFont->printRowCenter("ERROR", 10);
      p_Engine->swap();
      p_Engine->delay(1000);
    }
    return EM_MENU_BACK;
  }
};


/****************************************************************************
 * Menu functions
 ***************************************************************************/

/** Update the current meny with the configuration. */
void get_config(void) {
  // sound
  switch (Config::getInstance()->getSound()) {
  case 0: menusnd->setCurrent(0); break;
  case 1: menusnd->setCurrent(1); break;
  case 2: menusnd->setCurrent(2); break;
  case 3: menusnd->setCurrent(3); break;
  case 4: menusnd->setCurrent(4); break;
  case 5: menusnd->setCurrent(5); break;
  case 6: menusnd->setCurrent(6); break;
  case 7: menusnd->setCurrent(7); break;
  case 8: menusnd->setCurrent(8); break;
  }
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
  // screen size
  if (Config::getInstance()->getWidth() == 320) {
    menusize->setCurrent(0);
  } else        if (Config::getInstance()->getWidth() == 400) {
    menusize->setCurrent(1);
  } else        if (Config::getInstance()->getWidth() == 512) {
    menusize->setCurrent(2);
  } else        if (Config::getInstance()->getWidth() == 640) {
    menusize->setCurrent(3);
  } else        if (Config::getInstance()->getWidth() == 800) {
    menusize->setCurrent(4);
  } else        if (Config::getInstance()->getWidth() == 1024) {
    menusize->setCurrent(5);
  } else 	if (Config::getInstance()->getWidth() == 1280) {
    menusize->setCurrent(6);
  } else 	if (Config::getInstance()->getWidth() == 1920) {
    menusize->setCurrent(7);
  } else {
    menusize->setCurrent(3);
  }
  // view mode
  switch(Config::getInstance()->getView()) {
  case 1: menuview->setCurrent(1); break;
  case 2: menuview->setCurrent(2); break;
  case 3: menuview->setCurrent(3); break;
  case 4: menuview->setCurrent(4); break;
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
  MenuSub* menu = new MenuSub("main menu", engine);
  menu->setBottomText(PACKAGE_URL " " PACKAGE_VERSION);

  MenuSub* menuresume = new MenuSub("play", engine);
  menuresume->setAction(EM_MENU_RESUME);
  menu->addMenuItem(menuresume);

  MenuSub* menuload = new MenuSub("load table", engine);
  menu->addMenuItem(menuload);

  // Show high scores for current loadad table - pnf
  MyMenuHighScores* menuhighscores = new MyMenuHighScores("High Scores", NULL,
                                                          engine);
  menu->addMenuItem(menuhighscores);

  MenuSub* menucfg = new MenuSub("config", engine);
  menu->addMenuItem(menucfg);

  MenuSub* menuexit = new MenuSub("exit", engine);
  menuexit->setAction(EM_MENU_EXIT);
  menu->addMenuItem(menuexit);

  MenuSub* menugfx = new MenuSub("graphics", engine);
  menucfg->addMenuItem(menugfx);

  MenuSub* menuaudio = new MenuSub("audio", engine);
  menucfg->addMenuItem(menuaudio);

  MenuSub* menukey = new MenuSub("keyboard", engine);
  //menukey->setBottomText("shorcuts for view change F5-F8");
  menukey->setBottomText("L-eft R-ight B-ottom F-lip N-udge");
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
    chdir(Config::getInstance()->getDataDir());
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
  menuview->addText(  "view:         classic");
  menuview->addText(  "view:   softly moving");
  menuview->addText(  "view:          moving");
  menuview->addText(  "view:             top");
  menuview->addText(  "view:            full");
  menugfx->addMenuItem(menuview);

  menuscreen = new MenuChoose(engine);
  menuscreen->addText("screen:    fullscreen");
  menuscreen->addText("screen:      windowed");
  menugfx->addMenuItem(menuscreen);

  menubright = new MenuChoose(engine);
  menubright->addText("brightness:    =.....");
  menubright->addText("brightness:    ==....");
  menubright->addText("brightness:    ===...");
  menubright->addText("brightness:    ====..");
  menubright->addText("brightness:    =====.");
  menubright->addText("brightness:    ======");
  menugfx->addMenuItem(menubright);

  menusize = new MenuChoose(engine);
  menusize->addText(  "screen size:  340x240");
  menusize->addText(  "screen size:  400x300");
  menusize->addText(  "screen size:  512x384");
  menusize->addText(  "screen size:  640x480");
  menusize->addText(  "screen size:  800x600");
  menusize->addText(  "screen size: 1024x768");
  menusize->addText(  "screen size:1280x1024");
  menusize->addText(  "screen size:1920x1200");
  menugfx->addMenuItem(menusize);

  menufilter = new MenuChoose(engine);
  menufilter->addText("texture:       nicest"); //was linear // gamers  //!rzr
  menufilter->addText("texture:      fastest"); //was nearest // vocabulary :)
  menufilter->addText("texture:         none");
  menugfx->addMenuItem(menufilter);

  menufps = new MenuChoose(engine);
  menufps->addText(   "show fps:          no");
  menufps->addText(   "show fps:         yes");
  menugfx->addMenuItem(menufps);

  menufire = new MenuChoose(engine);
  menufire->addText(  "fire effect:       no");
  menufire->addText(  "fire effect:      yes");
  menugfx->addMenuItem(menufire);

  menulights = new MenuChoose(engine);
  menulights->addText("dynamic lights:   yes");
  menulights->addText("dynamic lights:    no");
  menugfx->addMenuItem(menulights);

  //   menumaxfps = new MenuChoose(engine);
  //   menumaxfps->addText("max fps            50");
  //   menumaxfps->addText("max fps           100");
  //   menugfx->addMenuItem(menumaxfps);

  menusnd = new MenuChoose(engine);
  menusnd->addText(   "sound:            off");
  menusnd->addText(   "sound:       =.......");
  menusnd->addText(   "sound:       ==......");
  menusnd->addText(   "sound:       ===.....");
  menusnd->addText(   "sound:       ====....");
  menusnd->addText(   "sound:       =====...");
  menusnd->addText(   "sound:       ======..");
  menusnd->addText(   "sound:       =======.");
  menusnd->addText(   "sound:       ========");
  menuaudio->addMenuItem(menusnd);

  menumusic = new MenuChoose(engine);
  menumusic->addText( "music:            off");
  menumusic->addText( "music:       =.......");
  menumusic->addText( "music:       ==......");
  menumusic->addText( "music:       ===.....");
  menumusic->addText( "music:       ====....");
  menumusic->addText( "music:       =====...");
  menumusic->addText( "music:       ======..");
  menumusic->addText( "music:       =======.");
  menumusic->addText( "music:       ========");
  menuaudio->addMenuItem(menumusic);

  MyMenuKey * menuleftflip = new MyMenuKey("leftflip", NULL, engine);
  menukey->addMenuItem(menuleftflip);
  MyMenuKey * menurightflip = new MyMenuKey("rightflip", NULL, engine);
  menukey->addMenuItem(menurightflip);
  MyMenuKey * menuleftnudge = new MyMenuKey("leftnudge", NULL, engine);
  menukey->addMenuItem(menuleftnudge);
  MyMenuKey * menurightnudge = new MyMenuKey("rightnudge", NULL, engine);
  menukey->addMenuItem(menurightnudge);
  MyMenuKey * menubottomnudge = new MyMenuKey("bottomnudge", NULL, engine);
  menukey->addMenuItem(menubottomnudge);
  MyMenuKey * menulaunch = new MyMenuKey("launch", NULL, engine);
  menukey->addMenuItem(menulaunch);
  MyMenuKey * menureset = new MyMenuKey("reset", NULL, engine);
  menukey->addMenuItem(menureset);

  MenuFct* menuapply = new MyMenuApply("apply", NULL, engine);
  menuaudio->addMenuItem(menuapply);
  //menucfg->addMenuItem(menuapply);
  menugfx->addMenuItem(menuapply);
  //menukey->addMenuItem(menuapply);

  MenuFct* menucancel = new MyMenuCancel("back", NULL, engine);
  menucfg->addMenuItem(menucancel);
  menugfx->addMenuItem(menucancel);
  menuaudio->addMenuItem(menucancel);
  menuload->addMenuItem(menucancel);
  menukey->addMenuItem(menucancel);

  get_config();
  return menu;
}

Pinball::Pinball()
  : mpMenu{}, mpEngine{}, miCount{}, miMaxCount{}
{
  PINBALL(FUNCT());
  char* var= getenv("PINBALL_QUIT");
  if (var) {
    if (*var) { miMaxCount = atoi(var); }
    else { miMaxCount = 1; }
  }
}

int Pinball::run(int argc, char *argv[])
{
  PINBALL(FUNCT());
  int status = 0;
  try {
    status |= setup(argc, argv);
    while (status == 0) {
      status = loop();
    }
    switch (status) {
    case -EM_MENU_EXIT:
      status = EXIT_SUCCESS;
      break;
    default:
      if (status!=0) { cout<<"log: run: status="<<status<<endl;}
    }
    if (status>0) status=0;
  } catch(string error) {
    status |= 0xF0;
  }
  status |= finish(status);
  return status;
}


int Pinball::setup(int argc, char *argv[])
{
  PINBALL(FUNCT());
  for (auto i=0; i<argc; i++) {
    if ((strcmp(argv[i], "--version") == 0)
        || (strcmp(argv[i], "-v") == 0)) {
      cout << PACKAGE_STRING << endl;
      exit(EXIT_SUCCESS);
    } else if ((strcmp(argv[i], "--help") == 0) 
               || (strcmp(argv[i], "-h") == 0)) {
      cout << "Usage: pinball [OPTIONS]" << endl << endl
           << "Supported environment variables:" << endl << endl
           << "# PINBALL_QUIT (number of frame before exit)" << endl
           << "# PINBALL_TABLE (name of table or random)" << endl
           << "# PINBALL_WINDOW_FULLSCREEN_DESKTOP (SDL)" << endl
           << "# PINBALL_RENDER_DRIVER (3D)" << endl
           << "# PINBALL_BITPERPIXEL (SDL)" << endl;
#if EM_USE_SDL
      for (int i=0; i<SDL_GetNumVideoDrivers(); i++)
        cout << "# SDL_VIDEODRIVER='" << SDL_GetVideoDriver(i) << "'"
             << endl;
#endif
      exit(EXIT_SUCCESS);
    }
  }

  // Create a engine and parse emilia arguments
  Config::getInstance()->loadConfig();
  mpEngine = new Engine(argc, argv);

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
  mpEngine->setLightning(direct, AMBIENT);

#if EM_USE_SDL
  string filename = Config::getInstance()->getDataDir() + string("/font_34.png");
#endif
#if EM_USE_ALLEGRO
  string filename = Config::getInstance()->getDataDir() + string("/font_35.pcx");
#endif

  EmFont::getInstance()->loadFont(filename.c_str());

  // Add a score board and a menu.
  mpMenu = createMenus(mpEngine);

  // cerr<<"log: pinball: Draw to the screen"<<endl;
  miCount = 0;
  char* text = getenv("PINBALL_SPEED");
  int fps = (text) ? atoi(text) : 8*25;
  mpEngine->setSpeed(fps);
  mpEngine->resetTick();
  mpEngine->swap();

  char const * table = 0;
  if (miCount == 0 && !mpEngine->getGroup(0)) {
    if (table = getenv("PINBALL_TABLE")) {
      if (table && !*table) {
	int iTable = 0;
#if ! EM_DEBUG
	srand(time(NULL));
	iTable = rand() % 100 +1; // TODO reload last table from config
#endif
	table = (iTable<51) ? "professor" : "tux";
      }
      cout<<"log: pinball: Load table on init"<<endl;
      if (Table::getInstance()->loadLevel(mpEngine, table) == 0)
	Table::getInstance()->readHighScoresFile();
      Keyboard::generate(SDLK_RETURN);
      Keyboard::clear();
    }
  }
  mpEngine->resetTick();
  return 0;
}


int Pinball::loop()
{
  PINBALL(FUNCT());
  if (miMaxCount && miCount >= miMaxCount) {
    return 1;
  }
#if EM_USE_SDL
  if (SDL_QuitRequested())
    return 2;
#if EM_DEBUG
  if (Keyboard::isKeyDown(SDLK_INSERT))
    return 3;
#endif
#endif
#if EM_DEBUG
  if (Keyboard::isKeyDown(SDLK_p)) {
    Keyboard::waitForKey();
    Keyboard::clear();
    mpEngine->resetTick();
  }
#endif
  if (mpEngine->getGroup(0)) {
    if (miCount++ == 0) {
      cerr<<"log: Start table if loaded: "<< miCount<<endl;
      return 0;
    }
  }
  
  if (Keyboard::isKeyDown(SDLK_ESCAPE) || miCount == 0) {
    SoundUtil::getInstance()->pauseMusic();
    if (mpMenu->perform() == EM_MENU_EXIT) {
      return -EM_MENU_EXIT;
    }
    mpEngine->resetTick();
    SoundUtil::getInstance()->resumeMusic();
  }

  if (Keyboard::isKeyDown(SDLK_r)) {
    SendSignal(PBL_SIG_RESET_ALL, 0, mpEngine, NULL);
  }
  if (mpEngine->nextTick()) {
    mpEngine->tick();
  } else {
    mpEngine->render();
    if (Table::getInstance()->getScore() != NULL) {
      Table::getInstance()->getScore()->draw();
    } else {
      EmFont::getInstance()->printRowCenter("no table loaded", 6);
      EmFont::getInstance()->printRowCenter("press esc", 8);
    }
    mpEngine->swap();
  }
  miCount++;
#if EM_DEBUG
  //engine->limitFPS(100); // TODO: remove if no relevent
#endif
  return EXIT_SUCCESS;
}

int Pinball::finish(int status)
{
  PINBALL(FUNCT());
  Config::getInstance()->saveConfig();

    // Write high scores to disk - pnf
  Table::getInstance()->writeHighScoresFile();
  delete(mpEngine); mpEngine=0;
  return EXIT_SUCCESS;
}

/// Main

int Pinball::main(int argc, char *argv[])
{
  PINBALL(FUNCT());
  Pinball instance;
  int status = instance.run(argc, argv);
  cout << "pinball: exit: " << status << " fps: " << Engine::getFps() << endl;
  return status;
}

int main(int argc=0, char *argv[]=0);

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
  PINBALL(FUNCT());
  int argc=0; char** argv = 0;
  convertStringWords( GetCommandLine(), &argc, &argv);

  //int argc = 1;   char* argv[1] = { 0 };
  //argv[0] = GetCommandLine(); //cut on space
  return main(argc,argv);
}
#endif


int main(int argc, char *argv[])
{
  PINBALL(FUNCT());
  int status = EXIT_SUCCESS;
  status |=  Pinball::main(argc, argv);
  return status;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif

// #eof "$Id: Pinball.cpp $"
