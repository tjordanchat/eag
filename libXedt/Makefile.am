##
## Process this file with automake to produce Makefile.in
##
if DO_EDT
INCLUDES = -I$(top_srcdir)/libebs -I$(X11IDIR)

lib_LTLIBRARIES = libXedt.la

libXedt_la_SOURCES = Editor.c Editor.h EditorP.h \
                     ImmedLabel.c ImmedLabel.h ImmedLabelP.h \
                     Primitive.c Primitive.h PrimitiveP.h \
                     XSupp.c XSupp.h \
                     XtArgs.c XtArgs.h \
                     XtSupp.c XtSupp.h

libXedt_la_LDFLAGS = -version-info 2:3:0
libXedt_la_CFLAGS = -Wall $(AM_CFLAGS)
endif
