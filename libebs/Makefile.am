##
## Process this file with automake to produce Makefile.in
##
## CVSID: "$Id: Makefile.am,v 1.6 2007/09/05 10:27:56 marcs Exp $"
##

lib_LTLIBRARIES = libebs.la

include_HEADERS = ebs_error.h \
		  ebs_memalloc.h \
		  ebs_textstorage.h \
		  ebs_bst.h \
		  ebs_cst.h \
		  ebs_value.h \
		  ebs_lexds.h

libebs_la_SOURCES = ebs_bst.c ebs_bst.h \
                    ebs_cst.c ebs_cst.h \
                    ebs_error.c ebs_error.h \
                    ebs_lexds.c ebs_lexds.h \
                    ebs_memalloc.c ebs_memalloc.h \
                    ebs_primio.c ebs_primio.h \
                    ebs_textstorage.c ebs_textstorage.h \
                    ebs_value.c ebs_value.h

libebs_la_LDFLAGS = -version-info 2:3:0

libebs_la_CFLAGS = -Wall $(AM_CFLAGS)
