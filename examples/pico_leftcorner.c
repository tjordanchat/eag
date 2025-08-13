/*
   File: pico_leftcorner.c
   Generated on Tue Aug  5 14:26:52 2025
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

static void rule_assignation ();
static void get_assignation ();
static void red_assignation ();
static void rule_boolean ();
static void get_boolean ();
static void red_boolean ();
static void rule_declarations ();
static void get_declarations ();
static void red_declarations ();
static void rule_elsepart ();
static void get_elsepart ();
static void red_elsepart ();
static void empv_elsepart ();
static void emp_elsepart ();
static void rule_identifierlist ();
static void get_identifierlist ();
static void red_identifierlist ();
static void rule_expression ();
static void get_expression ();
static void red_expression ();
static void rule_factor ();
static void get_factor ();
static void red_factor ();
static void rule_identifier ();
static void get_identifier ();
static void red_identifier ();
static void rule_ifstatement ();
static void get_ifstatement ();
static void red_ifstatement ();
static void rule_letgits ();
static void get_letgits ();
static void red_letgits ();
static void empv_letgits ();
static void emp_letgits ();
static void rule_layout ();
static void get_layout ();
static void red_layout ();
static void empv_layout ();
static void emp_layout ();
static void rule_picoprogram ();
static void get_picoprogram ();
static void red_picoprogram ();
static void rule_number ();
static void get_number ();
static void red_number ();
static void rule_series ();
static void get_series ();
static void red_series ();
static void rule_statement ();
static void get_statement ();
static void red_statement ();
static void rule_term ();
static void get_term ();
static void red_term ();
static void rule_type ();
static void get_type ();
static void red_type ();
static void rule_whilestatement ();
static void get_whilestatement ();
static void red_whilestatement ();

string pico_name_from_nodenr (int nodenr)
	{ switch (nodenr)
	     { case 0: return ("root_node");
	       case 1: return ("leaf_node");
	       case 2: return ("rule_assignation");
	       case 3: return ("rule_boolean");
	       case 4: return ("rule_boolean");
	       case 5: return ("rule_declarations");
	       case 6: return ("rule_elsepart");
	       case 7: return ("rule_elsepart");
	       case 8: return ("rule_elsepart");
	       case 9: return ("rule_identifierlist");
	       case 10: return ("rule_identifierlist");
	       case 11: return ("rule_expression");
	       case 12: return ("rule_expression");
	       case 13: return ("rule_expression");
	       case 14: return ("rule_expression");
	       case 15: return ("rule_factor");
	       case 16: return ("rule_factor");
	       case 17: return ("rule_factor");
	       case 18: return ("rule_factor");
	       case 19: return ("rule_identifier");
	       case 20: return ("rule_ifstatement");
	       case 21: return ("rule_letgits");
	       case 22: return ("rule_letgits");
	       case 23: return ("rule_layout");
	       case 24: return ("rule_picoprogram");
	       case 25: return ("rule_number");
	       case 26: return ("rule_series");
	       case 27: return ("rule_series");
	       case 28: return ("rule_statement");
	       case 29: return ("rule_statement");
	       case 30: return ("rule_statement");
	       case 31: return ("rule_term");
	       case 32: return ("rule_term");
	       case 33: return ("rule_type");
	       case 34: return ("rule_type");
	       case 35: return ("rule_whilestatement");
	       default: panic ("strange node %d in pico", nodenr);
	     };
	  return (NULL);
	};

/* code goals for non predicate syntax rules */
#define goal_assignation 0
#define goal_boolean 1
#define goal_declarations 2
#define goal_elsepart 3
#define goal_identifierlist 4
#define goal_expression 5
#define goal_factor 6
#define goal_identifier 7
#define goal_ifstatement 8
#define goal_letgits 9
#define goal_layout 10
#define goal_picoprogram 11
#define goal_number 12
#define goal_series 13
#define goal_statement 14
#define goal_term 15
#define goal_type 16
#define goal_whilestatement 17
#define on_spine_to_goal(x) lc_rel[x * 18 + topi()]
static char lc_rel[] =
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 
	  0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
	  0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
	  1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
	  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 
	  };

static void rule_assignation ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_assignation);
	       pushq (get_assignation);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_assignation);
	}

