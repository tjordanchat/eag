/*
   File: dupl_leftcorner.c
   Generated on Tue Aug  5 14:08:35 2025
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_error.h>
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>

/* libeag includes */
#include <eag_ds.h>
#include <eag_textparsing.h>
#include <eag_buildtree.h>
#include <eag_propagate.h>
#include <eag_trace.h>
#include <eag_nodeinfo.h>
#include <eag_misc.h>
#include <eag_init.h>

/* Predefines includes */
#include <eag_stddefs.h>

/* forward declare meta rule routines */

static void rule_duplicate ();
static void get_duplicate ();
static void red_duplicate ();
static void rule_identifier ();
static void get_identifier ();
static void red_identifier ();

string dupl_name_from_nodenr (int nodenr)
	{ switch (nodenr)
	     { case 0: return ("root_node");
	       case 1: return ("leaf_node");
	       case 2: return ("rule_duplicate");
	       case 3: return ("rule_identifier");
	       default: panic ("strange node %d in dupl", nodenr);
	     };
	  return (NULL);
	};

/* code goals for non predicate syntax rules */
#define goal_duplicate 0
#define goal_identifier 1
#define on_spine_to_goal(x) lc_rel[x * 2 + topi()]
static char lc_rel[] =
	{ 1, 0, 
	  1, 1, 
	  };

static void rule_duplicate ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_duplicate);
	       pushq (get_duplicate);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_duplicate);
	}

static void get_duplicate ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_l = new_affix_node ("a_l");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs (" \t\n");
	  pushq (parse_set_star_strict);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_l);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pusha (a_ls);
	  pusha (a_l);
	  pushi (2);
	  pushi (tag_concat);
	  pushi (1);
	  pushi (3);
	  pushi (3);
	  pushq (make_normal_node);
	  callq ();
	  pop (36);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_l -> val);
	  free_affix_node (a_l);
	  };
	  pushq (get_duplicate);
	}

static void red_duplicate ()
	{
	  if (topi () == goal_duplicate)
	     { pop (1);
	       callq ();
	       pushi (goal_duplicate);
	     };
	  pushq (red_duplicate);
	}

static void rule_identifier ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_identifier);
	       pushq (get_identifier);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_identifier);
	}

static void get_identifier ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_l = new_affix_node ("a_l");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs (" \t\n");
	  pushq (parse_set_star_strict);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_l);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pusha (a_ls);
	  pusha (a_l);
	  pushi (2);
	  pushi (tag_concat);
	  pushi (1);
	  pushi (3);
	  pushi (3);
	  pushq (make_normal_node);
	  callq ();
	  pop (36);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_l -> val);
	  free_affix_node (a_l);
	  };
	  pushq (get_identifier);
	}

static void red_identifier ()
	{
	  if (topi () == goal_identifier)
	     { pop (1);
	       callq ();
	       pushi (goal_identifier);
	     };
	  if (on_spine_to_goal (goal_duplicate)){
	  if (iptr_at ('\204'))
	  {
	  value v0 = new_string_value ("\n");
	  affix_node a_gen_2 = value_to_affix ("a_gen_2", v0);
	  value v1 = new_string_value (" = ");
	  affix_node a_gen_1 = value_to_affix ("a_gen_1", v1);
	  value v2 = new_string_value (" + ");
	  affix_node a_gen_0 = value_to_affix ("a_gen_0", v2);
	  affix_node a_id = new_affix_node ("a_id");
	  pushq (red_duplicate);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_id);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_gen_2);
	  pusha (a_id);
	  pusha (a_id);
	  pusha (a_gen_1);
	  pusha (a_id);
	  pusha (a_gen_0);
	  pusha (a_id);
	  pushi (7);
	  pushi (tag_concat);
	  pushi (1);
	  pushi (1);
	  pushi (2);
	  pushq (make_normal_node);
	  callq ();
	  pop (21);
	  rfre_value (a_gen_2 -> val);
	  free_affix_node (a_gen_2);
	  rfre_value (a_gen_1 -> val);
	  free_affix_node (a_gen_1);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  rfre_value (a_id -> val);
	  free_affix_node (a_id);
	  };
	  };
	  pushq (red_identifier);
	}

void transduce ()
	{
	  affix_node a_startrule_0 = new_affix_node ("a_startrule_0");
	  pushq (dummy_continuation);
	  pushq (increment_nrparses);
	  pusha (a_startrule_0);
	  pushq (output_affix);
	  pushq (endofsentence);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_startrule_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (rule_duplicate);
	  pusha (a_startrule_0);
	  pushi (tag_single);
	  pushi (1);
	  pushi (1);	/* one son only */
	  pushi (0);	/* the rootnode */
	  pushq (make_normal_node);
	  init_eag_stddefs (1);
	  callq ();
	  pop (17);
	  rfre_value (a_startrule_0 -> val);
	  free_affix_node (a_startrule_0);
	};

string module_name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return ("dupl");
	       case 1: return ("eag_stddefs");
	       default: panic ("strange module nr %d", mnr);
	     };
	  return (NULL);
	};

char *name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return (dupl_name_from_nodenr (nodenr));
	       case 1: return (eag_stddefs_name_from_nodenr (nodenr));
	       default: panic ("strange module nr %d", mnr);
	     };
	  return (NULL);
	};

void finish_parsing ()
{ complain_on_found_parses ();
  exit(0);
};

int main (int argc, char **argv)
	{ int status;
	  string fname;
	  int usize;
	  init_error ();
	  status = init_transducer (argc, argv, 0, &usize, &fname);
	  transduce ();
	  complain_on_found_parses ();
	  return (0);
	};

