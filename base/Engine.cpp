/***************************************************************************
                          Engine.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Private.h"

#include "Engine.h"
#include "Camera.h"
#include "Keyboard.h"
#include "AlignVisitor.h"
#include "AmbientLightVisitor.h"
#include "BehaviorVisitor.h"
#include "CollisionVisitor.h"
#include "OpenGLVisitor.h"
#include "OpenGLTransVisitor.h"
#include "PointLightVisitor.h"
#include "PNormalVisitor.h"
#include "SoundVisitor.h"
#include "SignalSender.h"
#include "TransformVisitor.h"
#include "Config.h"

#define COUNT 1
#define ALIGN 2
#define BEH 3
#define GLIGHT 4
#define COLLISION 5
#define PLIGHT 6
#define PNORMAL 7
#define RENDER 8
#define SOUND 9
#define SIG 10
#define TRANS 11
#define DRAW 13
#define SWAP 14
#define KEY 15
#define RENDER_OUT 16
#define TICK_OUT 17
#define SWAP_OUT 18

#define FPS 50 // req 100 50 33 25 20

volatile int g_iStartTime = -1;
volatile int g_iDesiredTime = -1;

volatile int g_iLoops = 0;
volatile int g_iSeconds = 0;
volatile float g_fFps = 0;

volatile int g_aFunction[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile int g_iCurrentFct = 0;

#if EM_DEBUG
Uint32 fctCallBack(Uint32 interval) {
	g_iSeconds++;
  g_aFunction[g_iCurrentFct]++;
	if (g_iSeconds == 50) {
		g_fFps = (float)g_iLoops*10.0f / ((float)g_iSeconds);
		g_iSeconds = 0;
		g_iLoops = 0;
		cerr << g_fFps << endl;
	}
  return interval;	
}
#endif // EM_DEBUG

Engine::Engine(int & argc, char *argv[]) {
	m_Background = NULL;

	Config * config = Config::getInstance();
	config->loadArgs(argc, argv);

	if (!config->useExternGL()) {
		this->initGrx();
#if EM_DEBUG
		SDL_SetTimer(100, fctCallBack); // 10 ticks per sec
#endif
	}
	if (config->useSound()) {
		this->initSound();
	}

  srand((unsigned int)time((time_t *)NULL));
}

Engine::~Engine() {
	SDL_Quit();

#if EM_DEBUG	
	cerr << "Function null " << g_aFunction[0] << endl;
	cerr << "Function align " << g_aFunction[ALIGN] << endl;
	cerr << "Function beh " << g_aFunction[BEH] << endl;
	cerr << "Function collision " << g_aFunction[COLLISION] << endl;
	cerr << "Function draw " << g_aFunction[DRAW] << endl;
	cerr << "Function glight " << g_aFunction[GLIGHT] << endl;
	cerr << "Function plight " << g_aFunction[PLIGHT] << endl;
	cerr << "Function pnormal " << g_aFunction[PNORMAL] << endl;
	cerr << "Function render " << g_aFunction[RENDER] << endl;
	cerr << "Function sound " << g_aFunction[SOUND] << endl;
	cerr << "Function signal " << g_aFunction[SIG] << endl;
	cerr << "Function trans " << g_aFunction[TRANS] << endl;
	cerr << "Function swap " << g_aFunction[SWAP] << endl;
	cerr << "Function keyboard poll " << g_aFunction[KEY] << endl;
	cerr << "Function render out " << g_aFunction[RENDER_OUT] << endl;
	cerr << "Function swap out " << g_aFunction[SWAP_OUT] << endl;
	cerr << "Function tick out " << g_aFunction[TICK_OUT] << endl;
	//	cerr << "Seconds " << ((float)g_iSeconds/FPS) <<" " << ((float)g_iLoops*FPS/g_iSeconds) 
	//			 << " fps" << endl;
	// cerr << "Fps " << (float)(g_iLoops)*1000 / (SDL_GetTicks()-g_iStartTime) << endl;
#endif
}


void Engine::initSound() {
	SDL_AudioSpec wanted, obtained;
	extern void fctFillAudio(void *udata, Uint8 *stream, int len);
	
	/* Set the audio format */
	wanted.freq = 22050;
	wanted.format = AUDIO_S16;
	wanted.channels = 2;    /* 1 = mono, 2 = stereo */
	wanted.samples = 512;  /* Good low-latency value for callback */
	wanted.callback = fctFillAudio;
	wanted.userdata = NULL;
	
	/* Open the audio device, forcing the desired format */
	if ( SDL_OpenAudio(&wanted, &obtained) < 0 ) {
		cerr << "Couldn't open audio: " << SDL_GetError() << endl;
		cerr << "Make sure that no other application has occupied audio resources."  << endl;
		cerr << "If your're running KDE you might try to kill the 'artsd' process." << endl;
		cerr << "Or run pinball with the '-nosound' switch." << endl;
		exit(1);
	}
	
	cerr << endl;
	cerr << "Freq (requested " << wanted.freq         << "hz) : " << obtained.freq << endl;
	cerr << "Format (requested " << wanted.format     << ") : " << obtained.format << endl;
	cerr << "Channels (requested " << wanted.channels << ") : " << obtained.channels << endl;
	cerr << "Samples (requested  " << wanted.samples  << ") : " << obtained.samples << endl;
}

