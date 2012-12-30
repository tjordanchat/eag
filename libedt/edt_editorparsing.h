/*
   File: edt_editorparsing.h
   Defines all routines necessary for reparsing and handling error nodes

   CVS ID: "$Id: edt_editorparsing.h,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/
#ifndef IncEdtEditorParsing
#define IncEdtEditorParsing

/* Exported parsing code */
void initial_transduce_and_unparse (int status, char *init_buf);
void reparse ();
void replace_focus_by_text (char *s);
void save_unparse_buffer (char *sname);
void load_parsebuffer (char *sname);

#endif /* IncEdtEditorParsing */
