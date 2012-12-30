/*
   File: ebs_lexds.h
   Defines the datastructures necessary to store a lexicon

   CVS ID: "$Id: ebs_lexds.h,v 1.4 2008/05/28 14:53:57 marcs Exp $"
*/
#ifndef IncEbsLexDs
#define IncEbsLexDs

/* local includes */
#include "ebs_bst.h"
#include "ebs_cst.h"
#include "ebs_value.h"

/* Introduce record type definitions */
typedef struct lexend_rec *lexend;
typedef struct lextree_rec *lextree;
typedef struct lexicon_rec *lexicon;

/* Introduce list definitions */
typedef struct lexend_list_rec *lexend_list;
typedef struct lextree_list_rec *lextree_list;

/* Introduce nil pointers for records */
#define lexend_nil ((lexend) NULL)
#define lextree_nil ((lextree) NULL)
#define lexicon_nil ((lexicon) NULL)

/* Introduce nil pointers for lists */
#define lexend_list_nil ((lexend_list) NULL)
#define lextree_list_nil ((lextree_list) NULL)

/* Introduce structure of record type definitions */
struct lexend_rec 
{ int nodenr;
  int_list valuenrs;
};

struct lextree_rec
{ string remainder;
  lexend_list prods;
  lextree_list tails;
};

struct lexicon_rec
{ int_list lex_lnames;
  value_list lex_vals;
  lextree_list lex_trees;
};

/* Introduce structure of value lists */
struct lexend_list_rec
{ int size;
  int room;
  lexend *array;
};

struct lextree_list_rec
{ int size;
  int room;
  lextree *array;
};

/* lexend manipulation */
lexend new_lexend (int nodenr, int_list valuenrs);
lexend rdup_lexend (lexend old);
void rfre_lexend (lexend old);
void save_lexend (FILE *f, lexend l);
int load_lexend (FILE *f, lexend *l);

/* lextree manipulation */
lextree new_lextree (string remainder, lexend_list prods, lextree_list tail);
lextree rdup_lextree (lextree old);
void rfre_lextree (lextree old);
void save_lextree (FILE *f, lextree l);
int load_lextree (FILE *f, lextree *l);

/* lexicon manipulation */
lexicon new_lexicon (int_list lex_lnames, value_list lex_vals, lextree_list lex_trees);
lexicon rdup_lexicon (lexicon old);
void rfre_lexicon (lexicon old);
void save_lexicon (FILE *f, lexicon l);
int load_lexicon (FILE *f, lexicon *l, string **lnames);
void csave_lexicon (string fname, lexicon l);
void cload_lexicon (string fname, lexicon *l, string **lnames);

/* lexend list manipulation */
lexend_list new_lexend_list ();
void room_lexend_list (lexend_list ll, int room);
void app_lexend_list (lexend_list ll, lexend s);
lexend_list rdup_lexend_list (lexend_list old);
void rfre_lexend_list (lexend_list old);
void save_lexend_list (FILE *f, lexend_list ll);
int load_lexend_list (FILE *f, lexend_list *ll);

/* lextree list manipulation */
lextree_list new_lextree_list (int room);
void room_lextree_list (lextree_list ll, int room);
void app_lextree_list (lextree_list ll, lextree l);
lextree_list rdup_lextree_list (lextree_list ll);
void rfre_lextree_list (lextree_list old);
void save_lextree_list (FILE *f, lextree_list ll);
int load_lextree_list (FILE *f, lextree_list *ll);

#endif /* IncEbsLexDs */
