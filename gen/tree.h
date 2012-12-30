/*
   File: tree.h
   Defines the internal representation of an EAG
  
   CVS ID: "$Id: tree.h,v 1.4 2008/06/18 12:26:02 marcs Exp $"
*/
#ifndef IncTree
#define IncTree

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* Introduce record type definitions */
typedef struct cset_rec *cset;			/* character sets */
typedef struct affix_rec *affix;		/* affixes */
typedef struct expr_rec *expr;			/* affix expressions */
typedef struct meta_alt_rec *meta_alt;		/* metarule alternatives */
typedef struct meta_rule_rec *meta_rule;	/* meta rules */
typedef struct pos_rec *pos;			/* positions */
typedef struct member_rec *member;		/* members */
typedef struct alt_rec *alt;			/* alternatives */
typedef struct rule_rec *rule;			/* syntax rules */
typedef struct element_def_rec *element_def;	/* lattice element def */
typedef struct element_set_rec *element_set;	/* set of lattice elements */

/* Introduce list definitions */
typedef struct affix_list_rec *affix_list;	/* affix lists */
typedef struct meta_alt_list_rec *meta_alt_list; /* meta alternative lists */
typedef struct pos_list_rec *pos_list;		/* position lists (displays) */
typedef struct member_list_rec *member_list;	/* member lists */
typedef struct alt_list_rec *alt_list;		/* alternative lists */
typedef struct element_def_list_rec *element_def_list; /* element def lists */
typedef struct element_set_list_rec *element_set_list; /* element set lists */

/* Introduce nil pointers for records */
#define cset_nil ((cset) NULL)
#define affix_nil ((affix) NULL)
#define expr_nil ((expr) NULL)			/* used as empty meta alt*/
#define meta_alt_nil ((meta_alt) NULL)
#define meta_rule_nil ((meta_rule) NULL)
#define pos_nil ((pos) NULL)
#define member_nil ((member) NULL)
#define alt_nil ((alt) NULL)
#define rule_nil ((rule) NULL)
#define element_def_nil ((element_def) NULL)
#define element_set_nil ((element_set) NULL)

/* Introduce nil pointers for lists */
#define affix_list_nil ((affix_list) NULL)
#define meta_alt_list_nil ((meta_alt_list) NULL)
#define pos_list_nil ((pos_list) NULL)
#define member_list_nil ((member_list) NULL)	/* used as empty alternative */
#define alt_list_nil ((alt_list) NULL)		/* used for predef rule group */
#define element_def_list_nil ((element_def_list) NULL)
#define element_set_list_nil ((element_set_list) NULL)

/* Introduce structure of character sets */
#define non 8
#define star 4
#define plus 2
#define strict 1
struct cset_rec
	{ int kind;
	  string str;
	};

/* Introduce structure of affixes */
#define tag_affix_variable 0
#define tag_affix_terminal 1
#define tag_affix_integer 2
#define tag_affix_real 3
#define tag_affix_semi 4
#define tag_affix_element 5
#define tag_affix_constant 6
struct affix_rec
	{ string name;			/* affix name, possibly anonymous */
	  int type;			/* type of affix */
	  affix next;			/* for local var list */
	  value val;			/* value of affix */
	  int ref_count;		/* reference count */
	  int reachable;		/* reachability */
	  int tag;
	  union
	     { struct variable_rec
		  { string nonterminal;
		    string suffix;
		    meta_rule def;
		  } var;		/* affix variables */
	       string str;		/* string constants */
	       int inum;		/* integer number constants */
	       real rnum;		/* real number constants */
	       cset semi;		/* affix filter */
	       struct element_rec
		  { string eltnm;	
		    element_def def;
		  } elt;		/* elements of a lattice */
	       expr cexpr;		/* evaluated expression */
	     } u;
	};

/* Introduce structure of affix lists */
struct affix_list_rec
	{ int size;
	  int room;
	  affix *array;
	};

/* Introduce structure of affix expressions */
#define tag_undefined 0
#define tag_single 1
#define tag_compos 2
#define tag_concat 3
#define tag_union 4
struct expr_rec
	{ int tag;
	  union
	     { affix single;		/* single value */
	       affix_list compos;	/* composition */
	       affix_list concat;	/* concatenation */
	       affix_list uni;		/* union */
	     } u;
	};

/* Introduce structure of metarule alternatives */
struct meta_alt_rec
	{ expr e;
	  int number;				/* used in coding */
	};

/* Introduce structure of metarule alternative lists */
struct meta_alt_list_rec
	{ int size;
	  int room;
	  meta_alt *array;
	};