static void get_assignation ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_letgits);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (19);
	  pushq (make_normal_node);
	  callq ();
	  pop (24);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  pushq (get_assignation);
	}

static void red_assignation ()
	{
	  if (topi () == goal_assignation)
	     { pop (1);
	       callq ();
	       pushi (goal_assignation);
	     };
	  if (on_spine_to_goal (goal_statement)){
	  if (in_lookahead ("EF;O"))
	  {
	  pushq (red_statement);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (28);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_assignation);
	}

static void rule_boolean ()
	{
	  if (in_lookahead ("TF"))
	     { pushi (goal_boolean);
	       pushq (get_boolean);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_boolean);
	}

static void get_boolean ()
	{
	  if (iptr_at ('T'))
	  {
	  pushq (red_boolean);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (3);
	  pushq (make_normal_node);
	  pushs ("TRUE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('F'))
	  {
	  pushq (red_boolean);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (4);
	  pushq (make_normal_node);
	  pushs ("FALSE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_boolean);
	}

static void red_boolean ()
	{
	  if (topi () == goal_boolean)
	     { pop (1);
	       callq ();
	       pushi (goal_boolean);
	     };
	  if (on_spine_to_goal (goal_factor)){
	  if (in_lookahead ("T=/+*)DEF;O"))
	  {
	  pushq (red_factor);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (18);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_boolean);
	}

static void rule_declarations ()
	{
	  if (iptr_at ('D'))
	     { pushi (goal_declarations);
	       pushq (get_declarations);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_declarations);
	}

static void get_declarations ()
	{
	  if (iptr_at ('D'))
	  {
	  pushq (red_declarations);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (";");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_identifierlist);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (5);
	  pushq (make_normal_node);
	  pushs ("DECLARE");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  pushq (get_declarations);
	}

static void red_declarations ()
	{
	  if (topi () == goal_declarations)
	     { pop (1);
	       callq ();
	       pushi (goal_declarations);
	     };
	  pushq (red_declarations);
	}

static void rule_elsepart ()
	{
	  if (iptr_at ('E'))
	     { pushi (goal_elsepart);
	       pushq (get_elsepart);
	       callq ();
	       pop(2);
	     };
	  if (iptr_at ('F'))
	     { pushq (empv_elsepart);
	       callq ();
	       pop(1);
	     };
	  pushq (rule_elsepart);
	}

static void empv_elsepart ()
	{
	  {
	  pushi (0);
	  pushi (0);
	  pushi (8);
	  pushq (make_normal_node);
	  callq ();
	  pop (4);
	  };
	  pushq (empv_elsepart);
	}

static void emp_elsepart ()
	{
	  {
	  pushi (0);
	  pushi (0);
	  pushi (8);
	  pushq (make_normal_node);
	  callq ();
	  pop (4);
	  };
	  pushq (emp_elsepart);
	}

static void get_elsepart ()
	{
	  if (iptr_at ('E'))
	  {
	  pushq (red_elsepart);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_series);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (2);
	  pushi (6);
	  pushq (make_normal_node);
	  pushs ("ELSE");
	  pushq (parse_terminal);
	  callq ();
	  pop (17);
	  };
	  if (iptr_at ('E'))
	  {
	  pushq (red_elsepart);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_elsepart);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_series);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("THEN");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (5);
	  pushi (7);
	  pushq (make_normal_node);
	  pushs ("ELIF");
	  pushq (parse_terminal);
	  callq ();
	  pop (34);
	  };
	  pushq (get_elsepart);
	}

static void red_elsepart ()
	{
	  if (topi () == goal_elsepart)
	     { pop (1);
	       callq ();
	       pushi (goal_elsepart);
	     };
	  pushq (red_elsepart);
	}

static void rule_identifierlist ()
	{
	  if (in_lookahead ("BI"))
	     { pushi (goal_identifierlist);
	       pushq (get_identifierlist);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_identifierlist);
	}

static void get_identifierlist ()
	{
	  if (iptr_at ('B'))
	  {
	  pushq (red_type);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (33);
	  pushq (make_normal_node);
	  pushs ("BOOL");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('I'))
	  {
	  pushq (red_type);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (34);
	  pushq (make_normal_node);
	  pushs ("INT");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_identifierlist);
	}

static void red_identifierlist ()
	{
	  if (topi () == goal_identifierlist)
	     { pop (1);
	       callq ();
	       pushi (goal_identifierlist);
	     };
	  if (on_spine_to_goal (goal_identifierlist)){
	  if (iptr_at (','))
	  {
	  pushq (red_identifierlist);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_type);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (",");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (4);
	  pushi (10);
	  pushq (make_normal_node);
	  callq ();
	  pop (27);
	  };
	  };
	  pushq (red_identifierlist);
	}

static void rule_expression ()
	{
	  if (in_lookahead ("(TFabcdefghijklmnopqrstuvwxyz0123456789"))
	     { pushi (goal_expression);
	       pushq (get_expression);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_expression);
	}

static void get_expression ()
	{
	  if (iptr_at ('T'))
	  {
	  pushq (red_boolean);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (3);
	  pushq (make_normal_node);
	  pushs ("TRUE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('F'))
	  {
	  pushq (red_boolean);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (4);
	  pushq (make_normal_node);
	  pushs ("FALSE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('('))
	  {
	  pushq (red_factor);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (")");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (15);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_letgits);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (19);
	  pushq (make_normal_node);
	  callq ();
	  pop (24);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_gen_4 = new_affix_node ("a_gen_4");
	  pushq (red_number);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_4);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (25);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_gen_4 -> val);
	  free_affix_node (a_gen_4);
	  };
	  pushq (get_expression);
	}

static void red_expression ()
	{
	  if (topi () == goal_expression)
	     { pop (1);
	       callq ();
	       pushi (goal_expression);
	     };
	  if (on_spine_to_goal (goal_expression)){
	  if (iptr_at ('='))
	  {
	  pushq (red_expression);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("=");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (11);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_expression)){
	  if (iptr_at ('/'))
	  {
	  pushq (red_expression);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("/=");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (12);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_expression)){
	  if (iptr_at ('+'))
	  {
	  pushq (red_expression);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_term);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("+");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (13);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  pushq (red_expression);
	}

static void rule_factor ()
	{
	  if (in_lookahead ("(TFabcdefghijklmnopqrstuvwxyz0123456789"))
	     { pushi (goal_factor);
	       pushq (get_factor);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_factor);
	}

static void get_factor ()
	{
	  if (iptr_at ('T'))
	  {
	  pushq (red_boolean);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (3);
	  pushq (make_normal_node);
	  pushs ("TRUE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('F'))
	  {
	  pushq (red_boolean);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (4);
	  pushq (make_normal_node);
	  pushs ("FALSE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('('))
	  {
	  pushq (red_factor);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (")");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (15);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_letgits);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (19);
	  pushq (make_normal_node);
	  callq ();
	  pop (24);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_gen_4 = new_affix_node ("a_gen_4");
	  pushq (red_number);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_4);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (25);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_gen_4 -> val);
	  free_affix_node (a_gen_4);
	  };
	  pushq (get_factor);
	}

static void red_factor ()
	{
	  if (topi () == goal_factor)
	     { pop (1);
	       callq ();
	       pushi (goal_factor);
	     };
	  if (on_spine_to_goal (goal_term)){
	  if (in_lookahead ("T=/+*)DEF;O"))
	  {
	  pushq (red_term);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (32);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_factor);
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
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_letgits);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (19);
	  pushq (make_normal_node);
	  callq ();
	  pop (24);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
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
	  if (on_spine_to_goal (goal_assignation)){
	  if (iptr_at (':'))
	  {
	  pushq (red_assignation);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (":=");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (2);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_factor)){
	  if (in_lookahead ("T=/+*)DEF;O"))
	  {
	  pushq (red_factor);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (16);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_identifier);
	}

static void rule_ifstatement ()
	{
	  if (iptr_at ('I'))
	     { pushi (goal_ifstatement);
	       pushq (get_ifstatement);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_ifstatement);
	}

static void get_ifstatement ()
	{
	  if (iptr_at ('I'))
	  {
	  pushq (red_ifstatement);
	  pushi (6);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("FI");
	  pushq (parse_terminal);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_elsepart);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_series);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("THEN");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (6);
	  pushi (20);
	  pushq (make_normal_node);
	  pushs ("IF");
	  pushq (parse_terminal);
	  callq ();
	  pop (41);
	  };
	  pushq (get_ifstatement);
	}

static void red_ifstatement ()
	{
	  if (topi () == goal_ifstatement)
	     { pop (1);
	       callq ();
	       pushi (goal_ifstatement);
	     };
	  if (on_spine_to_goal (goal_statement)){
	  if (in_lookahead ("EF;O"))
	  {
	  pushq (red_statement);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (29);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_ifstatement);
	}

static void rule_letgits ()
	{
	  if (in_lookahead (" abcdefghijklmnopqrstuvwxyz0123456789"))
	     { pushi (goal_letgits);
	       pushq (get_letgits);
	       callq ();
	       pop(2);
	     };
	  if (in_lookahead (" \t\n:;,T=/+*)DEFO"))
	     { pushq (empv_letgits);
	       callq ();
	       pop(1);
	     };
	  pushq (rule_letgits);
	}

static void empv_letgits ()
	{
	  {
	  pushi (0);
	  pushi (0);
	  pushi (22);
	  pushq (make_normal_node);
	  callq ();
	  pop (4);
	  };
	  pushq (empv_letgits);
	}

static void emp_letgits ()
	{
	  {
	  pushi (0);
	  pushi (0);
	  pushi (22);
	  pushq (make_normal_node);
	  callq ();
	  pop (4);
	  };
	  pushq (emp_letgits);
	}

static void get_letgits ()
	{
	  if (iptr_at (' '))
	  {
	  affix_node a_gen_2 = new_affix_node ("a_gen_2");
	  affix_node a_gen_1 = new_affix_node ("a_gen_1");
	  pushq (red_letgits);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_letgits);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_gen_2);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_1);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs (" ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (3);
	  pushi (21);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_gen_2 -> val);
	  free_affix_node (a_gen_2);
	  rfre_value (a_gen_1 -> val);
	  free_affix_node (a_gen_1);
	  };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz0123456789"))
	  {
	  affix_node a_gen_2 = new_affix_node ("a_gen_2");
	  affix_node a_gen_1 = new_affix_node ("a_gen_1");
	  pushq (red_letgits);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_letgits);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_1);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushq (make_empty_strstore);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_gen_2);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (3);
	  pushi (21);
	  pushq (make_normal_node);
	  callq ();
	  pop (27);
	  rfre_value (a_gen_2 -> val);
	  free_affix_node (a_gen_2);
	  rfre_value (a_gen_1 -> val);
	  free_affix_node (a_gen_1);
	  };
	  pushq (get_letgits);
	}

