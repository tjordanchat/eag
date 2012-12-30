##
## Process this file with automake to produce Makefile.in
##
## CVSID: "$Id: Makefile.am,v 1.5 2007/09/05 10:27:44 marcs Exp $"
##

bin_PROGRAMS = eag-compile eag-skel

if DO_EDT
bin_SCRIPTS = eagcc eagccl eaged eagedl
else
bin_SCRIPTS = eagcc eagccl
endif

eag_compile_SOURCES = codemeta.c codemeta.h \
                      common.c common.h \
                      constant.c constant.h \
                      eag_unparser.c eag_unparser.h \
                      empty.c empty.h \
                      flow.c flow.h \
                      gentemplates.c gentemplates.h \
                      gentree.c gentree.h \
                      ident.c ident.h \
                      lattice.c lattice.h \
                      layout.c layout.h \
                      leftcorner.c leftcorner.h \
                      lex.c lex.h \
                      lexicon.c lexicon.h \
                      limits.h \
                      lookahead.c lookahead.h \
                      lr.c lr.h \
                      main.c main.h \
                      numbering.c numbering.h \
                      parser.c parser.h \
                      placeholder.c placeholder.h \
                      prepare.c prepare.h \
                      recursion.c recursion.h \
                      reduce.c reduce.h \
                      rules.c rules.h \
                      topdown.c topdown.h \
                      tree.c tree.h \
                      typing.c typing.h \
                      warshall.c warshall.h

INCLUDES = -I$(top_srcdir)/libebs -I$(top_srcdir)/libeag \
           -I$(top_srcdir)/libedt -DPATH=\"${includedir}\" \
	   -DVERSION=\"${VERSION}\"

eag_compile_DEPENDENCIES = $(top_builddir)/libebs/libebs.la
eag_compile_LDADD = $(top_builddir)/libebs/libebs.la
eag_compile_CFLAGS = -Wall $(AM_CFLAGS)

eag_skel_SOURCES = skelmain.c \
                   codeskel.c codeskel.h \
                   gentree.c gentree.h \
                   lex.c lex.h \
                   parser.c parser.h \
                   numbering.c numbering.h \
                   tree.c tree.h

eag_skel_LDADD = $(top_builddir)/libebs/libebs.la
eag_skel_DEPENDENCIES = $(top_builddir)/libebs/libebs.la
eag_skel_CFLAGS = -Wall $(AM_CFLAGS)


eagcc: Makefile
	echo '#!/bin/sh' > $@
	echo '$(CC) -O -I$(includedir) -I. -c $$1.c' >> $@
	echo '$(CC) -o $$1 -L$(libdir) $$1.o $$2 $$3 $$4 $$5 -leag -lebs' >> $@
	chmod 755 $@

eagccl: Makefile
	echo '#!/bin/sh' > $@
	echo '$(CC) -o $$1 -L$(libdir) $$1.o $$2 $$3 $$4 $$5 -leag -lebs' >> $@
	chmod 755 $@

if DO_EDT
eaged: Makefile
	echo '#!/bin/sh' > $@
	echo '$(CC) -O -I$(includedir) -I. -c $$1.c' >> $@
	echo '$(CC) -o $$1 -L$(libdir) -L@X11LDIR@ $$1.o $$2 $$3 $$4 $$5 -ledt -lXedt -leag -lebs -lXaw -lXt -lX11 -lXmu -lXext -lSM -lICE -lm' >> $@
	chmod 755 $@

eagedl: Makefile
	echo '#!/bin/sh' > $@
	echo '$(CC) -o $$1 -L$(libdir) -L@X11LDIR@ $$1.o $$2 $$3 $$4 $$5 -ledt -lXedt -leag -lebs -lXaw -lXt -lX11 -lXmu -lXext -lSM -lICE -lm' >> $@
	chmod 755 $@
endif

if DO_EDT
CLEANFILES = eagedl eaged eagccl eagcc
else
CLEANFILES = eagccl eagcc
endif
