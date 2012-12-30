## Process this file with automake to produce Makefile.in
docdir = $(prefix)/doc
doc_DATA = $(PACKAGE)-manual-$(VERSION).ps

EXTRA_DIST = book.ps \
	     abbrev.tex \
	     affixes.tex \
	     biblio.tex \
	     book.tex \
	     cfg.tex \
	     eaggrammar.tex \
	     editors.tex \
	     final.tex \
	     intro.tex \
	     meta.tex \
	     misc.tex \
	     pico.eps \
	     pico2.eps \
	     pico3.eps \
	     pico4.eps

$(doc_DATA): book.ps
	cp book.ps $(doc_DATA)

CLEANFILES = $(doc_DATA)