static void red_letgits ()
	{
	  if (topi () == goal_letgits)
	     { pop (1);
	       callq ();
	       pushi (goal_letgits);
	     };
	  pushq (red_letgits);
	}

static void rule_layout ()
	{
	  if (in_lookahead (" \t\n"))
	     { pushi (goal_layout);
	       pushq (get_layout);
	       callq ();
	       pop(2);
	     };
	  if (in_lookahead ("(TFabcdefghijklmnopqrstuvwxyz0123456789BIW:;,D\204E=/\
+*O)"))
	     { pushq (empv_layout);
	       callq ();
	       pop(1);
	     };
	  pushq (rule_layout);
	}

static void empv_layout ()
	{
	  {
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushq (make_empty_strstore);
	  pushs (" \t\n");
	  pushq (fail_if_iptr_at_set);
	  pushi (0);
	  pushi (1);
	  pushi (23);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  };
	  pushq (empv_layout);
	}

static void emp_layout ()
	{
	  {
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushq (make_empty_strstore);
	  pushi (0);
	  pushi (1);
	  pushi (23);
	  pushq (make_normal_node);
	  callq ();
	  pop (12);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  };
	  pushq (emp_layout);
	}

static void get_layout ()
	{
	  if (in_lookahead (" \t\n"))
	  {
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  pushq (red_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs (" \t\n");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (1);
	  pushi (23);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
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
	  if (on_spine_to_goal (goal_picoprogram)){
	  if (iptr_at ('B'))
	  {
	  pushq (red_picoprogram);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("END");
	  pushq (parse_terminal);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_series);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_declarations);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("BEGIN");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (5);
	  pushi (24);
	  pushq (make_normal_node);
	  callq ();
	  pop (34);
	  };
	  };
	  pushq (red_layout);
	}

