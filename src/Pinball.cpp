/***************************************************************************
                          Pinball.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <fstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "Private.h"
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
#include "EyeBehavior.h"
#include "Config.h"
#include "EmFont.h"
#include "Loader.h"

#if EM_USE_SDL
#include <SDL.h>
#endif

/****************************************************************************
 * Creates a ball 
 ***************************************************************************/
Group * createBall(float r, float g, float b, int pbl, float x, Engine * engine) {
	Group * group = new Group();
  /*Shape3D* ballCyl = new BigSphere(1, 1, r, g, b, 1);*/
 	//string filename;
	//string datadir(Config::getInstance()->getDataDir());
	//filename = datadir + string("/ball_co.emi");
	//	sprintf(filename, "%s/ball_co.emi", Config::getInstance()->getDataDir());
  //Shape3D* ballCyl = Shape3DUtil::loadShape3D(filename.c_str());
  Shape3D* ballSphere = new BigSphere(1, 2, r, g, b, 1);
  ballSphere->setProperty(EM_SHAPE3D_SPECULAR);
  //ballCyl->setProperty(EM_SHAPE3D_HIDDEN);
  //CollisionBounds* ballBounds = new CollisionBounds(ballCyl->getCollisionSize());
  //ballBounds->setShape3D(ballCyl, 0); 
  CollisionBounds* ballBounds = new CollisionBounds(1.0f/EM_SQRT_3);

	// create a transparent black polygon in the bottom of the ball - a shadow
  Shape3D* shadow = new Shape3D(8, 1);

  shadow->add(0, -1, 1,
							0.0f, 0.0f, 0.0f, 0.5f,  0.0f, 0.0f);
  shadow->add(-0.707,-1, 0.707,
							0.0f, 0.0f, 0.0f, 0.5f,  0.0f, 0.0f);
  shadow->add(-1, -1, 0,
							0.0f, 0.0f, 0.0f, 0.5f,  0.0f, 0.0f);
  shadow->add(-0.707,-1, -0.707,
							0.0f, 0.0f, 0.0f, 0.5f,  0.0f, 0.0f);
  shadow->add(0, -1, -1,
							0.0f, 0.0f, 0.0f, 0.5f,  0.0f, 0.0f);
  shadow->add(0.707, -1, -0.707,
							0.0f, 0.0f, 0.0f, 0.5f,  0.0f, 0.0f);
	shadow->add(1, -1, 0,
							0.0f, 0.0f, 0.0f, 0.5f,  0.0f, 0.0f);
	shadow->add(0.707, -1, 0.707,
							0.0f, 0.0f, 0.0f, 0.5f,  0.0f, 0.0f);

  Polygon* poly = new Polygon(shadow, 3);
  poly->add(0);
  poly->add(1);
  poly->add(2);
  poly->add(3);
  poly->add(4);
  poly->add(5);
  poly->add(6);
  poly->add(7);
  //poly->setProperty(EM_POLY_TRANS);
  shadow->add(poly);
  //shadow->setProperty(EM_SHAPE3D_USE_TRANS);
  shadow->countNormals();

  BounceBehavior* bouBeh = new BounceBehavior(pbl);

  engine->add(group);
  group->setUserProperty(pbl);
  group->setCollisionBounds(ballBounds);
  //group->addShape3D(ballCyl);
  group->addShape3D(ballSphere);
  group->addShape3D(shadow);
  group->setBehavior(bouBeh);
  group->setTransform(x, 0, 8, 0, 0, 0);

	return group;
}

Score * score = NULL;

/****************************************************************************
 * Table loading
 ***************************************************************************/
