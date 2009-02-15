AUTOMAKE_OPTIONS = foreign

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
OGLMiscTables.cpp \
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
gbanner.cpp \
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
libglide2x_la_LDFLAGS = -no-undefined -version-info $(LIBVERSION) $(X_PRE_LIBS) $(X_LIBS) $(X_EXTRA_LIBS) $(DLLFLAGS)

INCLUDES=-g3 -O0

libglide2x_la_LIBADD = ./platform/linux/libplatform.la ./platform/sdl/libsdl.la

install-data-hook:
	ln -sf $(libglide2xincludedir)/sdk2_glide.h $(includedir)/glide.h
