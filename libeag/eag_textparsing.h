/*
   File: eag_textparsing.h
   Defines the routines to parse terminals and terminal sets for recursive backup parsing.

   CVS ID: "$Id: eag_textparsing.h,v 1.5 2008/06/18 13:59:18 marcs Exp $"
*/
#ifndef IncEagTextparsing
#define IncEagTextparsing

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* Exported global variables */
extern int nrofparses;
extern char *parsebuffer;
extern char *iptr;
extern char *miptr;
extern int mtotal;
extern char *eptr;
extern char pemsg[];

/* Exported support routines */
void lookahead_failure (char *name);
void copy_into_parsebuffer (char *src, int len, int col);
void copy_file_into_parsebuffer (FILE *fd);
int in_set (char *ptr, char *set);
#define in_lookahead(s) in_set(iptr,s)
#define iptr_at(c) ((*iptr) == c)
void endofsentence ();
int currentcolumn ();
int currentrow ();

/* routines for parsing terminals and semiterminals */
void parse_terminal ();
void parse_set ();
void parse_non_set ();
void parse_set_star ();
void parse_non_set_star ();
void parse_set_plus ();
void parse_non_set_plus ();
void parse_set_star_strict ();
void parse_non_set_star_strict ();
void parse_set_plus_strict ();
void parse_non_set_plus_strict ();

/* routines for parsing with lexica */
void do_lexicon_rule (lexicon the_lexicon, int treenr);
void do_lexicon_get (lexicon the_lexicon, int treenr);

/* miscellaneous parse routines */
void fail_if_iptr_at_set ();
void fail_if_iptr_not_at_set ();
void increment_nrparses ();
void init_textparsing (int pbuffer_size, int max);
void reinit_textparsing ();
void complain_on_found_parses ();
void dump_matches ();

/* routines for recognizing meta rules */
void rec_parse_terminal ();
void rec_parse_integer ();
void rec_parse_real ();
void rec_parse_set ();
void rec_parse_non_set ();
void rec_parse_set_star ();
void rec_parse_non_set_star ();
void rec_parse_set_plus ();
void rec_parse_non_set_plus ();
void rec_parse_set_star_strict ();
void rec_parse_non_set_star_strict ();
void rec_parse_set_plus_strict ();
void rec_parse_non_set_plus_strict ();

/* routines for recognizing meta rules through recursive backup parsing */
void meta_endofsentence ();
void meta_endofinteger ();
void recbup_parse_terminal ();
void recbup_parse_integer ();
void recbup_parse_set ();
void recbup_parse_non_set ();
void recbup_parse_set_star ();
void recbup_parse_non_set_star ();
void recbup_parse_set_plus ();
void recbup_parse_non_set_plus ();
void recbup_parse_set_star_strict ();
void recbup_parse_non_set_star_strict ();
void recbup_parse_set_plus_strict ();
void recbup_parse_non_set_plus_strict ();

#endif /* IncEagTextParsing */