static void rule_picoprogram ()
	{
	  if (in_lookahead (" \t\nB"))
	     { pushi (goal_picoprogram);
	       pushq (get_picoprogram);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_picoprogram);
	}

static void get_picoprogram ()
	{
	  if (in_lookahead (" \t\n"))
	  {
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  pushq (red_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs (" \t\n");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (1);
	  pushi (23);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  };
	  if (iptr_at ('B'))
	  {
	  pushq (red_picoprogram);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("END");
	  pushq (parse_terminal);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_series);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_declarations);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (emp_layout);
	  pushi (0);
	  pushi (5);
	  pushi (24);
	  pushq (make_normal_node);
	  pushs ("BEGIN");
	  pushq (parse_terminal);
	  callq ();
	  pop (34);
	  };
	  pushq (get_picoprogram);
	}

static void red_picoprogram ()
	{
	  if (topi () == goal_picoprogram)
	     { pop (1);
	       callq ();
	       pushi (goal_picoprogram);
	     };
	  pushq (red_picoprogram);
	}

static void rule_number ()
	{
	  if (in_lookahead ("0123456789"))
	     { pushi (goal_number);
	       pushq (get_number);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_number);
	}

static void get_number ()
	{
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_gen_4 = new_affix_node ("a_gen_4");
	  pushq (red_number);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_4);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (25);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_gen_4 -> val);
	  free_affix_node (a_gen_4);
	  };
	  pushq (get_number);
	}

