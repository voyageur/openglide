AUTOMAKE_OPTIONS = foreign

SUBDIRS=platform

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
TexDB.h

# We substitute the libtool-specific library version in configure.in.
libglide2x_la_LDFLAGS = -version-info $(LIBVERSION) -L/usr/X11R6/lib -lGL -lGLU -lXxf86vm
