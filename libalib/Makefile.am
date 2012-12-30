##
## Process this file with automake to produce Makefile.in
##
## CVSID: "$Id: Makefile.am,v 1.3 2007/09/05 10:27:51 marcs Exp $"
##

lib_LTLIBRARIES = libalib.la

include_HEADERS = alib.eag

libalib_la_SOURCES = alib.c alib.h

libalib_la_LDFLAGS = -version-info 2:3:0

libalib_la_DEPENDENCIES = $(top_builddir)/libebs $(top_builddir)/libeag

libalib_la_CFLAGS = -Wall $(AM_CFLAGS)

INCLUDES = -I$(top_srcdir)/libebs -I$(top_srcdir)/libeag
