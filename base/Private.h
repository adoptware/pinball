//#ident "$Id: Private.h,v 1.17 2003/06/11 13:25:49 rzr Exp $"
/***************************************************************************
                          Private.h  -  description
                             -------------------
    begin                : Sat Jul 8 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef PRIVATE_H
#define PRIVATE_H

#ifdef HAVE_CONFIG_H //autoconf
#include "pinconfig.h"
#else //!+rzr: will merge in a config-static.h
#define RZR_PATCHES   
#endif // msvc

/// TODO: Enable this flag for profiling app using external "trako" lib
/// https://github.com/rzr/trako/
#if defined(PINBALL_CONFIG_TRAKO) && PINBALL_CONFIG_TRAKO
# include <trako/trako.h>
# define PINBALL(cmd)                           \
  TRAKO(cmd)
#else
# define PINBALL(cmd)
#endif



#ifndef unix
#undef  RZR_PATCHES   
#define RZR_PATCHES   
#endif
// If using autoconf and want patches just add :
// #define RZR_PATCHES (in pinconfig.h))
#ifdef RZR_PATCHES  
#include "config-rzr.h" // in a separate file until a better integration
#endif //!-rzr 

#ifndef EM_USE_SDL
#define EM_USE_SDL 0
#endif

#ifndef EM_USE_ALLEGRO
#define EM_USE_ALLEGRO 0
#endif

#if EM_USE_SDL
#if EM_USE_ALLEGRO
#error "Can't compile sdl and allegro at the same time."
#endif
#endif

#if EM_USE_SDL
#else
#if EM_USE_ALLEGRO
#else
#error "Must specify allegro or sdl."
#endif
#endif
#ifndef  WIN32 // not for msvc, codewarrior // can't be empty for codewarrior
namespace std {}; // !+-rzr: must be definied before used  (msvc)
#endif
using namespace std;

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


#if EM_DEBUG
#include <iostream>
#define EM_COUT(a, level) if (level > 0) { cerr << a << endl; };
#define EM_CERR(a) cerr << a << endl
#else
#define EM_COUT(a, level)
#define EM_CERR(a)
#endif

#if EM_FULL_DEBUG
  #define EM_COUT_D(a, level) if (level > 0) { cerr << a << endl; };
#else
	#define EM_COUT_D(a, level)
#endif

#if EM_USE_SDL
#if EM_DEBUG
#define EM_GLERROR(a) \
	GLenum error = glGetError(); \
	if (error != GL_NO_ERROR) {  \
		const GLubyte* str = gluErrorString(error); \
		cerr << "OpenGL error: " << a << str << endl;    \
	}
#else
#define EM_GLERROR(a)
#endif
#endif // EM_USE_SDL

#if EM_DEBUG
#define EmAssert(a, b)	\
	   if (!(a)) {						\
		   cerr << b << " : In file " << __FILE__ << ":" << __LINE__ << endl; \
		   exit(0);							\
	   }
#else
#define EmAssert(a, b)
#endif


#endif // PRIVATE_H
//EOF: $Id: Private.h,v 1.17 2003/06/11 13:25:49 rzr Exp $
