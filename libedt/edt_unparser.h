/*
   File: edt_unparser.h
   Unparses the folded syntax tree delivered by cpmerge
   according to the unparsing rules read by lrules

   CVS ID: "$Id: edt_unparser.h,v 1.3 2004/12/25 22:40:52 marcs Exp $"
*/
#ifndef IncEdtUnparser
#define IncEdtUnparser

/* Exported vars and unparser code */
extern int unparsed_width;
extern int unparsed_height;
extern char *unparse_buffer;
extern void unparse ();
void init_unparser (int ubuffer_size);

#endif /* IncEdtUnparser */
