dnl Setup library CURRENT, REVISION and AGE
m4_define([LIBCUR],0)
m4_define([LIBREV],0)
m4_define([LIBAGE],0)

dnl Process this file with autoconf to produce a configure script.
AC_INIT(libglide2x,2.LIBCUR.LIBREV)
AC_CONFIG_HEADER(config.h)
AC_CONFIG_SRCDIR(Makefile.am)
AM_INIT_AUTOMAKE(no-define)

dnl libtool-style version-info number
LIBVERSION=LIBCUR:LIBREV:LIBAGE
AC_SUBST(LIBVERSION)

AC_CANONICAL_HOST

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX

dnl Initialize libtool.
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

dnl Checks for header files.
AC_HEADER_STDC

AC_OUTPUT(
Makefile \
)

