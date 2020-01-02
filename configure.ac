#                                               -*- Autoconf -*-
# SPDX-License-Identifier: GPL-2

# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(pinball-table-hurd, 0.0.0, [[pinball.sf.net]])
AC_CONFIG_SRCDIR([data/hurd/ModuleHurd.cpp])
AC_CONFIG_HEADERS([config.h])
LT_CONFIG_LTDL_DIR([libltdl])
AC_CONFIG_AUX_DIR([libltdl/config])
AC_CONFIG_MACRO_DIR([libltdl/m4])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_LIBTOOL_DLOPEN
LTDL_INIT

# Checks for libraries.
PKG_CHECK_MODULES([SDL],
	[sdl >= 1.2.0],
	[],
	[SDL_VERSION=1.2.0
	 AM_PATH_SDL($SDL_VERSION, :,
		AC_MSG_ERROR([SDL version $SDL_VERSION not found! http://www.libsdl.org]))])

CFLAGS="$CFLAGS $SDL_CFLAGS" 
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS" 
LIBS="$LIBS $SDL_LIBS"
DEP_PKGS="$DEP_PKGS sdl >= 1.2.0"

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.


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

EM_LIBDIR="$libdir/pinball"
AC_SUBST(EM_LIBDIR)
eval EM_LIBPATH=`eval echo "$EM_LIBDIR"`
AC_DEFINE_UNQUOTED(EM_LIBDIR, "$EM_LIBPATH", [The library or plugin dir /usr/local/lib/pinball])


AC_CONFIG_FILES([Makefile
data/hurd/Makefile
])
AC_OUTPUT
