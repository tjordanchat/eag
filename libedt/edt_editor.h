/*
   File: edt_editor.h
   Defines some necessary constants

   CVS ID: "$Id: edt_editor.h,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/
#ifndef IncEdtEditor
#define IncEdtEditor

/* libebs includes */
#include <ebs_bst.h>

#define typed_open_symbol "<|"
#define typed_open_symbol_len 2
#define typed_close_symbol "|>"
#define typed_close_symbol_len 2
#define untyped_symbol "<|>"
#define untyped_symbol_len 3

extern int screenwidth;
extern int changed;
extern string Filename;

#endif /* IncEdtEditor */
