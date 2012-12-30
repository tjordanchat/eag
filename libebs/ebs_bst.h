/*
   File: ebs_bst.h
   Defines some global definitions

   CVS ID: "$Id: ebs_bst.h,v 1.4 2008/05/28 11:54:02 marcs Exp $"
*/
#ifndef IncEbsBst
#define IncEbsBst

/* Standard includes */
#include <stdio.h>
#include <string.h>

/* Define the string type */
typedef char *string;
#define string_nil ((string) NULL)
#define streq(s1,s2) (!strcmp(s1,s2))
#define strlt(s1,s2) (strcmp(s1,s2) < 0)

/* Define the real type and two cmp macros */
typedef double real;
#define cmp_int(a,b) ((a) < (b)?-1:(a) > (b)?1:0)
#define cmp_real(a,b) ((a) < (b)?-1:(a) > (b)?1:0)

/* Two utility macros */
#define MAXSTRNAME 1024
#define MAXFNAME 1024

/* Define support for saving */
void init_save (FILE *f);
void finish_save (FILE *f);
void save_char (FILE *f, char x);
void save_int (FILE *f, int x);
void save_real (FILE *f, real x);
void save_string (FILE *f, string x);

/* Define support for loading */
void init_load (FILE *f);
int finish_load (FILE *f);
int load_char (FILE *f, char *x);
int load_int (FILE *f, int *x);
int load_real (FILE *f, real *x);
int load_string (FILE *f, string *x);

#endif /* IncEbsBst */
