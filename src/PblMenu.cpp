/***************************************************************************
                          PblMenu.cpp  -  description
                             -------------------
    begin                : Wed Aug 22 2001
    copyright            : (C) 2001 by Henrik Enqvist IB
    email                : henqvist@excite.com
 ***************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include "Private.h"
#include "Menu.h"
#include "EmFont.h"
#include "Config.h"
#include "Engine.h"
#include "TextureUtil.h"

#if EM_USE_SDL
#include <SDL.h>
#endif

MenuChoose* menusnd = NULL;
MenuChoose* menubright = NULL;
MenuChoose* menuscreen = NULL;
MenuChoose* menusize = NULL;
MenuChoose* menuview = NULL;
MenuChoose* menufilter = NULL;

/** Update the current meny with the configuration. */
void get_config(void) {
	// sound
	if (Config::getInstance()->useSound()) {
		menusnd->setCurrent(0);
	} else {
		menusnd->setCurrent(1);
	}
	// fullscreen
	if (Config::getInstance()->useFullScreen()) {
		menuscreen->setCurrent(0);
	} else {
		menuscreen->setCurrent(1);
	}
	// brightness
	if (Config::getInstance()->getBrightness() < 0.09f) {
		menubright->setCurrent(0);
	} else 	if (Config::getInstance()->getBrightness() > 0.11f) {
		menubright->setCurrent(2);
	} else {
		menubright->setCurrent(1);
	}
	// screen size
	if (Config::getInstance()->getWidth() == 320) {
		menusize->setCurrent(0);
	} else 	if (Config::getInstance()->getWidth() == 400) {
		menusize->setCurrent(1);
	} else 	if (Config::getInstance()->getWidth() == 512) {
		menusize->setCurrent(2);
	} else 	if (Config::getInstance()->getWidth() == 640) {
		menusize->setCurrent(3);
	} else 	if (Config::getInstance()->getWidth() == 800) {
		menusize->setCurrent(4);
	} else 	if (Config::getInstance()->getWidth() == 1024) {
		menusize->setCurrent(5);
	} else {
		menusize->setCurrent(0);
	}
	// view mode
	if (Config::getInstance()->getView() == 0) {
		menuview->setCurrent(0);
	} else {
		menuview->setCurrent(1);
	}
	// texture filter
	if (Config::getInstance()->getGLFilter() == EM_LINEAR) {
		menufilter->setCurrent(0);
	} else if (Config::getInstance()->getGLFilter() == EM_NEAREST) {
		menufilter->setCurrent(1);
	} else {
		menufilter->setCurrent(2);
	}
}

/** The apply meny item calls this function */	
int fctApply(void) {
	Config* config = Config::getInstance();
	TextureUtil * textureutil = TextureUtil::getInstance();
	// sound
	if (menusnd->getCurrent() == 0) {
		config->setSound(true);
	} else {
		config->setSound(false);
	}
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
	case 0: menubright->getEngine()->setLightning(0, 0); 
		Config::getInstance()->setBrightness(0); break;
	case 1: menubright->getEngine()->setLightning(0, 0.1f);
		Config::getInstance()->setBrightness(0.1f); break;
	case 2: menubright->getEngine()->setLightning(0, 0.2f);
		Config::getInstance()->setBrightness(0.2f); break;
	default: menubright->getEngine()->setLightning(0, 0.1f);
		Config::getInstance()->setBrightness(0.1f); break;
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
	default: w = 640; h = 480;
	}
	if (config->getWidth() != w) {
#if EM_USE_SDL
		SDL_SetVideoMode(w, h, config->getBpp(), 
										 SDL_OPENGL | (config->useFullScreen() ? SDL_FULLSCREEN : 0));
#endif
		textureutil->resizeView(w, h);
	}
	config->setSize(640, 480);

	if (menuview->getCurrent() == 0) {
		config->setView(0);
	} else {
		config->setView(1);
	}
	// texture filter
	if (menufilter->getCurrent() == 0) {
		config->setGLFilter(EM_LINEAR);
	} else if (menufilter->getCurrent() == 1) {
		config->setGLFilter(EM_NEAREST);
	} else {
		config->setGLFilter(-1);
	}

	get_config();
	return EM_MENU_NOP;
}

int fctCancel(void) {
	get_config();
	return EM_MENU_BACK;
}

