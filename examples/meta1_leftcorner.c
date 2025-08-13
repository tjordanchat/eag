/*
   File: meta1_leftcorner.c
   Generated on Tue Aug  5 14:20:51 2025
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
static affix_node make_empty_affix ();
static affix_node make_nlcr_affix ();
static affix_node make_quote_affix ();
static affix_node make_preamble_affix ();
static affix_node make_postamble_affix ();
static affix_node make_printheader_affix ();
static affix_node make_printtrailer_affix ();
static affix_node make_tab_affix ();

static affix_node make_empty_affix ()
	{
	  value v1 = new_string_value ("");
	  return (value_to_affix ("meta_empty", v1));
	};

static affix_node make_nlcr_affix ()
	{
	  value v1 = new_string_value ("\n");
	  return (value_to_affix ("meta_nlcr", v1));
	};

static affix_node make_quote_affix ()
	{
	  value v1 = new_string_value ("\"");
	  return (value_to_affix ("meta_quote", v1));
	};

static affix_node make_preamble_affix ()
	{
	  value v1 = new_string_value ("#include <stdio.h>\n\nmain ()\n\t{ ");
	  return (value_to_affix ("meta_preamble", v1));
	};

static affix_node make_postamble_affix ()
	{
	  value v1 = new_string_value ("exit (0);\n\t}\n");
	  return (value_to_affix ("meta_postamble", v1));
	};

static affix_node make_printheader_affix ()
	{
	  value v1 = new_string_value ("printf (\"");
	  return (value_to_affix ("meta_printheader", v1));
	};

static affix_node make_printtrailer_affix ()
	{
	  value v1 = new_string_value ("\\n\");\n\t  ");
	  return (value_to_affix ("meta_printtrailer", v1));
	};

static affix_node make_tab_affix ()
	{
	  value v1 = new_string_value ("\t");
	  return (value_to_affix ("meta_tab", v1));
	};

static void rule_identifiers ();
static void get_identifiers ();
static void red_identifiers ();
static void empv_identifiers ();
static void emp_identifiers ();
static void rule_identifier ();
static void get_identifier ();
static void red_identifier ();
static void rule_layout ();
static void get_layout ();
static void red_layout ();
static void empv_layout ();
static void emp_layout ();
static void rule_program ();
static void get_program ();
static void red_program ();
static void empv_program ();
static void emp_program ();

string meta1_name_from_nodenr (int nodenr)
	{ switch (nodenr)
	     { case 0: return ("root_node");
	       case 1: return ("leaf_node");
	       case 2: return ("rule_identifiers");
	       case 3: return ("rule_identifiers");
	       case 4: return ("rule_identifier");
	       case 5: return ("rule_layout");
	       case 6: return ("rule_program");
	       default: panic ("strange node %d in meta1", nodenr);
	     };
	  return (NULL);
	};

/* code goals for non predicate syntax rules */
#define goal_identifiers 0
#define goal_identifier 1
#define goal_layout 2
#define goal_program 3
#define on_spine_to_goal(x) lc_rel[x * 4 + topi()]
static char lc_rel[] =
	{ 1, 0, 0, 1, 
	  1, 1, 0, 1, 
	  0, 0, 1, 1, 
	  0, 0, 0, 1, 
	  };

static void rule_identifiers ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_identifiers);
	       pushq (get_identifiers);
	       callq ();
	       pop(2);
	     };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz\204"))
	     { pushq (empv_identifiers);
	       callq ();
	       pop(1);
	     };
	  pushq (rule_identifiers);
	}

static void empv_identifiers ()
	{
	  {
	  affix_node a_empty = make_empty_affix ();
	  pusha (a_empty);
	  pushi (tag_single);
	  pushi (1);
	  pushi (0);
	  pushi (3);
	  pushq (make_normal_node);
	  callq ();
	  pop (6);
	  rfre_value (a_empty -> val);
	  free_affix_node (a_empty);
	  };
	  pushq (empv_identifiers);
	}

static void emp_identifiers ()
	{
	  {
	  affix_node a_empty = make_empty_affix ();
	  pusha (a_empty);
	  pushi (tag_single);
	  pushi (1);
	  pushi (0);
	  pushi (3);
	  pushq (make_normal_node);
	  callq ();
	  pop (6);
	  rfre_value (a_empty -> val);
	  free_affix_node (a_empty);
	  };
	  pushq (emp_identifiers);
	}

