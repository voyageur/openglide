AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = libglide2x.la

libglide2x_la_SOURCES = \
grguDepth.cpp \
grguMisc.cpp \
grgu3df.cpp \
grguDraw.cpp \
grguSstGlide.cpp \
grguBuffer.cpp \
grguFog.cpp \
grguTex.cpp \
grguColorAlpha.cpp \
grguLfb.cpp \
GLExtensions.cpp \
GLRender.cpp \
GLutil.cpp \
OGLFogTables.cpp \
OGLMiscTables.cpp \
OGLTextureTables.cpp \
OGLColorAlphaTables.cpp \
PGTexture.cpp \
PGUTexture.cpp \
TexDB.cpp \
Glide.cpp \
FormatConversion.cpp \
\
amd3dx.h \
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
libglide2x_la_LDFLAGS = -version-info $(LIBVERSION) -lgl
