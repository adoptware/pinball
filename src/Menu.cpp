/***************************************************************************
                          Menu.cpp  -  description
                             -------------------
    begin                : Wed Aug 22 2001
    copyright            : (C) 2001 by Henrik Enqvist IB
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Meny.h"
#include "EmFont.h"
#include "Config.h"
#include "Engine.h"

MenuChoose* menusnd = NULL;
MenuChoose* menugrx = NULL;
MenuChoose* menusize = NULL;
MenuChoose* menuview = NULL;
MenuChoose* menufilter = NULL;

void get_config(void) {
	if (Config::getInstance()->useSound()) {
		menusnd->setCurrent(0);
	} else {
		menusnd->setCurrent(1);
	}

	if (Config::getInstance()->useFullScreen()) {
		menugrx->setCurrent(0);
	} else {
		menugrx->setCurrent(1);
	}

	if (Config::getInstance()->getWidth() == 800) {
		menusize->setCurrent(1);
	} else 	if (Config::getInstance()->getWidth() == 1024) {
		menusize->setCurrent(2);
	} else {
		menusize->setCurrent(0);
	}

	if (Config::getInstance()->getView() == 0) {
		menuview->setCurrent(0);
	} else {
		menuview->setCurrent(1);
	}

	if (Config::getInstance()->getGLFilter() == GL_LINEAR) {
		menufilter->setCurrent(0);
	} else if (Config::getInstance()->getGLFilter() == GL_NEAREST) {
		menufilter->setCurrent(1);
	} else {
		menufilter->setCurrent(2);
	}
}
	
int fctApply(void) {
	Config* config = Config::getInstance();

	if (menusnd->getCurrent() == 0) {
		config->setSound(true);
	} else {
		config->setSound(false);
	}

	if (menugrx->getCurrent() == 0) {
		if (config->useFullScreen() == false) {
			SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
		}
		config->setFullScreen(true);
	} else {
		if (config->useFullScreen() == true) {
			SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
		}
		config->setFullScreen(false);
	}

	if (menusize->getCurrent() == 0) {
		if (config->getWidth() != 640) {
			SDL_SetVideoMode(640, 480, config->getBpp(), 
											 SDL_OPENGL | (config->useFullScreen() ? SDL_FULLSCREEN : 0));
		}
		config->setSize(640, 480);
	} else if (menusize->getCurrent() == 1) {
		if (config->getWidth() != 800) {
			SDL_SetVideoMode(800, 600, config->getBpp(), 
											 SDL_OPENGL | (config->useFullScreen() ? SDL_FULLSCREEN : 0));
		}
		config->setSize(800, 600);
	} else {
		if (config->getWidth() != 1024) {
			SDL_SetVideoMode(1024, 768, config->getBpp(), 
											 SDL_OPENGL | (config->useFullScreen() ? SDL_FULLSCREEN : 0));
		}
		config->setSize(1024, 768);
	}

	if (menuview->getCurrent() == 0) {
		config->setView(0);
	} else {
		config->setView(1);
	}

	if (menufilter->getCurrent() == 0) {
		config->setGLFilter(GL_LINEAR);
	} else if (menufilter->getCurrent() == 1) {
		config->setGLFilter(GL_NEAREST);
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
	char filename[256];
	// Load a font
	EmFont* font = EmFont::getInstance();
	// TODO the font is loaded twice once in score once in meny
	sprintf(filename, "%s/font_34.png", Config::getInstance()->getDataDir());
	font->loadFont(filename);
	
	// Create the meny
	MenuSub* menu = new MenuSub("menu", engine);

	MenuSub* menuresume = new MenuSub("resume", engine);
	menuresume->setAction(EM_MENU_RESUME);
	menu->addMenuItem(menuresume);

	MenuSub* menucfg = new MenuSub("config", engine);
	menu->addMenuItem(menucfg);

	MenuSub* menuexit = new MenuSub("exit", engine);
	menuexit->setAction(EM_MENU_EXIT);
	menu->addMenuItem(menuexit);

	menusnd = new MenuChoose(engine);
	menusnd->addText(   "sound:             on");
	menusnd->addText(   "sound:            off");
	menucfg->addMenuItem(menusnd);

	menugrx = new MenuChoose(engine);
	menugrx->addText(   "graphics:  fullscreen");
	menugrx->addText(   "graphics:    windowed");
	menucfg->addMenuItem(menugrx);

	menusize = new MenuChoose(engine);
	menusize->addText(  "screen size:  640x480");
	menusize->addText(  "screen size:  800x600");
	menusize->addText(  "screen size: 1024x768");
	menucfg->addMenuItem(menusize);

	menuview = new MenuChoose(engine);
	menuview->addText(  "view:    follows ball");
	menuview->addText(  "view:          locked");
	menucfg->addMenuItem(menuview);

	menufilter = new MenuChoose(engine);
	menufilter->addText("texture:       linear");
	menufilter->addText("texture:      nearest");
	menufilter->addText("texture:         none");
	menucfg->addMenuItem(menufilter);

	MenuFct* menuapply = new MenuFct("apply", fctApply, engine);
	menucfg->addMenuItem(menuapply);

	MenuFct* menucancel = new MenuFct("back", fctCancel, engine);
	menucfg->addMenuItem(menucancel);

	get_config();
	return menu;
}