static void red_number ()
	{
	  if (topi () == goal_number)
	     { pop (1);
	       callq ();
	       pushi (goal_number);
	     };
	  if (on_spine_to_goal (goal_factor)){
	  if (in_lookahead ("T=/+*)DEF;O"))
	  {
	  pushq (red_factor);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (17);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_number);
	}

static void rule_series ()
	{
	  if (in_lookahead ("IabcdefghijklmnopqrstuvwxyzW"))
	     { pushi (goal_series);
	       pushq (get_series);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_series);
	}

static void get_series ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_letgits);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (19);
	  pushq (make_normal_node);
	  callq ();
	  pop (24);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  if (iptr_at ('I'))
	  {
	  pushq (red_ifstatement);
	  pushi (6);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("FI");
	  pushq (parse_terminal);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_elsepart);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_series);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("THEN");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (6);
	  pushi (20);
	  pushq (make_normal_node);
	  pushs ("IF");
	  pushq (parse_terminal);
	  callq ();
	  pop (41);
	  };
	  if (iptr_at ('W'))
	  {
	  pushq (red_whilestatement);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("OD");
	  pushq (parse_terminal);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_series);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("DO");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (5);
	  pushi (35);
	  pushq (make_normal_node);
	  pushs ("WHILE");
	  pushq (parse_terminal);
	  callq ();
	  pop (36);
	  };
	  pushq (get_series);
	}

static void red_series ()
	{
	  if (topi () == goal_series)
	     { pop (1);
	       callq ();
	       pushi (goal_series);
	     };
	  if (on_spine_to_goal (goal_series)){
	  if (iptr_at (';'))
	  {
	  pushq (red_series);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_statement);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (";");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (27);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  pushq (red_series);
	}

static void rule_statement ()
	{
	  if (in_lookahead ("IabcdefghijklmnopqrstuvwxyzW"))
	     { pushi (goal_statement);
	       pushq (get_statement);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_statement);
	}

static void get_statement ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_letgits);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (19);
	  pushq (make_normal_node);
	  callq ();
	  pop (24);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  if (iptr_at ('I'))
	  {
	  pushq (red_ifstatement);
	  pushi (6);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("FI");
	  pushq (parse_terminal);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_elsepart);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_series);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("THEN");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (6);
	  pushi (20);
	  pushq (make_normal_node);
	  pushs ("IF");
	  pushq (parse_terminal);
	  callq ();
	  pop (41);
	  };
	  if (iptr_at ('W'))
	  {
	  pushq (red_whilestatement);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("OD");
	  pushq (parse_terminal);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_series);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("DO");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (5);
	  pushi (35);
	  pushq (make_normal_node);
	  pushs ("WHILE");
	  pushq (parse_terminal);
	  callq ();
	  pop (36);
	  };
	  pushq (get_statement);
	}