int loadLevel(Engine * engine, const char * subdir) {
	score = NULL;
	// Clear old engine objects
	engine->clear();
	// Load from file
	Config::getInstance()->setSubDir(subdir);
	string datadir(Config::getInstance()->getDataSubDir());
	string filename = datadir + string("/pinball.pbl");
	Loader::getInstance()->clearSignalVariable();
	if (Loader::getInstance()->loadFile(filename.c_str(), engine) != 0) {
		cerr << "Error loading level" << endl;
		cerr << "Try reinstalling the game or use the -data switch to specify the data directory" 
				 << endl;
		engine->clear();
		return -1;
	}
	score = Score::getInstance();
	engine->setBehavior(score);
	// Add a camera.
	Group* groupCR = new Group();
	Group* groupCT = new Group();
	Camera* camera = new Camera();
	//	KeyRotBehavior* keyRBeh = new KeyRotBehavior();
	//	KeyBehavior* keyBeh = new KeyBehavior();
	engine->add(groupCT);
	// 	groupCT->setBehavior(keyBeh);
	//	groupCT->setTransform(0, 25, 25, 0, 0, 0);
	groupCT->add(groupCR);
	// 	groupCR->setBehavior(keyRBeh);
	//	groupCR->setTransform(0.0f, 0.0f, 0.0f, 0.175f, 0, 0);
	groupCR->setCamera(camera);

	Light* cl = new Light(1, 0.05f, 0, 1,1,1);
	groupCR->setLight(cl);
	engine->addLight(cl);

	engine->setEngineCamera(groupCR);
	/*
	// Add pinball floor
	Group* groupG = new Group();
	#if EM_USE_ALLEGRO
	filename = datadir + string("/floor2.pcx");
	#endif
	#if EM_USE_SDL
	filename = datadir + string("/floor2.png");
	#endif
	EmTexture* tex = TextureUtil::getInstance()->loadTexture(filename.c_str());
	Shape3D* gs = new Grid(tex, 78.0f, 43.0f, 16, 1.0f/16.0f, 1, 1, 1, 1);
	gs->setProperty(EM_SHAPE3D_BEHIND2);
	engine->add(groupG);
	groupG->addShape3D(gs);
	groupG->setTransform(1.5f, -1.0f, -3.0f, 0.0f, 0.25f, 0.0f);
	groupG->setProperty(EM_GROUP_TRANSFORM_ONCE);
	*/

	Group* gb1 = createBall(1, 0, 0, PBL_BALL_1, 4, engine);
	Group* gb2 = createBall(0, 1, 0, PBL_BALL_2, 0, engine);
	Group* gb3 = createBall(0, 0, 1, PBL_BALL_3, -4, engine);

	EyeBehavior* eyebeh = new EyeBehavior(gb1, gb2, gb3);
	filename = datadir + string("/nudge.wav");
	eyebeh->setSound(SoundUtil::getInstance()->loadSample(filename.c_str()));
	groupCT->setBehavior(eyebeh);

	// Reset pinball
	SendSignal(PBL_SIG_RESET_ALL, 0, engine, NULL);

	return 0;
}

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
MenuChoose* menumaxfps = NULL;

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
	switch(Config::getInstance()->getView()) {
	case 1: menuview->setCurrent(1);
	case 2: menuview->setCurrent(2);
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
	//
	if (Config::getInstance()->getShowFPS()) {
		menufps->setCurrent(1);
	} else {
		menufps->setCurrent(0);
	}
	// max fps
	if (Config::getInstance()->getMaxFPS() == 40) {
		menumaxfps->setCurrent(0);
	} else {
		menumaxfps->setCurrent(1);
	}
}

/** The apply meny item calls this function */	
class MyMenuApply : public MenuFct {
public:
	MyMenuApply(const char * name, int (*fct)(void), Engine *e) : 	MenuFct(name, fct, e) {};
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
		config->setSize(w, h);
		
		switch (menuview->getCurrent()) {
		case 1: config->setView(1);
		case 2: config->setView(2);
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
		// max fps
		if (menumaxfps->getCurrent() == 0) {
			config->setMaxFPS(40); 
		} else {
			config->setMaxFPS(80);
		}
		
		get_config();
		return EM_MENU_NOP;
	}
};

class MyMenuCancel : public MenuFct {
public:
	MyMenuCancel(const char * name, int (*fct)(void), Engine *e) : MenuFct(name, fct, e) {};
protected:
	int perform () {
		get_config();
		return EM_MENU_BACK;
	}
};

class MyMenuLoad : public MenuFct {
public:
	MyMenuLoad(const char * name, int (*fct)(void), Engine *e) : MenuFct(name, fct, e) {
		
	};
protected:
	int perform () {
		p_Engine->clearScreen();
		p_EmFont->printRowCenter("LOADING", 10);
		p_Engine->swap();

		if (loadLevel(p_Engine, m_Name) == 0) {
			p_Engine->clearScreen();
			p_EmFont->printRowCenter("OK", 10);
			p_Engine->swap();
			p_Engine->delay(500);
		} else {
			p_Engine->clearScreen();
			p_EmFont->printRowCenter("ERROR", 10);
			p_Engine->swap();
			p_Engine->delay(1000);
		}
		return EM_MENU_BACK;
	}
};

