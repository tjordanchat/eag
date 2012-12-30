##
## Process this file with automake to produce Makefile.in
##
## CVSID: "$Id: Makefile.am,v 1.3 2007/09/05 10:27:58 marcs Exp $"
##

if DO_EDT
INCLUDES = -I$(top_srcdir)/libebs -I$(top_srcdir)/libeag \
	   -I$(top_srcdir)/libXedt -I$(X11IDIR)

lib_LTLIBRARIES = libedt.la

include_HEADERS = edt_templates.h \
		  edt_cpmerge.h \
		  edt_unparser.h \
		  edt_edit_ds.h \
		  edt_editmain.h \
		  edt_initedit.h \
		  edt_editorparsing.h

libedt_la_SOURCES = edt_cpmerge.c edt_cpmerge.h \
                    edt_edit_ds.c edt_edit_ds.h \
                    edt_editmain.c edt_editmain.h \
                    edt_editor.c edt_editor.h \
                    edt_editorparsing.c edt_editorparsing.h \
                    edt_focus.c edt_focus.h \
                    edt_initedit.c edt_initedit.h \
                    edt_lrules.c edt_lrules.h \
                    edt_templates.c edt_templates.h \
                    edt_unparser.c edt_unparser.h

libedt_la_LDFLAGS = -version-info 2:3:0

libedt_la_CFLAGS = -Wall $(AM_CFLAGS)

libedt_la_DEPENDENCIES = $(top_builddir)/libebs \
			 $(top_builddir)/libeag \
                         $(top_builddir)/libXedt
endif
