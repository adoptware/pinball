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
//#include "SignalVisitor.h"
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

#define FPS 50 // req 100 50 33 25 20

int iLoops = 0;
volatile int iSeconds = 0;

int aFunction[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int iCurrentFct = 0;

Uint32 callBack(Uint32 interval) {
	iSeconds++;
  aFunction[iCurrentFct]++;
  return interval;
}

void resize( unsigned int w, unsigned int h ) {

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

void Engine::initSound() {
	SDL_AudioSpec wanted, obtained;
	extern void fill_audio(void *udata, Uint8 *stream, int len);
	
	/* Set the audio format */
	wanted.freq = 22050;
	wanted.format = AUDIO_S16;
	wanted.channels = 2;    /* 1 = mono, 2 = stereo */
	wanted.samples = 512;  /* Good low-latency value for callback */
	wanted.callback = fill_audio;
	wanted.userdata = NULL;
	
	/* Open the audio device, forcing the desired format */
	if ( SDL_OpenAudio(&wanted, &obtained) < 0 ) {
		cerr << "Couldn't open audio: " << SDL_GetError() << endl;
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
	SDL_Surface* screen = SDL_SetVideoMode(config->getWidth(), config->getHeight(), 
																				 config->getBpp(), 
																				 SDL_OPENGL | 
																				 (config->getFullScreen() ? SDL_FULLSCREEN : 0));
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
	
	resize( config->getWidth(), config->getHeight() );
}

Engine::Engine(int & argc, char *argv[]) {
	m_Background = NULL;

	Config * config = Config::getInstance();
	config->loadArgs(argc, argv);

  srand((unsigned int)time((time_t *)NULL));

  // Init grx, sound
	if (!config->getExternGL()) {
		this->initGrx();
		SDL_SetTimer((int)(1000/FPS), callBack);
		if (config->getSound()) this->initSound();
	}

  // Create Visitors.
  p_AmbientLightVisitor = new AmbientLightVisitor();
  p_BehaviorVisitor = new BehaviorVisitor();
  p_CollisionVisitor = new CollisionVisitor(100);
  p_PointLightVisitor = new PointLightVisitor();
  p_PNormalVisitor = new PNormalVisitor();
	//  p_SignalVisitor = new SignalVisitor();
  p_SoundVisitor = new SoundVisitor();
  p_TransformVisitor = new TransformVisitor();
  p_OpenGLVisitor = new OpenGLVisitor();
  p_OpenGLTransVisitor = new OpenGLTransVisitor();
}

Engine::~Engine() {
	delete(p_AmbientLightVisitor);
	delete(p_BehaviorVisitor);
	delete(p_OpenGLVisitor);
	delete(p_OpenGLTransVisitor);
	delete(p_PointLightVisitor);
	delete(p_PNormalVisitor);
	delete(p_SoundVisitor);
	//	delete(p_SignalVisitor);
	delete(p_TransformVisitor);
	delete(p_CollisionVisitor);
	
	if (!Config::getInstance()->getExternGL()) {
		SDL_Quit();
	}
	
	cerr << "Function null " << aFunction[0] << endl;
	cerr << "Function align " << aFunction[ALIGN] << endl;
	cerr << "Function beh " << aFunction[BEH] << endl;
	cerr << "Function collision " << aFunction[COLLISION] << endl;
	cerr << "Function draw " << aFunction[DRAW] << endl;
	cerr << "Function glight " << aFunction[GLIGHT] << endl;
	cerr << "Function plight " << aFunction[PLIGHT] << endl;
	cerr << "Function pnormal " << aFunction[PNORMAL] << endl;
	cerr << "Function render " << aFunction[RENDER] << endl;
	cerr << "Function sound " << aFunction[SOUND] << endl;
	cerr << "Function signal " << aFunction[SIG] << endl;
	cerr << "Function trans " << aFunction[TRANS] << endl;
	cerr << "Function swap " << aFunction[SWAP] << endl;
	cerr << "Seconds " << ((float)iSeconds/FPS) <<" " << ((float)iLoops*FPS/iSeconds) 
			 << " fps(" << FPS << ")" << endl;
}

void Engine::addLight(Light* l) {
 	p_PointLightVisitor->add(l);
}

void Engine::clearScreen() {
	iCurrentFct = DRAW;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glColor3f(1, 1, 1);
	glLoadIdentity();
  gluLookAt(0,0,0, 0,0,-1, 0,1,0);
}

void Engine::setLightning(float fV, float fB) {
 	p_AmbientLightVisitor->setLightning(fV, fB);
	// light model
}

void Engine::setBackground(EmImage * bg) {
	m_Background = bg;
}

void Engine::setEngineCamera(Group* g) {
	AlignVisitor::getInstance()->setCamera(g);
	p_TransformVisitor->setCamera(g);
}

void Engine::render() {
	EM_COUT("Engine::render()", 0);
	// Put some overall light.
	EM_COUT("Engine::render() glight", 0);
	iCurrentFct = GLIGHT;
	p_AmbientLightVisitor->empty();
	this->accept(p_AmbientLightVisitor);

	// Put some light from light sources.
	EM_COUT("Engine::render() plight", 0);
	iCurrentFct = PLIGHT;
	p_PointLightVisitor->empty();
	this->accept(p_PointLightVisitor);

	// Align vertices to view space.
	EM_COUT("Engine::render() align", 0);
	iCurrentFct = ALIGN;
	AlignVisitor::getInstance()->empty();
	this->accept(AlignVisitor::getInstance());

	// Adjust sounds.
	EM_COUT("Engine::render() sound", 0);
	iCurrentFct = SOUND;
//	p_SoundVisitor->empty();
//	this->accept(p_SoundVisitor);

	// Draw screen
	EM_COUT("Engine::render() opengl", 0);
	iCurrentFct = RENDER;
	this->clearScreen();
	p_OpenGLVisitor->empty();
	this->accept(p_OpenGLVisitor);

	EM_COUT("Engine::render() opengltrans", 0);
	p_OpenGLTransVisitor->empty();
	this->accept(p_OpenGLTransVisitor);

	iCurrentFct = 0;
}

void Engine::swap() {
	EM_COUT("Engine::swap()", 0);
	iCurrentFct = SWAP;
	iLoops++;
	// Draw to screen.
	if (!Config::getInstance()->getExternGL()) {
		SDL_GL_SwapBuffers();
	}

	GLenum error = glGetError();
	if( error != GL_NO_ERROR ) {
		EM_COUT("OpenGL error: " << error << endl, 1);
	}
	iCurrentFct = 0;
}

void Engine::tick() {
	EM_COUT("Engine::tick()", 0);
	// Check keyboard.
	Keyboard::poll();

	// Perform signals.
	EM_COUT("Engine::tick() signal", 0);
	iCurrentFct = SIG;
	SignalSender::getInstance()->tick();
	/*
	p_SignalVisitor->empty();
	this->accept(p_SignalVisitor);
	p_SignalVisitor->clear();
	*/

	// Perform behaviors.
	EM_COUT("Engine::tick() behavior", 0);
	iCurrentFct = BEH;
	p_BehaviorVisitor->empty();
	this->accept(p_BehaviorVisitor);

	// Calculate positions
	EM_COUT("Engine::tick() trans", 0);
	iCurrentFct = TRANS;
	p_TransformVisitor->empty();
	this->accept(p_TransformVisitor);

	// Detect collision.
	EM_COUT("Engine::tick() collision", 0);
	iCurrentFct = COLLISION;
	p_CollisionVisitor->empty();
	this->accept(p_CollisionVisitor);

	/*
	// Calculate positions one more time, collision may have changed the positions.
	EM_COUT("Engine::tick() trans" << endl, 0);
	iCurrentFct = TRANS;
	p_TransformVisitor->empty();
	this->accept(p_TransformVisitor);
	*/
	iCurrentFct = 0;
}

