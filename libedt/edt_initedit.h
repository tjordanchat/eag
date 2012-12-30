/*
   File: edt_initedit.h
   initializes the transducer or recognizer routines

   CVS ID: "$Id: edt_initedit.h,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/
#ifndef IncEdtInitEdit
#define IncEdtInitEdit

/* Exported initialization code */
int init_editor (char *name, int *argc, char **argv);
void initial_transduce_and_unparse ();
void start_editor ();

#endif /* IncEdtInitEdit */
