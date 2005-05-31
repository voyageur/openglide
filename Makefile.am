AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = libglide2x.la

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
\
platform/linux/clock.cpp \
platform/linux/error.cpp \
platform/linux/openglext.cpp \
platform/linux/library.cpp \
platform/linux/window.cpp \
\
FormatConversion.h \
glext.h \
Glextensions.h \
GlOgl.h \
GLRender.h \
OGLTables.h \
PGTexture.h \
PGUTexture.h \
sdk2_3dfx.h \
sdk2_glide.h \
sdk2_glidesys.h \
sdk2_glideutl.h \
sdk2_sst1vid.h \
TexDB.h \
\
platform/clock.h \
platform/error.h \
platform/openglext.h \
platform/window.h \
platform/linux/platform.h

INCLUDES = -Iplatform/linux

# We substitute the libtool-specific library version in configure.in.
libglide2x_la_LDFLAGS = -version-info $(LIBVERSION) -L/usr/X11R6/lib -lGL -lGLU -lXxf86vm