void Engine::initGrx() {
	Config* config = Config::getInstance();

	cerr << "Initing SDL" << endl;
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0 ) {
		cerr << "Couldn't initialize SDL: " <<  SDL_GetError() << endl;
		exit(1);
	}

	// See if we should detect the display depth 
	if ( SDL_GetVideoInfo()->vfmt->BitsPerPixel <= 8 ) {
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 2 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 3 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 3 );
	} else 	if ( SDL_GetVideoInfo()->vfmt->BitsPerPixel <= 16 ) {
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	}	else {
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	}

	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	/* Initialize the display */
	SDL_Surface* screen = SDL_SetVideoMode(config->getWidth(), config->getHeight(), config->getBpp(), 
																				 SDL_OPENGL | (config->useFullScreen() ? SDL_FULLSCREEN : 0));

	if (config->useFullScreen()) {
		SDL_ShowCursor(SDL_DISABLE);
	}
	SDL_WM_SetCaption("Emilia Pinball", NULL);

	if (screen == NULL) {
		cerr << "Couldn't set video mode: " << SDL_GetError() << endl;
		exit(1);
	}

	cerr << endl;
	cerr << "Vendor     : " << glGetString( GL_VENDOR ) << endl;
	cerr << "Renderer   : " << glGetString( GL_RENDERER ) << endl;
	cerr << "Version    : " << glGetString( GL_VERSION ) << endl;
	cerr << "Extensions : " << glGetString( GL_EXTENSIONS ) << endl;
	cerr << endl;

	int value;
	SDL_GL_GetAttribute( SDL_GL_RED_SIZE, &value );
	cerr << "SDL_GL_RED_SIZE: " << value << endl;
	SDL_GL_GetAttribute( SDL_GL_GREEN_SIZE, &value );
	cerr << "SDL_GL_GREEN_SIZE: " << value << endl;
	SDL_GL_GetAttribute( SDL_GL_BLUE_SIZE, &value );
	cerr << "SDL_GL_BLUE_SIZE: " << value << endl;
	SDL_GL_GetAttribute( SDL_GL_DEPTH_SIZE, &value );
	cerr << "SDL_GL_DEPTH_SIZE: " << value << endl;
	SDL_GL_GetAttribute( SDL_GL_DOUBLEBUFFER, &value );
	cerr << "SDL_GL_DOUBLEBUFFER: " << value << endl;
	
	this->resize( config->getWidth(), config->getHeight() );
}

void Engine::resize( unsigned int w, unsigned int h ) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);

	glShadeModel(GL_SMOOTH);

 	glEnable(GL_CULL_FACE);
 	glCullFace(GL_BACK);
 	glFrontFace(GL_CW);
 	
 	glEnable(GL_DEPTH_TEST);
 	glDepthFunc(GL_LESS);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 200);
	glMatrixMode(GL_MODELVIEW);

#if OPENGL_LIGHTS 
	glEnable(GL_LIGHTING);
#endif
}


void Engine::addLight(Light* l) {
	PointLightVisitor::getInstance()->add(l);
}

void Engine::clearScreen() {
	g_iCurrentFct = DRAW;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glLoadIdentity();
  gluLookAt(0,0,0, 0,0,-1, 0,1,0);
}

