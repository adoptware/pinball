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

#if EM_USE_SDL
#include <GL/gl.h>
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
		engine->clearScreen();
		Keyboard::poll();
		glLoadIdentity();
		glTranslatef(-6.0f, 0.0f, -10.0f);
		for (int a=0; a<5; ++a) {
			for (int b=0; b<PSIZE; ++b) {
				glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, ibuf[b]);
			}
			glTranslatef(3.0f, 0.0f, 0.0f);
		}
		engine->swap();
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	cerr << "separate calls" << endl;
	while (!Keyboard::isKeyDown(SDLK_ESCAPE)) {		
		engine->clearScreen();
		Keyboard::poll();
		glLoadIdentity();
		glTranslatef(-6.0f, 0.0f, -10.0f);
		for (int a=0; a<5; ++a) {
			for (int b=0; b<PSIZE; ++b) {
				glBegin(GL_POLYGON);
				for (int c=0; c<4; ++c) {
					glColor4f(cbuf[ibuf[b][c]].r, cbuf[ibuf[b][c]].g, cbuf[ibuf[b][c]].b, cbuf[ibuf[b][c]].a);
					glVertex3f(vbuf[ibuf[b][c]].x, vbuf[ibuf[b][c]].y, vbuf[ibuf[b][c]].z);
				}
				glEnd();
			}
			glTranslatef(3.0f, 0.0f, 0.0f);
		}
		engine->swap();
	}
	delete(engine);
#endif
	return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
