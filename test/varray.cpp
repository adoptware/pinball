//#ident "$Id: varray.cpp,v 1.4 2003/08/29 23:33:10 rzr Exp $"
/***************************************************************************
 Vertex Array test. Tests wheater vertex arrays are faster or not.
 As you can see by looking at the FPS, this is the reason why I am not using
 vertex arrays. Mesa sucks dealing with vertex arrays. Windows  may handle 
 them better but that's a diffrent story.
 ***************************************************************************/

#include <iostream>

#include "Private.h"
#include "Engine.h"
#include "Camera.h"
#include "Keyboard.h"
#include "OpenGLVisitor.h"

#if EM_USE_SDL
#include <SDL_opengl.h> //!rzr //should fix GL portability @ MOX //@varray.cpp
#else
#warning This is a sdl specific test.
#endif

#define VSIZE 8
#define PSIZE 6

Vertex3D vbuf[] = { 
  { 1.0f, 1.0f, -1.0f},
  { -1.0f, 1.0f, -1.0f},
  { 1.0f, -1.0f, -1.0f},
  { -1.0f, -1.0f, -1.0f},
  { 1.0f, 1.0f, 1.0f},
  { -1.0f, 1.0f, 1.0f},
  { 1.0f, -1.0f, 1.0f},
  { -1.0f, -1.0f, 1.0f},
};

unsigned int ibuf[][4] = {
  { 0, 1, 3, 2},
  { 4, 0, 2, 6},
  { 5, 4, 6, 7},
  { 1, 5, 7, 3},
  { 0, 4, 5, 1},
  { 2, 3, 7, 6}
};

Color cbuf[] = {
  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f }
};

/** Main */
int main(int argc, char *argv[]) {
  cerr << "Vertex array test, press space then esc to quit." << endl;

#if EM_USE_SDL
  // Create the engine.
  Engine* engine = new Engine(argc, argv);
  engine->setLightning(0.7f, 0.1f);
  
  // Add a camera. Move a bit.
  Camera* camera = new Camera();
  Group* groupCamera = new Group();
  engine->add(groupCamera);
  groupCamera->setCamera(camera);
  groupCamera->setTranslation(0, 0, 3);
  engine->setEngineCamera(groupCamera);
  
  cerr << "vertex arrays" << endl;
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vbuf);
  glColorPointer(4, GL_FLOAT, 0, cbuf);
  while (!Keyboard::isKeyDown(SDLK_SPACE)) {
    //engine->clearScreen();
    engine->render();
    Keyboard::poll();
    glLoadIdentity();
    glTranslatef(-6.0f, -6.0f, -26.0f);
    for (int a=0; a<5; ++a) {
      for (int b=0; b<5; ++b) {
	for (int c=0; c<5; ++c) {
	  for (int d=0; d<PSIZE; ++d) {
	    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, ibuf[d]);
	  }
	  glTranslatef(3.0f, 0.0f, 0.0f);
	}
	glTranslatef(-15.0f, 3.0f, 0.0f);
      }
      glTranslatef(0.0f, -15.0f, 3.0f);
    }
    engine->swap();
  }
  // polys == 5*5*5*6 = 
  cerr << "fps " << Engine::getFps() << " polys 750 " << endl;
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  cerr << "separate calls" << endl;
  while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {		
    //engine->clearScreen();
    engine->render();
    Keyboard::poll();
    glLoadIdentity();
    glTranslatef(-6.0f, -6.0f, -26.0f);
    for (int a=0; a<5; ++a) {
      for (int b=0; b<5; ++b) {
	for (int c=0; c<5; ++c) {
	  for (int d=0; d<PSIZE; ++d) {
	    glBegin(GL_POLYGON);
	    for (int e=0; e<4; ++e) {
	      glColor4f(cbuf[ibuf[d][e]].r, cbuf[ibuf[d][e]].g, 
			cbuf[ibuf[d][e]].b, cbuf[ibuf[d][e]].a);
	      glVertex3f(vbuf[ibuf[d][e]].x, vbuf[ibuf[d][e]].y, vbuf[ibuf[d][e]].z);
	    }
	    glEnd();
	  }
	  glTranslatef(3.0f, 0.0f, 0.0f);
	}
	glTranslatef(-15.0f, 3.0f, 0.0f);
      }
      glTranslatef(0.0f, -15.0f, 3.0f);
    }
    engine->swap();
  }
  // polys == 5*5*5*6 = 750
  cerr << "fps " << Engine::getFps() << " polys 750 " << endl;
  delete(engine);
#endif
  return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
