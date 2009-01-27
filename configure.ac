dnl Setup library CURRENT, REVISION and AGE
m4_define([LIBCUR],0)
m4_define([LIBREV],0)
m4_define([LIBAGE],0)

dnl Process this file with autoconf to produce a configure script.
AC_INIT(libglide2x,2.LIBCUR.LIBREV)
AM_CONFIG_HEADER(sdk2_unix.h)
AC_CONFIG_SRCDIR(Makefile.am)

dnl libtool-style version-info number
LIBVERSION=LIBCUR:LIBREV:LIBAGE
AC_SUBST(LIBVERSION)
AC_SUBST(DLLFLAGS)

AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(no-define)

dnl Checks for programs.
AC_PROG_CXX

dnl Initialize libtool.
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

dnl Checks for header files.
AC_HEADER_STDC

dnl Checks for data type sizes
AC_CHECK_SIZEOF(unsigned char)
AC_CHECK_SIZEOF(unsigned short)
AC_CHECK_SIZEOF(unsigned int)
AC_CHECK_SIZEOF(unsigned long)
AC_CHECK_SIZEOF(unsigned long long)
AC_CHECK_SIZEOF(int *)

dnl Check for the X11 library
AC_PATH_XTRA

AH_TEMPLATE(C_USE_SDL,[Define to use SDL library])
AC_ARG_ENABLE(sdl,AC_HELP_STRING([--disable-sdl],[Disable SDL support]),,disable_sdl=no)

dnl Check for SDL
if test x$disable_sdl = xno ; then
    SDL_VERSION=1.2.0
    AM_PATH_SDL($SDL_VERSION,
            have_sdl_lib=yes,
            AC_MSG_WARN([SDL version $SDL_VERSION not found!])
    )
fi

if test x$have_sdl_lib = xyes ; then
    AC_DEFINE(C_USE_SDL)
    LIBS="$LIBS $SDL_LIBS"
    CPPFLAGS="$CPPFLAGS $SDL_CFLAGS -I/usr/include"
fi

dnl The target cpu checks
AH_TEMPLATE(HAVE_MMX,[Define if you have mmx support])
AC_MSG_CHECKING(for target cpu type)
case "$target_cpu" in
  x86_64 | amd64)
    AC_DEFINE(HAVE_MMX,1)
    AC_MSG_RESULT(x86-64 bit compatible)
  ;;
  i?86)
    AC_DEFINE(HAVE_MMX,1)
    AC_MSG_RESULT(x86 compatible)
    CXXFLAGS="$CXXFLAGS -mmmx -msse"
    CXXFLAGS="-march=$target_cpu -fomit-frame-pointer $CXXFLAGS"
  ;;
  *)
   AC_MSG_RESULT(unknown)
  ;;
esac

dnl OS flag define test
osflag=""
case "$host" in
    *cygwin* | *mingw32*) 
        CXXFLAGS="$CXXFLAGS -DWIN32 -I\$(top_srcdir)/platform/windows"
        LIBS="$LIBS -lopengl32 -lglu32"
        DLLFLAGS="-Wc,-def -Wc,Glide2x.def"
        ;;
    *freebsd*)
        ;;
    *hpux*)
        ;;
    *irix*)
        ;;
    *linux*)
        osflag="__linux__"
        CXXFLAGS="$CXXFLAGS -D__unix__ -D$osflag -I\$(top_srcdir)/platform/linux $X_CFLAGS"
        LIBS="$LIBS -lGL -lGLU"
        if test x$have_sdl_lib != xyes ; then
            $X_EXTRA_LIBS="$X_EXTRA_LIBS -lXxf86vm"
        fi
        ;;
    *netbsd*)
        ;;
    *qnx*)
        ;;
    *solaris*)
        ;;
    *sunos*)
        ;;
    *)
        ;;
esac

AC_OUTPUT(
Makefile \
platform/Makefile \
platform/linux/Makefile \
platform/sdl/Makefile \
)
