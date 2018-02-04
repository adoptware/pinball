dnl #                                               -*- Autoconf -*-
dnl # Process this file with autoconf 2.53 to produce a configure script.
dnl init autoconf and automake
AC_INIT(pinball, 0.3.4.1)

AC_CONFIG_HEADERS(pinconfig.h)
LT_CONFIG_LTDL_DIR([libltdl])
AC_CONFIG_AUX_DIR([libltdl/config])
AC_CONFIG_MACRO_DIR([libltdl/m4])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE

dnl # Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_MKDIR_P
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION(0.19.8)
#0.17
dnl this is a c++ program
dnl CFLAGS="-g -W -Wall -O2"
dnl CXXFLAGS="-g -W -Wall -O2"

AC_ARG_WITH(cflags,
[  --with-cflags=CFLAGS            use CFLAGS as compile time arguments.],
    [CFLAGS=$with_cflags; export CFLAGS])

AC_ARG_WITH(cxxflags,
[  --with-cxxflags=CXXFLAGS        use CXXFLAGS as compile time arguments.],
    [CXXFLAGS=$with_cxxflags; export CXXFLAGS])

AC_LIBTOOL_DLOPEN
LTDL_INIT

dnl *******************************************
dnl PATHS AND DIRS ****************************
dnl data dirs, fuck I hate autoconf, spent 4 hours getting this right, had to 
dnl steal it from xine and even that took a couple of hours
if test "x$prefix" = xNONE; then
  prefix="${ac_default_prefix}"
fi

if test "x$exec_prefix" = xNONE; then
  exec_prefix="$prefix"
fi

EM_DATADIR="$datadir/$PACKAGE_NAME"
AC_SUBST(EM_DATADIR)
eval EM_DATAPATH=`eval echo "$EM_DATADIR"`
AC_DEFINE_UNQUOTED(EM_DATADIR, "$EM_DATAPATH", [The data dir /usr/local/share/pinball])

EM_LIBDIR="$libdir/$PACKAGE_NAME"
AC_SUBST(EM_LIBDIR)
eval EM_LIBPATH=`eval echo "$EM_LIBDIR"`
AC_DEFINE_UNQUOTED(EM_LIBDIR, "$EM_LIBPATH", [The library or plugin dir /usr/local/lib/pinball])


dnl special fix for rpmbuild, I hate rpmbuild even more than I hate autoconf
AC_ARG_WITH(buildroot-prefix, [  --with-buildroot-prefix=PFX    Only used when building packages], buildroot_prefix="$withval", buildroot_prefix="")

if test x"$buildroot_prefix" != "x"; then
	EM_BUILD_ROOT=$buildroot_prefix
else
	EM_BUILD_ROOT="/"
fi
AC_SUBST(EM_BUILD_ROOT)

AC_ARG_WITH(localization-prefix, [  --with-localization-prefix=PFX    Install localization files in PFX (/usr/share/locale)], localization_prefix="$withval", localization_prefix="")

if test x"$localization_prefix" != "x"; then
    EM_LOCALIZATION_DIR=$localization_prefix
else
    EM_LOCALIZATION_DIR="/usr/share/locale"
fi

dnl EM_LOCALIZATION_DIR="/usr/share/locale"
AC_SUBST(EM_LOCALIZATION_DIR)
eval EM_LOCALIZATION_PATH=`eval echo "$EM_LOCALIZATION_DIR"`
AC_DEFINE_UNQUOTED(EM_LOCALIZATION_DIR, "$EM_LOCALIZATION_PATH", [Localization directory])


AC_ARG_ENABLE(libtest, [  --disable-libtest     SDL_image _mixer libtest work around ], , enable_libtest=yes)

dnl *******************************************
dnl Allegro or SDL version ********************

AC_ARG_WITH(allegro, [  --with-allegro          use allegro (software rendering) instead of SDL and OpenGL], use_allegro="yes", use_allegro="no")

if test x"$use_allegro" = "xno"; then

AC_DEFINE(EM_USE_SDL, [1], [Shall we use SDL and OpenGL])

dnl *******************************************
dnl OPENGL ************************************
dnl Figure out which math and GL library to use
case "$target" in
    *-*-cygwin* | *-*-mingw32*)
        MATHLIB=""
        SYS_GL_LIBS="-lopengl32"
        ;;	
    *-*-beos*)
        MATHLIB=""
        SYS_GL_LIBS="-lGL"
        ;;
    *-*-darwin*)
        MATHLIB=""
        SYS_GL_LIBS=""
        ;;
    *-*-aix*)
        if test x$ac_cv_prog_gcc = xyes; then
            CFLAGS="-mthreads"
				fi
        SYS_GL_LIBS=""
        ;;
    *)
        MATHLIB="-lm"
        AC_PATH_X
        AC_PATH_XTRA
        if test x$have_x = xyes; then
            CFLAGS="$CFLAGS $X_CFLAGS"
            SYS_GL_LIBS="$X_PRE_LIBS $X_LIBS $X_EXTRA_LIBS -lGL -lGLU"
        else
            SYS_GL_LIBS="-lGL -lGLU"
        fi
        ;;
esac

AC_SUBST(MATHLIB)

dnl Check for OpenGL
AC_MSG_CHECKING(for OpenGL support)
AC_TRY_COMPILE([
#ifdef WIN32
# define WINAPI
#endif

 #if defined(__APPLE__) && defined(__MACH__)
 # include <OpenGL/gl.h>
 #else
 # include <GL/gl.h>
 #endif
], , , AC_MSG_ERROR([*** OpenGL not found on system!]))
AC_MSG_RESULT(yes)
GL_LIBS="$SYS_GL_LIBS"
AC_SUBST(GL_LIBS)
LIBS="$GL_LIBS $LIBS"