static void get_identifiers ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_l = new_affix_node ("a_l");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
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
	  pushi (4);
	  pushq (make_normal_node);
	  callq ();
	  pop (32);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_l -> val);
	  free_affix_node (a_l);
	  };
	  pushq (get_identifiers);
	}

static void red_identifiers ()
	{
	  if (topi () == goal_identifiers)
	     { pop (1);
	       callq ();
	       pushi (goal_identifiers);
	     };
	  if (on_spine_to_goal (goal_identifiers)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_printtrailer = make_printtrailer_affix ();
	  affix_node a_id = new_affix_node ("a_id");
	  affix_node a_printheader = make_printheader_affix ();
	  affix_node a_prints = new_affix_node ("a_prints");
	  pushq (red_identifiers);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_id);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (rule_identifier);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_prints);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_printtrailer);
	  pusha (a_id);
	  pusha (a_printheader);
	  pusha (a_prints);
	  pushi (4);
	  pushi (tag_concat);
	  pushi (1);
	  pushi (2);
	  pushi (2);
	  pushq (make_normal_node);
	  callq ();
	  pop (25);
	  rfre_value (a_printtrailer -> val);
	  free_affix_node (a_printtrailer);
	  rfre_value (a_id -> val);
	  free_affix_node (a_id);
	  rfre_value (a_printheader -> val);
	  free_affix_node (a_printheader);
	  rfre_value (a_prints -> val);
	  free_affix_node (a_prints);
	  };
	  };
	  if (on_spine_to_goal (goal_program)){
	  if (iptr_at ('\204'))
	  {
	  affix_node a_postamble = make_postamble_affix ();
	  affix_node a_prints = new_affix_node ("a_prints");
	  affix_node a_preamble = make_preamble_affix ();
	  pushq (red_program);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (emp_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_prints);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_postamble);
	  pusha (a_prints);
	  pusha (a_preamble);
	  pushi (3);
	  pushi (tag_concat);
	  pushi (1);
	  pushi (2);
	  pushi (6);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  rfre_value (a_postamble -> val);
	  free_affix_node (a_postamble);
	  rfre_value (a_prints -> val);
	  free_affix_node (a_prints);
	  rfre_value (a_preamble -> val);
	  free_affix_node (a_preamble);
	  };
	  };
	  pushq (red_identifiers);
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
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_l = new_affix_node ("a_l");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
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
	  pushi (4);
	  pushq (make_normal_node);
	  callq ();
	  pop (32);
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
	  if (on_spine_to_goal (goal_identifiers)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz\204"))
	  {
	  affix_node a_printtrailer = make_printtrailer_affix ();
	  affix_node a_id = new_affix_node ("a_id");
	  affix_node a_printheader = make_printheader_affix ();
	  affix_node a_prints = new_affix_node ("a_prints");
	  pushq (red_identifiers);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_prints);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (emp_identifiers);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_id);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_printtrailer);
	  pusha (a_id);
	  pusha (a_printheader);
	  pusha (a_prints);
	  pushi (4);
	  pushi (tag_concat);
	  pushi (1);
	  pushi (2);
	  pushi (2);
	  pushq (make_normal_node);
	  callq ();
	  pop (25);
	  rfre_value (a_printtrailer -> val);
	  free_affix_node (a_printtrailer);
	  rfre_value (a_id -> val);
	  free_affix_node (a_id);
	  rfre_value (a_printheader -> val);
	  free_affix_node (a_printheader);
	  rfre_value (a_prints -> val);
	  free_affix_node (a_prints);
	  };
	  };
	  pushq (red_identifier);
	}

static void rule_layout ()
	{
	  if (in_lookahead (" \t\n"))
	     { pushi (goal_layout);
	       pushq (get_layout);
	       callq ();
	       pop(2);
	     };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz\204"))
	     { pushq (empv_layout);
	       callq ();
	       pop(1);
	     };
	  pushq (rule_layout);
	}