#define unknown 0
#define undefined_meta_value 0
#define single_meta_value 1
#define recognizer_meta_value 2
#define error_meta_value 3
struct meta_rule_rec
	{ string nonterminal;
	  meta_alt_list alts;
	  int ext;
	  int type;
	  int kind;
	  int empty;
	  int number;
	  value mvalue;
	  int reachable;
	  element_def lat_repr;
	  int needs_rec;
	  int needs_recbup;
	};

/* Introduce structure of positions */
#define undefinedflow -1
#define noflow 0
#define inherited 1
#define derived 2
#define transient 3
#define errorflow 4
struct pos_rec
	{ int kind;
	  expr ex;
	  int type;
	  element_def lat_repr;
	};

/* Introduce structure of position lists, that is displays */
struct pos_list_rec
	{ int size;
	  int room;
	  pos *array;
	};

/* Introduce structure of members */
#define tag_call 0
#define tag_terminal 1
#define tag_semiterminal 2
#define tag_cut 3
struct member_rec 
	{ int empty;		/* emptyness */
	  int followlayout;	/* layout may be inserted after this member */
	  char *first;		/* first (member) */
	  int number;		/* member numbering */
	  int sonnr;
	  int tag;
	  union 
	     { struct call_rec
		  { string nonterminal;
		    pos_list display;
		    rule def;
		  } call;			/* nonterminal calls */
	       string terminal;			/* terminals */
	       struct semiterminal_rec
		  { cset set;
		    pos_list display;
	          } semi;			/* semiterminals */
	     } u;
	};

/* Introduce structure of member lists */
struct member_list_rec
	{ int size;
	  int room;
	  member *array;
	};

/* Introduce structure of alternatives */
struct alt_rec
	{ pos_list display;			/* lhs display */
	  member_list members;
	  int number;				/* to number all the alts */
	  int nodenr;				/* to number all the alts */
	  int nrsons;				/* number of sons with tree */
	  affix locals;				/* local affixes */
	  int empty;				/* may generate empty */
	  string first;				/* first set */
	  string director;			/* director set */
	  int rule_nr;				/* number of hyper_rule */
	};

/* Introduce structure of alternative lists */
struct alt_list_rec
	{ int size;
	  int room;
	  alt *array;
	};

/* Introduce structure of rules */
/* flags to indicate the kind of hyper_rule */
#define rule_undefined 0
#define rule_nonpredicate 1
#define rule_predicate 2
#define rule_semipredicate 4
#define may_produce_empty 1
#define never_produces_empty 2
#define always_produces_empty 3
struct rule_rec
	{ string nonterminal;
	  alt_list alts;
	  int kind;
	  int empty;
	  int lex;
	  int ext;
	  int number;
	  int placeholder;
	  int startswithlayout;
	  int endsinlayout;
	  string first;
	  string follow;
	  pos_list proto_display;
	  int reachable;
	  int has_sons;
	  int goal;
	  lextree rev_tree;
	};

/* Introduce the structure of element_defs */
struct element_def_rec
	{ string eltnm;
	  int number;
	  int eltnr;
	  element_set set;
	};

/* Introduce structure of element_def_lists */
struct element_def_list_rec
	{ int size;
	  int room;
	  element_def *array;
	};

/* Introduce structure of element_sets */
struct element_set_rec
	{ int number;
	  element_def_list defs;
	};

/* Introduce structure of element_set_lists */
struct element_set_list_rec
	{ int size;
	  int room;
	  element_set *array;
	};

/* Introduce the structure of the rule tree */
typedef struct rule_tree_rec
	{ rule node;				/* sorted by name */
	  struct rule_tree_rec *left;		/* left ptr */
	  struct rule_tree_rec *right;		/* right ptr */
	} *rule_tree;
#define rule_tree_nil ((rule_tree) NULL)

/* Introduce the structure of the meta tree */
typedef struct meta_tree_rec
	{ meta_rule node;			/* sorted by name */
	  struct meta_tree_rec *left;		/* left ptr */
	  struct meta_tree_rec *right;		/* right ptr */
	} *meta_tree;
#define meta_tree_nil ((meta_tree) NULL)

/* Introduce the structure of the element spec tree */
typedef struct element_tree_rec
	{ element_def node;			/* sorted by name */
	  struct element_tree_rec *left;	/* left ptr */
	  struct element_tree_rec *right;	/* right ptr */
	} *element_tree;
#define element_tree_nil ((element_tree) NULL)

/* Exported global vars */
extern rule_tree rule_root;
extern meta_tree meta_root;
extern element_tree element_root;
extern rule layout_rule;
extern member start_rule;
extern alt start_alt;

/* Exported code */
int enter_meta_rule (meta_rule node);
meta_rule lookup_meta_rule (string nont);
int enter_syntax_rule (rule node);
rule lookup_syntax_rule (string nont);
int enter_element (element_def node);
element_def lookup_element (string eltnm);
void init_tree ();

#endif /* IncTree */
