dnl Process this file with autoconf 2.53 to produce a configure script.

dnl init autoconf and automake
AC_INIT([pinball],[0.3.1],[[henqvist@users.sourceforge.net]])

AM_CONFIG_HEADER(pinconfig.h)

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE

dnl this is a c++ program
dnl CFLAGS="-g -W -Wall -O2"
dnl CXXFLAGS="-g -W -Wall -O2"

AC_ARG_WITH(cflags,
[  --with-cflags=CFLAGS            use CFLAGS as compile time arguments.],
    [CFLAGS=$with_cflags; export CFLAGS])

AC_ARG_WITH(cxxflags,
[  --with-cxxflags=CXXFLAGS        use CXXFLAGS as compile time arguments.],
    [CXXFLAGS=$with_cxxflags; export CXXFLAGS])

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
_LT_SET_OPTION([LT_INIT],[dlopen])
AC_DIAGNOSE([obsolete],[AC_LIBTOOL_DLOPEN: Remove this warning and the call to _LT_SET_OPTION when you
put the 'dlopen' option into LT_INIT's first parameter.])

LTDL_INIT
LT_INIT

AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)

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

AC_ARG_WITH(highscore-prefix, [  --with-highscore-prefix=PFX    Install highscore files in PFX (/var/games/pinball)], highscore_prefix="$withval", highscore_prefix="")

if test x"$highscore_prefix" != "x"; then
	EM_HIGHSCORE_DIR=$highscore_prefix
else
	EM_HIGHSCORE_DIR="/var/games/pinball"
fi

dnl EM_HIGHSCORE_DIR="/var/games/pinball"
AC_SUBST(EM_HIGHSCORE_DIR)
eval EM_HIGHSCORE_PATH=`eval echo "$EM_HIGHSCORE_DIR"`
AC_DEFINE_UNQUOTED(EM_HIGHSCORE_DIR, "$EM_HIGHSCORE_PATH", [Highscore directory])


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
    *-*-darwin*)
        MATHLIB=""
        AC_CHECK_HEADER(
            [OpenGL/gl.h],
            [ac_cv_search_glBegin="none required"],
            [AC_MSG_ERROR(*** OpenGL headers not found on system!)]
        )
        AC_CHECK_HEADER(
            [OpenGL/glu.h],
            [ac_cv_search_gluScaleImage="none required"],
            [AC_MSG_ERROR(*** GLU headers not found on system!)]
        )
        ;;
    *-*-aix*)
        if test x$ac_cv_c_compiler_gnu = xyes; then
            CFLAGS="-mthreads"
				fi
        SYS_GL_LIBS=""
        ;;
    *)
        MATHLIB="-lm"
        AC_CHECK_HEADER(
            [GL/gl.h],
            AC_SEARCH_LIBS(glBegin, [opengl32 GL GL2 OpenGL], [test $ac_cv_search_glBegin = "none required" || GL_LIBS="$ac_cv_search_glBegin $GL_LIBS"], [AC_MSG_ERROR(*** OpenGL library not found on system!)]),
            [AC_MSG_ERROR(*** OpenGL headers not found on system!)]
        )
        AC_CHECK_HEADER(
            [GL/glu.h],
            AC_SEARCH_LIBS(gluScaleImage, [glu32 GL GLU], [test $ac_cv_search_gluScaleImage = "none required" || GL_LIBS="$ac_cv_search_gluScaleImage $GL_LIBS"], [AC_MSG_ERROR(*** GLU library not found on system!)]),
            [AC_MSG_ERROR(*** GLU headers not found on system!)]
        )
        ;;
esac

AC_SUBST(MATHLIB)
LIBS="$MATHLIB $LIBS"
AC_SUBST(GL_LIBS)
LIBS="$GL_LIBS $LIBS"

dnl *******************************************
dnl SDL ***************************************
PKG_CHECK_MODULES([SDL],
	[sdl2 >= 2.0.0],
	[],
	[SDL_VERSION=2.0.0
	 AM_PATH_SDL2($SDL_VERSION, :,
		AC_MSG_ERROR([SDL version $SDL_VERSION not found! http://www.libsdl.org]))])

CFLAGS="$CFLAGS $SDL_CFLAGS" 
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS" 
LIBS="$LIBS $SDL_LIBS"
DEP_PKGS="$DEP_PKGS sdl2 >= 2.0.0"

dnl SDL_image *********************************
AC_ARG_WITH(sdl2image-prefix, [  --with-sdl2image-prefix=PFX prefix where SDL2_image library is installed], sdl2image_prefix="$withval", sdl2image_prefix="") 

if test x"$sdl2image_prefix" != "x"; then
  SDL2IMAGE_LIB="-L$sdl2image_prefix/lib"
  SDL2IMAGE_CFLAGS="-I$sdl2image_prefix/include"
  LDFLAGS="$SDL2IMAGE_LIB $LDFLAGS"
  CPPFLAGS="$CPPFLAGS $SDL2IMAGE_CFLAGS"
  CFLAGS="$CFLAGS $SDL2IMAGE_CFLAGS"
fi

if test x"$enable_libtest" = x"yes"; then

  PKG_CHECK_MODULES([SDL2_IMAGE],
	[SDL2_image],
	[],
	[AC_MSG_NOTICE([SDL2_image not located by pkg-config, checking with AC CHECK LIB:])
	 AC_CHECK_LIB([SDL2_image],
		[IMG_Load],
		[],
		[AC_MSG_ERROR([SDL2_image not found! http://www.libsdl.org/projects/SDL2_image])])])
  LIBS="$LIBS -lSDL2_image"
fi

dnl SDL_mixer **********************************
AC_ARG_WITH(sdl2mixer-prefix, [  --with-sdl2mixer-prefix=PFX prefix where SDL2_mixer library is installed], sdl2mixer_prefix="$withval", sdl2mixer_prefix="")

if test x"$sdl2mixer_prefix" != "x"; then
  SDL2MIXER_LIB="-L$sdl2mixer_prefix/lib"
  SDL2MIXER_CFLAGS="-I$sdl2mixer_prefix/include"
  LDFLAGS="$SDL2MIXER_LIB $LDFLAGS"
  CPPLAGS="$CPPFLAGS $SDL2MIXER_CFLAGS"
  CFLAGS="$CFLAGS $SDL2MIXER_CFLAGS"
fi

if test x"enable_libtest" = x"yes"; then
  AC_CHECK_LIB(SDL2_mixer, Mix_OpenAudio, : ,AC_MSG_ERROR([*** libSDL2_mixer not found. Make sure you have the development package of SDL2_mixer installed - (www.libsdl.org) - or try to use --with-sdl2mixer-prefix option]), )
fi
LIBS="$LIBS -lSDL2_mixer"

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

AC_CONFIG_FILES([Makefile addon/Makefile base/Makefile data/Makefile data/tux/Makefile data/professor/Makefile src/Makefile test/Makefile pinball-config])

AC_OUTPUT