static void empv_layout ()
	{
	  {
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushq (make_empty_strstore);
	  pushs (" \t\n");
	  pushq (fail_if_iptr_at_set);
	  pushi (0);
	  pushi (1);
	  pushi (5);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  pushq (empv_layout);
	}

static void emp_layout ()
	{
	  {
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushq (make_empty_strstore);
	  pushi (0);
	  pushi (1);
	  pushi (5);
	  pushq (make_normal_node);
	  callq ();
	  pop (12);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  pushq (emp_layout);
	}

static void get_layout ()
	{
	  if (in_lookahead (" \t\n"))
	  {
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs (" \t\n");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (1);
	  pushi (5);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  pushq (get_layout);
	}

static void red_layout ()
	{
	  if (topi () == goal_layout)
	     { pop (1);
	       callq ();
	       pushi (goal_layout);
	     };
	  if (on_spine_to_goal (goal_program)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz\204"))
	  {
	  affix_node a_postamble = make_postamble_affix ();
	  affix_node a_prints = new_affix_node ("a_prints");
	  affix_node a_preamble = make_preamble_affix ();
	  pushq (red_program);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_prints);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (rule_identifiers);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_postamble);
	  pusha (a_prints);
	  pusha (a_preamble);
	  pushi (3);
	  pushi (tag_concat);
	  pushi (1);
	  pushi (2);
	  pushi (6);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  rfre_value (a_postamble -> val);
	  free_affix_node (a_postamble);
	  rfre_value (a_prints -> val);
	  free_affix_node (a_prints);
	  rfre_value (a_preamble -> val);
	  free_affix_node (a_preamble);
	  };
	  };
	  pushq (red_layout);
	}

static void rule_program ()
	{
	  if (in_lookahead (" \t\nabcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_program);
	       pushq (get_program);
	       callq ();
	       pop(2);
	     };
	  if (iptr_at ('\204'))
	     { pushq (empv_program);
	       callq ();
	       pop(1);
	     };
	  pushq (rule_program);
	}

static void empv_program ()
	{
	  {
	  affix_node a_postamble = make_postamble_affix ();
	  affix_node a_prints = new_affix_node ("a_prints");
	  affix_node a_preamble = make_preamble_affix ();
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (empv_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_prints);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (empv_identifiers);
	  pusha (a_postamble);
	  pusha (a_prints);
	  pusha (a_preamble);
	  pushi (3);
	  pushi (tag_concat);
	  pushi (1);
	  pushi (2);
	  pushi (6);
	  pushq (make_normal_node);
	  callq ();
	  pop (21);
	  rfre_value (a_postamble -> val);
	  free_affix_node (a_postamble);
	  rfre_value (a_prints -> val);
	  free_affix_node (a_prints);
	  rfre_value (a_preamble -> val);
	  free_affix_node (a_preamble);
	  };
	  pushq (empv_program);
	}

static void emp_program ()
	{
	  {
	  affix_node a_postamble = make_postamble_affix ();
	  affix_node a_prints = new_affix_node ("a_prints");
	  affix_node a_preamble = make_preamble_affix ();
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (emp_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_prints);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (emp_identifiers);
	  pusha (a_postamble);
	  pusha (a_prints);
	  pusha (a_preamble);
	  pushi (3);
	  pushi (tag_concat);
	  pushi (1);
	  pushi (2);
	  pushi (6);
	  pushq (make_normal_node);
	  callq ();
	  pop (21);
	  rfre_value (a_postamble -> val);
	  free_affix_node (a_postamble);
	  rfre_value (a_prints -> val);
	  free_affix_node (a_prints);
	  rfre_value (a_preamble -> val);
	  free_affix_node (a_preamble);
	  };
	  pushq (emp_program);
	}

static void get_program ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_l = new_affix_node ("a_l");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
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
	  pushi (4);
	  pushq (make_normal_node);
	  callq ();
	  pop (32);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_l -> val);
	  free_affix_node (a_l);
	  };
	  if (in_lookahead (" \t\n"))
	  {
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs (" \t\n");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (1);
	  pushi (5);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  pushq (get_program);
	}

static void red_program ()
	{
	  if (topi () == goal_program)
	     { pop (1);
	       callq ();
	       pushi (goal_program);
	     };
	  pushq (red_program);
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
	  pushq (rule_program);
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
	     { case 0: return ("meta1");
	       case 1: return ("eag_stddefs");
	       default: panic ("strange module nr %d", mnr);
	     };
	  return (NULL);
	};

char *name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return (meta1_name_from_nodenr (nodenr));
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

