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

void fctSound(Meny * m) {
  if (Config::getInstance()->useSound()) {
		Config::getInstance()->setSound(false);
		m->setName("sound off");
	} else {
		Config::getInstance()->setSound(true);
		m->setName("sound on");
	}
	EM_COUT("sound", 1);
}

void fctGrx(Meny * m) {
  if (Config::getInstance()->useFullScreen()) {
		Config::getInstance()->setFullScreen(false);
		m->setName("graphics windowed");
		SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
	} else {
		Config::getInstance()->setFullScreen(true);
		m->setName("graphics fullscreen");
		SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
	}
	EM_COUT("graphics", 1);
}	

void fctTexture(Meny * m) {
  if (Config::getInstance()->getGLFilter() == GL_LINEAR) {
		Config::getInstance()->setGLFilter(GL_NEAREST);
		m->setName("texture filter nearest");
	} else {
		Config::getInstance()->setGLFilter(GL_LINEAR);
		m->setName("texture filter linear");
	}
	EM_COUT("texture", 1);
}	

Meny* createMenus(Engine * engine) {
	char filename[256];
	// Load a font
	EmFont* font = EmFont::getInstance();
	// TODO the font is loaded twice once in score once in meny
	sprintf(filename, "%s/font_16.png", Config::getInstance()->getDataDir());
	font->loadFont(filename);
	
	// Create the meny
	Meny* meny = new Meny("meny", engine);

	Meny* menyresume = new Meny("resume", engine);
	menyresume->setAction(EM_MENU_RESUME);
	meny->addMeny(menyresume);

	Meny* menycfg = new Meny("config", engine);
	menycfg->setAction(EM_MENU_SUB);
	meny->addMeny(menycfg);

	Meny* menyexit = new Meny("exit", engine);
	menyexit->setAction(EM_MENU_EXIT);
	meny->addMeny(menyexit);

	Meny* menysnd = new Meny("sound off", engine);
	if (Config::getInstance()->useSound()) {
		menysnd->setName("sound on");
	}
	menysnd->setAction(EM_MENU_SUB);
	menysnd->setFunction(fctSound);
	menycfg->addMeny(menysnd);

	Meny* menygrx = new Meny("graphics windowed", engine);
	if (Config::getInstance()->useFullScreen()) {
		menygrx->setName("graphics fullscreen");
	}
	menygrx->setAction(EM_MENU_SUB);
	menygrx->setFunction(fctGrx);
	menycfg->addMeny(menygrx);

	Meny* menytex = new Meny("texture filter nearest", engine);
	if (Config::getInstance()->getGLFilter() == GL_LINEAR) {
		menytex->setName("texture filter linear");
	}
	menytex->setAction(EM_MENU_SUB);
	menytex->setFunction(fctTexture);
	menycfg->addMeny(menytex);

	Meny* menyctrl = new Meny("controls", engine);
	menyctrl->setAction(EM_MENU_SUB);
	menycfg->addMeny(menyctrl);

	Meny* menyback = new Meny("back", engine);
	menyback->setAction(EM_MENU_BACK);
	menycfg->addMeny(menyback);

	return meny;
}

