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

void sndFunc(Meny * m) {
  if (Config::getInstance()->useSound()) {
		Config::getInstance()->setSound(false);
		m->setName("sound off");
	} else {
		Config::getInstance()->setSound(true);
		m->setName("sound on");
	}

	cerr << "sound" << endl;
}

void grxFunc(Meny * m) {
  if (Config::getInstance()->useFullScreen()) {
		Config::getInstance()->setFullScreen(false);
		m->setName("graphics windowed");
	} else {
		Config::getInstance()->setFullScreen(true);
		m->setName("graphics fullscreen");
	}

	cerr << "graphics" << endl;
}	

Meny*  createMenus(Engine * engine) {
	
	// Load a font
	EmFont* font = EmFont::getInstance();
	// TODO the font is loaded twice once in score once in meny
	font->loadFont("data/font_16.png");
	
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
	menysnd->setFunction(sndFunc);
	menycfg->addMeny(menysnd);

	Meny* menygrx = new Meny("graphics windowed", engine);
	if (Config::getInstance()->useFullScreen()) {
		menygrx->setName("graphics fullscreen");
	}
	menygrx->setAction(EM_MENU_SUB);
	menygrx->setFunction(grxFunc);
	menycfg->addMeny(menygrx);

	Meny* menyctrl = new Meny("controls", engine);
	menyctrl->setAction(EM_MENU_SUB);
	menycfg->addMeny(menyctrl);

	Meny* menyback = new Meny("back", engine);
	menyback->setAction(EM_MENU_BACK);
	menycfg->addMeny(menyback);

	return meny;
}

