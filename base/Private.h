/***************************************************************************
                          Private.h  -  description
                             -------------------
    begin                : Sat Jul 8 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef PRIVATE_H
#define PRIVATE_H

#include <sys/stat.h>

#include <stddef.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glut.h>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

#ifdef HAVE_CONFIG_H
#include "conf.h"
#else
#error "Must have conf.h file"
#endif

using namespace std;

typedef struct {
	float v[3][3];
	float t[3];
} Matrix;

typedef struct {
	float x, y, z;
} Vertex3D;

typedef struct {
	float u, v;
} TexCoord;

typedef struct {
	float r, g, b, a;
} Color;

// Dummy operator to make MSVC carp work ??
/*
bool operator < (const VertexStruct & vstA, const VertexStruct & vstB) {
	return vstA.vtxSrc.x < vstB.vtxSrc.x;
}

bool operator == (const VertexStruct & vstA, const VertexStruct & vstB) {
	return vstA.vtxSrc.x == vstB.vtxSrc.x;
}
*/

// Dummy operator to make MSVC crap work ??
/*
bool operator < (const PolygonEdge & peA, const PolygonEdge & peB) {
	return peA.index < peB.index;
}

bool operator == (const PolygonEdge & peA, const PolygonEdge & peB) {
	return peA.index == peB.index;
}
*/

extern Matrix identityMatrix;

#if EM_DEBUG
  #define EM_COUT(a, level) if (level > 0) { cerr << a << endl; };
#else
	#define EM_COUT(a, level)
#endif

#if EM_FULL_DEBUG
  #define EM_COUT_D(a, level) if (level > 0) { cerr << a << endl; };
#else
	#define EM_COUT_D(a, level)
#endif

#define EM_GLERROR(a) \
	GLenum error = glGetError(); \
	if (error != GL_NO_ERROR) {  \
		const GLubyte* str = gluErrorString(error); \
		cerr << "OpenGL error: " << a << str << endl;    \
	}

#if EM_DEBUG
  #define EmAssert(a, b)	\
	   if (!(a)) {						\
		   cerr << b << " : In file " << __FILE__ << ":" << __LINE__ << endl; \
		   exit(0);							\
	   }
#else
#define EmAssert(a, b)
#endif

typedef struct {
	SDL_AudioSpec spec;
	Uint32 length;
	Uint32 pos;
	Uint8 *wave;
	bool loop;
} WaveStruct;


typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned int channels;
	unsigned char* pixels;
} struct_image;


#define EmImage struct_image
#define EmTexture GLuint
#define EmSample WaveStruct

#endif // PRIVATE_H
