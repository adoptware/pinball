// !rzr+ Win32 portability hacks @ www.rzr.online.fr
#ifndef config_rzr_h_ // !+rzr 
#define config_rzr_h_
/**
 * @author: www.Philippe.COVAL.free.fr - $author$
 * Portability issues:
 *
 * - Linux + gcc(linux)
 configure && make && make install && make clean
 *
 * - WIN32 + mingw32(linux) 

 cross-configure.sh --prefix=/pinball --disable-sdltest && \
 cross-make.sh
 cross-make.sh -C src wpinball.exe && \
 \cp -f src/wpinball.exe /pinball/

 **/

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
// #define RZR_PATHRELATIVE // @ base/Config.cpp
#undef RELEASE
#define RELEASE
#endif
//--------------------------------------------------------------------------

#ifdef RZR_PATHRELATIVE // on upcoming versions
#undef EM_DATADIR 
#undef EM_LIBDIR
//#define EM_DATADIR "../share/pinball" 
//#define EM_LIBDIR "../lib/pinball"
#define EM_DATADIR "/share/pinball" // single dir and exes in it
#define EM_LIBDIR "lib/pinball"
#endif
//--------------------------------------------------------------------------
#ifdef WIN32 //!+rzr MSVC++ , mingw32 (etc not tested so far)
#ifndef WINAPI  // what are those ? dll rt ?  @ windows.h 4 GL/gl.h
#define WINAPI __stdcall  // not sure it is good , but it worked for me
// ----- // configure should check if the install fs supports modes & links
#include <io.h> // mkdir
#include <sys/stat.h>
#define mkdir(dir,modes) mkdir(dir) // autoconf should do that 
#define lstat( name, opt)  stat(name, opt); //undef on mingw32@Linux
// -----
//#include <cstdlib> // random is not definied 
#define random rand
// ---- Texture  Engine // undefined reference to `gluErrorString@4'
#undef EM_GLERROR
#define EM_GLERROR(a) EM_COUT(a,42)
#endif // WIN32 // !rzr-
#endif

//--------------------------------------------------------------------------
#ifdef  _MSC_VER //!+rzr : MS VC++ 
//#include <windows.h>
//#include <iostream> //!+-rzr
namespace std {};
using namespace std;
#ifdef Polygon // Polygon Is A Macro So It Is Renamed To Be Used As AClass
#undef Polygon
#define Polygon PolygonClass 
#endif // other solution is to write "class Polygon" instead of just Polygon
#endif // msvc6


#endif //!-rzr ----------------------------------------------------------------
