/***************************************************************************
                          Private.h  -  description
                             -------------------
    begin                : Sat Jul 8 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef PRIVATE_H
#define PRIVATE_H

#include <stddef.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glut.h>

#include <SDL/SDL.h>

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

#define EM_DEBUG

#ifdef EM_DEBUG
	extern int em_level_;
	extern int em_counter_;
  #define EM_COUT(a, level) if (level > 0) { cerr << a << endl; };
#else
	#define EM_COUT(a, level)
#endif

#define EmAssert( a, b )	\
	if ( !(a) ) {						\
		cerr << b << " : In file " << __FILE__ << ":" << __LINE__ << endl; \
		exit(0);							\
	}

#define OPENGL_EM
#define SDL_GRX_EM
#define OPENGL_LIGHTS 0

typedef struct {
	SDL_AudioSpec spec;
	Uint32 length;
	Uint32 pos;
	Uint8 *wave;
	bool loop;
} WaveStruct;

#define EmImage SDL_Surface
#define EmTexture GLuint
#define EmSample WaveStruct
#define EmSound int

#endif // PRIVATE_H