/* Create some menus */
MenuItem* createMenus(Engine * engine) {
	// Create the meny
	MenuSub* menu = new MenuSub("menu", engine);

	MenuSub* menuresume = new MenuSub("play", engine);
	menuresume->setAction(EM_MENU_RESUME);
	menu->addMenuItem(menuresume);

	MenuSub* menuload = new MenuSub("load table", engine);
	menu->addMenuItem(menuload);

	MenuSub* menucfg = new MenuSub("config", engine);
	menu->addMenuItem(menucfg);

	MenuSub* menuexit = new MenuSub("exit", engine);
	menuexit->setAction(EM_MENU_EXIT);
	menu->addMenuItem(menuexit);

	MenuSub* menugfx = new MenuSub("graphics", engine);
	menucfg->addMenuItem(menugfx);

	MenuSub* menuaudio = new MenuSub("audio", engine);
	menucfg->addMenuItem(menuaudio);

	// create one entry for each directory
	// TODO scrolling text if to many tables
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
				MenuFct* menufct = new MyMenuLoad(entry->d_name, NULL, engine);
				menuload->addMenuItem(menufct);
			}
		}
		chdir(cwd);
		closedir(datadir);
	}

	menuview = new MenuChoose(engine);
	menuview->addText(  "view:             top");
	menuview->addText(  "view:        standard");
	menuview->addText(  "view:          locked");
	menugfx->addMenuItem(menuview);

	menuscreen = new MenuChoose(engine);
	menuscreen->addText("screen:    fullscreen");
	menuscreen->addText("screen:      windowed");
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

	menufps = new MenuChoose(engine);
	menufps->addText(   "show fps:          no");
	menufps->addText(   "show fps:         yes");
	menugfx->addMenuItem(menufps);

	menumaxfps = new MenuChoose(engine);
	menumaxfps->addText("max fps            40");
	menumaxfps->addText("max fps            80");
	menugfx->addMenuItem(menumaxfps);

	menusnd = new MenuChoose(engine);
	menusnd->addText(   "sound:            off");
	menusnd->addText(   "sound:       o.......");
	menusnd->addText(   "sound:       oo......");
	menusnd->addText(   "sound:       ooo.....");
	menusnd->addText(   "sound:       oooo....");
	menusnd->addText(   "sound:       ooooo...");
	menusnd->addText(   "sound:       oooooo..");
	menusnd->addText(   "sound:       ooooooo.");
	menusnd->addText(   "sound:       oooooooo");
	menuaudio->addMenuItem(menusnd);

	menumusic = new MenuChoose(engine);
	menumusic->addText( "music:            off");
	menumusic->addText( "music:       o.......");
	menumusic->addText( "music:       oo......");
	menumusic->addText( "music:       ooo.....");
	menumusic->addText( "music:       oooo....");
	menumusic->addText( "music:       ooooo...");
	menumusic->addText( "music:       oooooo..");
	menumusic->addText( "music:       ooooooo.");
	menumusic->addText( "music:       oooooooo");
	menuaudio->addMenuItem(menumusic);

	MenuFct* menuapply = new MyMenuApply("apply", NULL, engine);
	menuaudio->addMenuItem(menuapply);
	//menucfg->addMenuItem(menuapply);
	menugfx->addMenuItem(menuapply);

	MenuFct* menucancel = new MyMenuCancel("back", NULL, engine);
	menucfg->addMenuItem(menucancel);
	menugfx->addMenuItem(menucancel);
	menuaudio->addMenuItem(menucancel);
	menuload->addMenuItem(menucancel);

	get_config();
	return menu;
}


/** Main */
int main(int argc, char *argv[]) {
	cout << "pinball" << endl;
	//	srandom(time(0));

	// Create a engine and parse emilia arguments
	Config::getInstance()->loadConfig();
	Engine * engine = new Engine(argc, argv);

	float direct, ambient;
	if (Config::getInstance()->useLights()) {
		direct = 0.0f;
	} else {
		direct = 0.5f;
	}
	if (Config::getInstance()->getBrightness() < 0.09f) {
		ambient = 0.0f;
	} else if (Config::getInstance()->getBrightness() > 0.11f) {
		ambient = 0.2f;
	} else {
		ambient = 0.1f;
	}
	engine->setLightning(direct, ambient);

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
	bool render = true;
	int skip = 0;
	int all = 0;
	while (!Keyboard::isKeyDown(SDLK_INSERT)) {
		if (Keyboard::isKeyDown(SDLK_ESCAPE) || all == 0) {
			SoundUtil::getInstance()->pauseMusic();
			if (menu->perform() == EM_MENU_EXIT) {
				break;
			}
			SoundUtil::getInstance()->resumeMusic();
		}
#if EM_DEBUG
		if (Keyboard::isKeyDown(SDLK_p)) {
			Keyboard::waitForKey();
			Keyboard::clear();
		}
#endif
		if (Keyboard::isKeyDown(SDLK_r)) {
			SendSignal(PBL_SIG_RESET_ALL, 0, engine, NULL);
		}

		engine->tick();
		engine->tick();
		if (Config::getInstance()->getMaxFPS() == 40) {
			engine->tick();
			engine->tick();
		}

		if (Config::getInstance()->getMaxFPS() == 40) {
			render = engine->limitFPS(40);
		} else {	
			render = engine->limitFPS(80);
		}

		if (render) {
			engine->render();
			if (score != NULL) {
				score->draw();
			}
			if (engine->getGroup(0) == NULL) {
				EmFont::getInstance()->printRowCenter("no table loaded", 6);
				EmFont::getInstance()->printRowCenter("press esc", 8);
			}
			engine->swap();
		} else {
			skip++;
		}
		all++;
		//engine->limitFPS(100);
	}

#if EM_DEBUG
	cerr << "Skip " << skip  << " of " << all << endl;
#endif

	Config::getInstance()->saveConfig();

	delete(engine);
	return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