static void red_statement ()
	{
	  if (topi () == goal_statement)
	     { pop (1);
	       callq ();
	       pushi (goal_statement);
	     };
	  if (on_spine_to_goal (goal_series)){
	  if (in_lookahead ("EF;O"))
	  {
	  pushq (red_series);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (26);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_statement);
	}

static void rule_term ()
	{
	  if (in_lookahead ("(TFabcdefghijklmnopqrstuvwxyz0123456789"))
	     { pushi (goal_term);
	       pushq (get_term);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_term);
	}

static void get_term ()
	{
	  if (iptr_at ('T'))
	  {
	  pushq (red_boolean);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (3);
	  pushq (make_normal_node);
	  pushs ("TRUE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('F'))
	  {
	  pushq (red_boolean);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (4);
	  pushq (make_normal_node);
	  pushs ("FALSE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('('))
	  {
	  pushq (red_factor);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (")");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (15);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_letgits);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (19);
	  pushq (make_normal_node);
	  callq ();
	  pop (24);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_gen_4 = new_affix_node ("a_gen_4");
	  pushq (red_number);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_4);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (25);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_gen_4 -> val);
	  free_affix_node (a_gen_4);
	  };
	  pushq (get_term);
	}

static void red_term ()
	{
	  if (topi () == goal_term)
	     { pop (1);
	       callq ();
	       pushi (goal_term);
	     };
	  if (on_spine_to_goal (goal_expression)){
	  if (in_lookahead ("T=/+)DEF;O"))
	  {
	  pushq (red_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (14);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  if (on_spine_to_goal (goal_term)){
	  if (iptr_at ('*'))
	  {
	  pushq (red_term);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_factor);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("*");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (31);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  pushq (red_term);
	}

static void rule_type ()
	{
	  if (in_lookahead ("BI"))
	     { pushi (goal_type);
	       pushq (get_type);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_type);
	}

static void get_type ()
	{
	  if (iptr_at ('B'))
	  {
	  pushq (red_type);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (33);
	  pushq (make_normal_node);
	  pushs ("BOOL");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('I'))
	  {
	  pushq (red_type);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (34);
	  pushq (make_normal_node);
	  pushs ("INT");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_type);
	}

static void red_type ()
	{
	  if (topi () == goal_type)
	     { pop (1);
	       callq ();
	       pushi (goal_type);
	     };
	  if (on_spine_to_goal (goal_identifierlist)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  pushq (red_identifierlist);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_identifier);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (9);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  pushq (red_type);
	}

static void rule_whilestatement ()
	{
	  if (iptr_at ('W'))
	     { pushi (goal_whilestatement);
	       pushq (get_whilestatement);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_whilestatement);
	}

static void get_whilestatement ()
	{
	  if (iptr_at ('W'))
	  {
	  pushq (red_whilestatement);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("OD");
	  pushq (parse_terminal);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_series);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("DO");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (5);
	  pushi (35);
	  pushq (make_normal_node);
	  pushs ("WHILE");
	  pushq (parse_terminal);
	  callq ();
	  pop (36);
	  };
	  pushq (get_whilestatement);
	}

static void red_whilestatement ()
	{
	  if (topi () == goal_whilestatement)
	     { pop (1);
	       callq ();
	       pushi (goal_whilestatement);
	     };
	  if (on_spine_to_goal (goal_statement)){
	  if (in_lookahead ("EF;O"))
	  {
	  pushq (red_statement);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (30);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_whilestatement);
	}

void transduce ()
	{
	  pushq (dummy_continuation);
	  pushq (increment_nrparses);
	  pushq (endofsentence);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_picoprogram);
	  pushi (0);
	  pushi (1);	/* one son only */
	  pushi (0);	/* the rootnode */
	  pushq (make_normal_node);
	  init_eag_stddefs (1);
	  callq ();
	  pop (11);
	};

string module_name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return ("pico");
	       case 1: return ("eag_stddefs");
	       default: panic ("strange module nr %d", mnr);
	     };
	  return (NULL);
	};

char *name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return (pico_name_from_nodenr (nodenr));
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

