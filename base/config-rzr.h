//#ident "$Id: config-rzr.h,v 1.2 2003/04/09 12:19:06 rzr Exp $"
//#warning "!+rzr: Win32 portability hacks @ www.rzr.online.fr"
#ifndef config_rzr_h_ // !+rzr 
#define config_rzr_h_
/**
 * @author: www.Philippe.COVAL.free.fr - $author:$
 * Portability issues:
 *
 * - Linux = gcc@linux
 configure && make && make install && make clean
 *
 * - WIN32 = mingw32@linux

 cross-configure.sh --prefix=/pinball --disable-sdltest && \
 cross-make.sh
 cross-make.sh -C src wpinball.exe && \
 \cp -f src/wpinball.exe /pinball/

 * - WIN32 = msvc@windows
  Install SDL libs
  Just open the project file pinball.dsw
  Build and copy dll in windows/system... or build dir (Release or Debug)
  exec Pinball.exe
 
 **/
#if ( ! ( (defined __GNUC__ ) || ( defined _MSC_VER )) )
#warning "This compiler has never been tested so far"
#endif

//--------------------------------------------------------------------------
/// default settings customise yourself
/// just define if you dont want to use plugins (dll vs static) 
/// default: undefined
#undef RZR_LIBSTATIC
//#define RZR_LIBSTATIC  // @ src/Loader.cpp

/// if the program file is copied elsewhere it still works
#undef RZR_PATHRELATIVE
/// default: undefined
//#define RZR_PATHRELATIVE // @ base/Config.cpp

#ifdef WIN32
// mingw32@Linux : Dyn loadling works, but keyboard is too slow w/ dll ???
#define RZR_LIBSTATIC 
// no more absolute path
#define RZR_PATHRELATIVE // @ base/Config.cpp
#undef RELEASE
#define RELEASE
#endif

#ifdef RZR_PATHRELATIVE // on upcoming versions
#undef EM_DATADIR 
#undef EM_LIBDIR
//#define EM_DATADIR "../share/pinball" 
//#define EM_LIBDIR "../lib/pinball"
#define EM_DATADIR "share/pinball" // single dir and exes in it (!= unix)
#define EM_LIBDIR "lib/pinball"  // BUT subdirs are like unix
#endif

//--------------------------------------------------------------------------
// Common WIN32 options tested on mingw32 + msvc6
#ifdef WIN32 //!+rzr MSVC++ , mingw32 (etc not tested so far)
#undef EM_DATADIR
#define EM_DATADIR "../data" //Make in work ONLY in the source tree
//#define EM_LIBDIR "." // dll not build (so far)

// ----- // configure should check if the install fs supports modes & links
#ifndef WINAPI  // what are those ? dll rt ?  @ windows.h 4 GL/gl.h
#define WINAPI __stdcall  // not sure it is good , but it worked for me
#endif
#define random rand // cstdlibs random is not definied @msvc + mingw32
//#include <io.h> // mkdir
//#include <sys/stat.h>
//#define mkdir(dir,modes) mkdir(dir) // @direct.h // autoconf should do that 
// ---- Texture  Engine // undefined reference to `gluErrorString@4
//#undef EM_GLERROR
//#define EM_GLERROR(a) EM_COUT(a,42)
#endif // WIN32 // !rzr-
//--------------------------------------------------------------------------
//!+rzr : MS VC++ 's pinconfig.h ; why not have a default pinconfig.h
// will be override with ./configure 
#ifdef  _MSC_VER 
#define EM_USE_SOURCE_SPECULAR 1
#define EM_USE_QUADTREE 1
#define EM_USE_SDLIMAGE 1
#define EM_USE_SDLMIXER 1
#define EM_USE_SDL 1
#define EM_USE_SHARED_COLOR 1
#define HAVE_MEMORY_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 0
#define PACKAGE "pinball"
#define PACKAGE_BUGREPORT ""
#define PACKAGE_NAME ""
#define PACKAGE_STRING ""
#define PACKAGE_TARNAME ""
#define PACKAGE_VERSION ""
#define STDC_HEADERS 1
#define VERSION "cvs-2003-04-08-rzr"
#undef HAVE_UNISTD_H
//--------------------------------------------------------------------------
#undef WINAPI
#include <windows.h>
//#include <iostream> //!+-rzr
namespace std {};
using namespace std;
//#ifdef Polygon // Polygon Is A Macro So It Is Renamed To Be Used As AClass
#undef Polygon
#define Polygon PolygonClass
//#endif // other solution is to write "class Polygon" instead of just Polygon
#endif // msvc6
//--------------------------------------------------------------------------
#ifdef _MSC_VER
// trunct template 255
#pragma warning (disable:4786)
//Disable bogus conversion warnings.
#pragma warning (disable:4244)
// VC++ 5.0 version of above warning.
#pragma warning (disable:4305)
// ??? // MSVC6.0 dll / ccxx
#pragma warning (disable:4273)
// bool / uc
#pragma warning (disable:4800)
// symb trunc
#pragma warning (disable:4786)
// dominance ?
#pragma warning (disable:4250)
// unreferenced
#pragma warning (disable:4101)
#endif

//-----------------------------------------------------------------------------
/// mingw32 hacks
#if ( (defined WIN32 ) && ( defined __GNUC__ ) )
#define lstat( name, opt)  stat(name, opt); //undef on mingw32@Linux
#endif

#endif //!-rzr ----------------------------------------------------------------
