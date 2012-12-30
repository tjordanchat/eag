/*
   File: edt_lrules.h
   Handles the storage, reading, writing and updating of the unparsing rules

   CVS ID: "$Id: edt_lrules.h,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/
#ifndef IncEdtLRules
#define IncEdtLRules

#define type_terminal 1
#define type_forcednonterminal 2
#define type_nonterminal 3
#define type_set 4

/*
   vert_offset registers the vertical offset to the end position
   of the previous tuple; hor_offset registers the horizontal
   offset to the end position of the previous tuple or the
   position of the tuple to which it is related.
*/
typedef struct tuple_rec
	{ int type;
	  char *text;
	  int relates;
	  int hor_offset;
	  int vert_offset;
	  int sonnr;
	  struct tuple_rec *next;
	} *tuple;
#define tuple_nil ((tuple) NULL)

typedef struct rule_rec
	{ tuple hrule;
	  tuple vrule;
	} *rule;
#define rule_nil ((rule) NULL)

/* Exported vars and code */
extern rule *ruletable;
extern int max_nr_of_rules;
extern int max_nr_of_tuples;
void init_layout_rules (char *gram);	/* grammar basename */

extern char *tuple_error_buffer;
extern char *tuple_edit_buffer;

#define max_rule_header "number of rules:"
#define max_tuple_header "number of tuples:"

extern int changed_rules;
void write_layout_rules ();

#define hor_rule 0
#define ver_rule 1
void write_rule_to_buffer (rule lrule, int dir);
int try_and_replace_rule (rule lrule, int dir);

#endif /* IncEdtLRules */