dnl *******************************************
dnl SDL ***************************************
AM_PATH_SDL(1.2.0, : , AC_MSG_ERROR([*** SDL version 1.2.0 not found! Make sure you have the development package of SDL installed - (www.libsdl.org) - or try to use the --with-sdl-prefix option]))
CFLAGS="$CFLAGS $SDL_CFLAGS"
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

dnl SDL_image *********************************
AC_ARG_WITH(sdlimage-prefix, [  --with-sdlimage-prefix=PFX prefix where SDL_image library is installed], sdlimage_prefix="$withval", sdlimage_prefix="") 

if test x"$sdlimage_prefix" != "x"; then
  SDLIMAGE_LIB="-L$sdlimage_prefix/lib"
  SDLIMAGE_CFLAGS="-I$sdlimage_prefix/include"
  LDFLAGS="$SDLIMAGE_LIB $LDFLAGS"
  CPPFLAGS="$CPPFLAGS $SDLIMAGE_CFLAGS"
  CFLAGS="$CFLAGS $SDLIMAGE_CFLAGS"
fi

if test x"$enable_libtest" = x"yes"; then
  AC_CHECK_LIB(SDL_image, IMG_Load, : ,AC_MSG_ERROR([*** libSDL_image not found! Make sure you have the development package of SDL_image installed - (www.libsdl.org) - or try to use the --with-sdlimage-prefix option]), )
fi
LIBS="$LIBS -lSDL_image"

dnl SDL_mixer **********************************
AC_ARG_WITH(sdlmixer-prefix, [  --with-sdlmixer-prefix=PFX prefix where SDL_mixer library is installed], sdlmixer_prefix="$withval", sdlmixer_prefix="")

if test x"$sdlmixer_prefix" != "x"; then
  SDLMIXER_LIB="-L$sdlmixer_prefix/lib"
  SDLMIXER_CFLAGS="-I$sdlmixer_prefix/include"
  LDFLAGS="$SDLMIXER_LIB $LDFLAGS"
  CPPLAGS="$CPPFLAGS $SDLMIXER_CFLAGS"
  CFLAGS="$CFLAGS $SDLMIXER_CFLAGS"
fi

if test x"enable_libtest" = x"yes"; then
  AC_CHECK_LIB(SDL_mixer, Mix_OpenAudio, : ,AC_MSG_ERROR([*** libSDL_mixer not found. Make sure you have the development package of SDL_mixer installed - (www.libsdl.org) - or try to use --with-sdlmixer-prefix option]), )
fi
LIBS="$LIBS -lSDL_mixer"

dnl else use allegro instead of sdl

else

AC_DEFINE(EM_USE_ALLEGRO, [1], [Shall we use allegro - software rendering])

dnl *******************************************
dnl ALLEGRO ***********************************
AM_PATH_ALLEGRO(4.0.0, : , AC_MSG_ERROR([*** Allegro version 4.0.0 not found! Make sure you have the development package of Allegro installed - (www.talula.demon.co.uk/allegro) - or try to use the --with-allegro-prefix option]) )
CFLAGS="$CFLAGS $ALLEGRO_CFLAGS"
CPPFLAGS="$CPPFLAGS $ALLEGRO_CFLAGS"
LIBS="$LIBS $ALLEGRO_LIBS"

fi 
dnl End Allegro or SDL *************************


dnl ********************************************
dnl CPP_UNIT ***********************************

AC_ARG_WITH(unittest, [  --with-unittest         Try to use unitest], use_unittest="yes", use_unittest="no")

if test x"$use_unittest" = "xyes"; then

AM_PATH_CPPUNIT(1.8.0, [
  AC_DEFINE(EM_UNIT_TEST, [1], [Shall we compile unittests]) 
  CPPFLAGS="$CPPFLAGS $CPPUNIT_CFLAGS"	
  CFLAGS="$CFLAGS $CPPUNIT_CFLAGS" 
  LIBS="$LIBS $CPPUNIT_LIBS"
  ], AC_MSG_ERROR([*** CppUnit was not found. No unit testing.]))

fi

dnl ********************************************
dnl HEADERS ************************************
dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(sys/stat.h, : ,AC_MSG_ERROR([*** Header sys/stat.h not found. Not able to save config]))
AC_CHECK_HEADERS(sys/types.h, : ,AC_MSG_ERROR([*** Header sys/types.h not found. Not able to save config]))


# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([getcwd gettimeofday memset mkdir pow sqrt strchr strrchr strstr])


dnl ********************************************
dnl Debug and Config ***************************
AC_ARG_WITH(debug, [  --with-debug            Compile debug version], use_debug="yes", use_debug="no")

if test x"$use_debug" = "xyes"; then
AC_DEFINE(EM_DEBUG, [1], [Shall we use debug features such as profiling and keyboard ball steering])
fi

AC_DEFINE(EM_USE_SOURCE_SPECULAR, [1], [Specular])
dnl AC_DEFINE(EM_USE_FAST_FLOAT2INT)
dnl AC_DEFINE(EM_DEBUG_COLLISION)
AC_DEFINE(EM_USE_QUADTREE, [1], [Quadtree instead of octtree])
AC_DEFINE(EM_USE_SHARED_COLOR, [1], [One color per vertex])

dnl AC_SUBST(pkglibdir)
dnl AC_SUBST(pkgincludedir)

AC_CONFIG_FILES([Makefile addon/Makefile base/Makefile data/Makefile
 src/Makefile test/Makefile pinball-config
 data/tux/Makefile data/professor/Makefile data/hurd/Makefile
 data/gnu/Makefile data/professor2/Makefile data/locale/Makefile.in
])

AC_OUTPUT