/*
void fct

void fctSize(Meny * m, bool change) {
	if (change) {
		switch (Config::getInstance()->getWidth()) {
		case 640:	Config::getInstance()->setSize(800, 600); break;
		case 800: Config::getInstance()->setSize(1024, 768); break;
		default: Config::getInstance()->setSize(640, 480); break;
		}
	}
	switch (Config::getInstance()->getWidth()) {
	case 800:	m->setName("screen size: 800x600"); break;
	case 1024: m->setName("screen size: 1024x768"); break;
	default: m->setName("screen size: 640x480"); break;
	}
	EM_COUT("size", 1);
}

void fctSound(Meny * m, bool change) {
	switch (Config
  if (Config::getInstance()->useSound()) {
		Config::getInstance()->setSound(false);
		m->setName("sound: off");
	} else {
		Config::getInstance()->setSound(true);
		m->setName("sound: on");
	}
	EM_COUT("sound", 1);
}

void fctGrx(Meny * m) {
  if (Config::getInstance()->useFullScreen()) {
		Config::getInstance()->setFullScreen(false);
		m->setName("graphics: windowed");
		SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
	} else {
		Config::getInstance()->setFullScreen(true);
		m->setName("graphics: fullscreen");
		SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
	}
	EM_COUT("graphics", 1);
}	

void fctTexture(Meny * m) {
  if (Config::getInstance()->getGLFilter() == -1) {
		Config::getInstance()->setGLFilter(GL_NEAREST);
		m->setName("texture filter: nearest");
	} else if (Config::getInstance()->getGLFilter() == GL_NEAREST) {
		Config::getInstance()->setGLFilter(GL_LINEAR);
		m->setName("texture filter: linear");
	} else {
		Config::getInstance()->setGLFilter(-1);
		m->setName("texture filter: no textures");
	}
	EM_COUT("texture", 1);
}	

void fctView(Meny * m) {
	if (Config::getInstance()->getView() == 0) {
		Config::getInstance()->setView(1);
		m->setName("view: locked");
	} else {
		Config::getInstance()->setView(0);
		m->setName("view: follows ball");
	}
}
*/

MenuItem* createMenus(Engine * engine) {
	// Load a font
	EmFont* font = EmFont::getInstance();
	// TODO the font is loaded twice once in score once in meny
	//	string filename = Config::getInstance()->getDataDir() + string("/font_34.png");
	//	font->loadFont(filename.c_str());
	
	// Create the meny
	MenuSub* menu = new MenuSub("menu", engine);

	MenuSub* menuresume = new MenuSub("play", engine);
	menuresume->setAction(EM_MENU_RESUME);
	menu->addMenuItem(menuresume);

	MenuSub* menuload = new MenuSub("playfield", engine);
	menu->addMenuItem(menuload);

	MenuSub* menucfg = new MenuSub("config", engine);
	menu->addMenuItem(menucfg);

	MenuSub* menuexit = new MenuSub("exit", engine);
	menuexit->setAction(EM_MENU_EXIT);
	menu->addMenuItem(menuexit);

	MenuSub* menugfx = new MenuSub("graphics", engine);
	menucfg->addMenuItem(menugfx);

	// create one entry for each directory
	// TODO scrolling text
	DIR * datadir = opendir(Config::getInstance()->getDataDir());
	char cwd[256];
	if (datadir != NULL && getcwd(cwd, 256) != NULL) {
		struct dirent * entry;
		struct stat statbuf;
		chdir(Config::getInstance()->getDataDir());
		while ((entry = readdir(datadir)) != NULL) {
			lstat(entry->d_name, &statbuf);
			if (S_ISDIR(statbuf.st_mode) &&
					strcmp(".", entry->d_name) != 0 &&
					strcmp("..", entry->d_name) != 0) {
				MenuFct* menufct = new MenuFct(entry->d_name, fctCancel, engine);
				menuload->addMenuItem(menufct);
			}
		}
		chdir(cwd);
		closedir(datadir);
	}

	menuscreen = new MenuChoose(engine);
	menuscreen->addText(   "screen:    fullscreen");
	menuscreen->addText(   "screen:      windowed");
	menugfx->addMenuItem(menuscreen);

	menubright = new MenuChoose(engine);
	menubright->addText("brightness:       low");
	menubright->addText("brightness:    medium");
	menubright->addText("brightness:      high");
	menugfx->addMenuItem(menubright);

	menusize = new MenuChoose(engine);
	menusize->addText(  "screen size:  340x240");
	menusize->addText(  "screen size:  400x300");
	menusize->addText(  "screen size:  512x384");
	menusize->addText(  "screen size:  640x480");
	menusize->addText(  "screen size:  800x600");
	menusize->addText(  "screen size: 1024x768");
	menugfx->addMenuItem(menusize);

	menufilter = new MenuChoose(engine);
 	menufilter->addText("texture:       linear");
	menufilter->addText("texture:      nearest");
	menufilter->addText("texture:         none");
	menugfx->addMenuItem(menufilter);

	menuview = new MenuChoose(engine);
	menuview->addText(  "view:    follows ball");
	menuview->addText(  "view:          locked");
	menucfg->addMenuItem(menuview);

	menusnd = new MenuChoose(engine);
	menusnd->addText(   "sound:             on");
	menusnd->addText(   "sound:            off");
	menucfg->addMenuItem(menusnd);

	MenuFct* menuapply = new MenuFct("apply", fctApply, engine);
	menucfg->addMenuItem(menuapply);
	menugfx->addMenuItem(menuapply);

	MenuFct* menucancel = new MenuFct("back", fctCancel, engine);
	menucfg->addMenuItem(menucancel);
	menugfx->addMenuItem(menucancel);
	menuload->addMenuItem(menucancel);

	get_config();
	return menu;
}