void Engine::setLightning(float diffuse, float ambient) {
 	AmbientLightVisitor::getInstance()->setLightning(diffuse, ambient);
	// light model
}

void Engine::setBackground(EmImage * bg) {
	m_Background = bg;
}

void Engine::setEngineCamera(Group* g) {
	AlignVisitor::getInstance()->setCamera(g);
	TransformVisitor::getInstance()->setCamera(g);
}

void Engine::render() {
	EM_COUT("Engine::render()", 0);
	// Put some overall light.
	EM_COUT("Engine::render() glight", 0);
	g_iCurrentFct = GLIGHT;
	AmbientLightVisitor::getInstance()->empty();
	this->accept(AmbientLightVisitor::getInstance());

	// Put some light from light sources.
	EM_COUT("Engine::render() plight", 0);
	g_iCurrentFct = PLIGHT;
	PointLightVisitor::getInstance()->empty();
	this->accept(PointLightVisitor::getInstance());

	// Align vertices to view space.
	EM_COUT("Engine::render() align", 0);
	g_iCurrentFct = ALIGN;
	AlignVisitor::getInstance()->empty();
	this->accept(AlignVisitor::getInstance());

	// Adjust sounds.
	EM_COUT("Engine::render() sound", 0);
	g_iCurrentFct = SOUND;
//	p_SoundVisitor->empty();
//	this->accept(p_SoundVisitor);

	// Draw screen
	EM_COUT("Engine::render() opengl", 0);
	g_iCurrentFct = RENDER;
	this->clearScreen();
	OpenGLVisitor::getInstance()->empty();
	this->accept(OpenGLVisitor::getInstance());

	EM_COUT("Engine::render() opengltrans", 0);
	OpenGLTransVisitor::getInstance()->empty();
	this->accept(OpenGLTransVisitor::getInstance());

	g_iCurrentFct = RENDER_OUT;
}

void Engine::swap() {
	EM_COUT("Engine::swap()", 0);
	g_iCurrentFct = SWAP;
	g_iLoops++;
	// Draw to screen.
	SDL_GL_SwapBuffers();

	EM_GLERROR(" In Engine::swap ");
	g_iCurrentFct = SWAP_OUT;
}

void Engine::tick() {
	EM_COUT("Engine::tick()", 0);
	g_iCurrentFct = KEY;
	// Check keyboard.
	Keyboard::poll();

	// Perform signals.
	EM_COUT("Engine::tick() signal", 0);
	g_iCurrentFct = SIG;
	SignalSender::getInstance()->tick();

	// Perform behaviors.
	EM_COUT("Engine::tick() behavior", 0);
	g_iCurrentFct = BEH;
	BehaviorVisitor::getInstance()->empty();
	this->accept(BehaviorVisitor::getInstance());

	// Calculate positions
	EM_COUT("Engine::tick() trans", 0);
	g_iCurrentFct = TRANS;
	TransformVisitor::getInstance()->empty();
	this->accept(TransformVisitor::getInstance());

	// Detect collision.
	EM_COUT("Engine::tick() collision", 0);
	g_iCurrentFct = COLLISION;
	CollisionVisitor::getInstance()->empty();
	this->accept(CollisionVisitor::getInstance());

	/*
	// Calculate positions one more time, collision may have changed the positions.
	EM_COUT("Engine::tick() trans" << endl, 0);
	g_iCurrentFct = TRANS;
	p_TransformVisitor->empty();
	this->accept(p_TransformVisitor);
	*/
	g_iCurrentFct = TICK_OUT;
}

/* ATTENTION! This function wraps after ~49 days */
bool Engine::limitFPS(int fps) {
	int delay = 0;
	if (fps > 0) {
		delay = 1000/fps;
	}
	if (g_iStartTime == -1) {
		g_iDesiredTime = g_iStartTime = SDL_GetTicks();
	}
	g_iDesiredTime += delay;
	int time = SDL_GetTicks();
	int realdelay = (g_iDesiredTime - time);
	if (realdelay < -500) {
		EM_COUT("TO SLOW", 1);
		g_iDesiredTime = time;
		return true;
	} else if (realdelay < 0) {
		return false;
	} else {
		while (SDL_GetTicks() < g_iDesiredTime);
		//SDL_Delay(delay);
		return true;
	}
}
