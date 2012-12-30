##
## Process this file with automake to produce Makefile.in
##
## CVSID: "$Id: Makefile.am,v 1.2 2007/09/05 10:27:53 marcs Exp $"
##
lib_LTLIBRARIES = libeag.la

include_HEADERS = eag_stddefs.eag \
		  eag_ds.h \
		  eag_textparsing.h \
		  eag_buildtree.h \
		  eag_propagate.h \
		  eag_trace.h \
		  eag_nodeinfo.h \
		  eag_misc.h \
		  eag_init.h \
		  eag_c_interface.h \
		  eag_stddefs.h

libeag_la_SOURCES = eag_buildtree.c eag_buildtree.h \
                    eag_c_interface.c eag_c_interface.h \
                    eag_ds.c eag_ds.h \
                    eag_init.c eag_init.h \
                    eag_misc.c eag_misc.h \
                    eag_nodeinfo.h \
                    eag_propagate.c eag_propagate.h \
                    eag_stddefs.c eag_stddefs.h \
                    eag_textparsing.c eag_textparsing.h \
                    eag_trace.c eag_trace.h

INCLUDES = -I$(top_srcdir)/libebs

libeag_la_LDFLAGS = -version-info 2:3:0

libeag_la_DEPENDECIES = $(top_builddir)/libebs/libebs.la

libeag_la_CFLAGS = -Wall $(AM_CFLAGS)

