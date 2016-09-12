AUTOMAKE_OPTIONS = foreign

#bin_PROGRAMS = test 
#test_SOURCES = test12.c tlib.c linutil.c
#test_LDADD = ./libglide2x.la
#test_LDADD = -L/usr/lib -lglide2x -lm

SUBDIRS=platform
EXTRA_DIST=splash.dat banner.dat

lib_LTLIBRARIES = libglide2x.la

libglide2xincludedir = $(includedir)/openglide
libglide2xinclude_HEADERS = \
sdk2_glide.h \
sdk2_3dfx.h \
sdk2_glidesys.h \
sdk2_glideutl.h \
sdk2_sst1vid.h \
sdk2_unix.h

libglide2x_la_SOURCES = \
grguDepth.cpp \
grguMisc.cpp \
grgu3df.cpp \
grguDraw.cpp \
grguSstGlide.cpp \
grguFog.cpp \
grguTex.cpp \
grguLfb.cpp \
GLRender.cpp \
OGLFogTables.cpp \
OGLTextureTables.cpp \
OGLColorAlphaTables.cpp \
TexDB.cpp \
PGUTexture.cpp \
Glide.cpp \
GLExtensions.cpp \
PGTexture.cpp \
FormatConversion.cpp \
grguBuffer.cpp \
grguColorAlpha.cpp \
GLutil.cpp \
gsplash.cpp \
\
FormatConversion.h \
glext.h \
Glextensions.h \
GlOgl.h \
GLRender.h \
OGLTables.h \
PGTexture.h \
PGUTexture.h \
TexDB.h

# We substitute the libtool-specific library version in configure.in.
libglide2x_la_LDFLAGS = -no-undefined -version-info $(LIBVERSION) $(DLLFLAGS)

libglide2x_la_LIBADD = ./platform/linux/libplatform.la ./platform/sdl/libsdl.la ./platform/windows/libwindows.la

install-data-hook:
	cd $(DESTDIR)$(includedir) && \
		$(LN_S) -f openglide/sdk2_glide.h glide.h

install-exec-hook:
	cd $(DESTDIR)$(libdir) && \
		$(LN_S) -f lib*.so libglide.so.2
