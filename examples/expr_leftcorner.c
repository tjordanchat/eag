/*
   File: expr_leftcorner.c
   Generated on Mon Aug  4 15:02:15 2025
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

static void rule_dividesbysymbol ();
static void get_dividesbysymbol ();
static void red_dividesbysymbol ();
static void rule_expr ();
static void get_expr ();
static void red_expr ();
static void rule_factor ();
static void get_factor ();
static void red_factor ();
static void rule_identifier ();
static void get_identifier ();
static void red_identifier ();
static void rule_leftparenthesis ();
static void get_leftparenthesis ();
static void red_leftparenthesis ();
static void rule_layout ();
static void get_layout ();
static void red_layout ();
static void empv_layout ();
static void emp_layout ();
static void rule_minussymbol ();
static void get_minussymbol ();
static void red_minussymbol ();
static void rule_number ();
static void get_number ();
static void red_number ();
static void rule_plussymbol ();
static void get_plussymbol ();
static void red_plussymbol ();
static void rule_rightparenthesis ();
static void get_rightparenthesis ();
static void red_rightparenthesis ();
static void rule_start ();
static void get_start ();
static void red_start ();
static void rule_term ();
static void get_term ();
static void red_term ();
static void rule_unsignedfactor ();
static void get_unsignedfactor ();
static void red_unsignedfactor ();
static void rule_timessymbol ();
static void get_timessymbol ();
static void red_timessymbol ();

string expr_name_from_nodenr (int nodenr)
	{ switch (nodenr)
	     { case 0: return ("root_node");
	       case 1: return ("leaf_node");
	       case 2: return ("rule_dividesbysymbol");
	       case 3: return ("rule_expr");
	       case 4: return ("rule_expr");
	       case 5: return ("rule_expr");
	       case 6: return ("rule_factor");
	       case 7: return ("rule_factor");
	       case 8: return ("rule_factor");
	       case 9: return ("rule_identifier");
	       case 10: return ("rule_leftparenthesis");
	       case 11: return ("rule_layout");
	       case 12: return ("rule_minussymbol");
	       case 13: return ("rule_number");
	       case 14: return ("rule_plussymbol");
	       case 15: return ("rule_rightparenthesis");
	       case 16: return ("rule_start");
	       case 17: return ("rule_term");
	       case 18: return ("rule_term");
	       case 19: return ("rule_term");
	       case 20: return ("rule_unsignedfactor");
	       case 21: return ("rule_unsignedfactor");
	       case 22: return ("rule_unsignedfactor");
	       case 23: return ("rule_timessymbol");
	       default: panic ("strange node %d in expr", nodenr);
	     };
	  return (NULL);
	};

/* code goals for non predicate syntax rules */
#define goal_dividesbysymbol 0
#define goal_expr 1
#define goal_factor 2
#define goal_identifier 3
#define goal_leftparenthesis 4
#define goal_layout 5
#define goal_minussymbol 6
#define goal_number 7
#define goal_plussymbol 8
#define goal_rightparenthesis 9
#define goal_start 10
#define goal_term 11
#define goal_unsignedfactor 12
#define goal_timessymbol 13
#define on_spine_to_goal(x) lc_rel[x * 14 + topi()]
static char lc_rel[] =
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 
	  0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 
	  0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 
	  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 
	  0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 
	  0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 
	  0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 
	  };

static void rule_dividesbysymbol ()
	{
	  if (iptr_at ('/'))
	     { pushi (goal_dividesbysymbol);
	       pushq (get_dividesbysymbol);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_dividesbysymbol);
	}

static void get_dividesbysymbol ()
	{
	  if (iptr_at ('/'))
	  {
	  pushq (red_dividesbysymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (2);
	  pushq (make_normal_node);
	  pushs ("/");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_dividesbysymbol);
	}

static void red_dividesbysymbol ()
	{
	  if (topi () == goal_dividesbysymbol)
	     { pop (1);
	       callq ();
	       pushi (goal_dividesbysymbol);
	     };
	  pushq (red_dividesbysymbol);
	}

static void rule_expr ()
	{
	  if (in_lookahead ("-+abcdefghijklmnopqrstuvwxyz0123456789("))
	     { pushi (goal_expr);
	       pushq (get_expr);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_expr);
	}

static void get_expr ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_1 = new_affix_node ("a_gen_1");
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_gen_1);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
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
	  pushi (9);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_gen_1 -> val);
	  free_affix_node (a_gen_1);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  if (iptr_at ('('))
	  {
	  pushq (red_leftparenthesis);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (10);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('-'))
	  {
	  pushq (red_minussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (12);
	  pushq (make_normal_node);
	  pushs ("-");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  pushq (red_number);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (13);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  };
	  if (iptr_at ('+'))
	  {
	  pushq (red_plussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (14);
	  pushq (make_normal_node);
	  pushs ("+");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_expr);
	}

static void red_expr ()
	{
	  if (topi () == goal_expr)
	     { pop (1);
	       callq ();
	       pushi (goal_expr);
	     };
	  if (on_spine_to_goal (goal_expr)){
	  if (iptr_at ('+'))
	  {
	  pushq (red_expr);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_term);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_plussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (3);
	  pushq (make_normal_node);
	  callq ();
	  pop (20);
	  };
	  };
	  if (on_spine_to_goal (goal_expr)){
	  if (iptr_at ('-'))
	  {
	  pushq (red_expr);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_term);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_minussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (4);
	  pushq (make_normal_node);
	  callq ();
	  pop (20);
	  };
	  };
	  if (on_spine_to_goal (goal_start)){
	  if (iptr_at ('\204'))
	  {
	  pushq (red_start);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (emp_layout);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (16);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  pushq (red_expr);
	}

static void rule_factor ()
	{
	  if (in_lookahead ("-+abcdefghijklmnopqrstuvwxyz0123456789("))
	     { pushi (goal_factor);
	       pushq (get_factor);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_factor);
	}

static void get_factor ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_1 = new_affix_node ("a_gen_1");
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_gen_1);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
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
	  pushi (9);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_gen_1 -> val);
	  free_affix_node (a_gen_1);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  if (iptr_at ('('))
	  {
	  pushq (red_leftparenthesis);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (10);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('-'))
	  {
	  pushq (red_minussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (12);
	  pushq (make_normal_node);
	  pushs ("-");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  pushq (red_number);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (13);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  };
	  if (iptr_at ('+'))
	  {
	  pushq (red_plussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (14);
	  pushq (make_normal_node);
	  pushs ("+");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
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
	  if (in_lookahead ("+-*/\204)"))
	  {
	  pushq (red_term);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (19);
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
	  affix_node a_gen_1 = new_affix_node ("a_gen_1");
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_gen_1);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
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
	  pushi (9);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_gen_1 -> val);
	  free_affix_node (a_gen_1);
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
	  if (on_spine_to_goal (goal_unsignedfactor)){
	  if (in_lookahead ("+-*/\204)"))
	  {
	  pushq (red_unsignedfactor);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (20);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_identifier);
	}

static void rule_leftparenthesis ()
	{
	  if (iptr_at ('('))
	     { pushi (goal_leftparenthesis);
	       pushq (get_leftparenthesis);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_leftparenthesis);
	}

static void get_leftparenthesis ()
	{
	  if (iptr_at ('('))
	  {
	  pushq (red_leftparenthesis);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (10);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_leftparenthesis);
	}

static void red_leftparenthesis ()
	{
	  if (topi () == goal_leftparenthesis)
	     { pop (1);
	       callq ();
	       pushi (goal_leftparenthesis);
	     };
	  if (on_spine_to_goal (goal_unsignedfactor)){
	  if (in_lookahead ("-+abcdefghijklmnopqrstuvwxyz0123456789("))
	  {
	  pushq (red_unsignedfactor);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_rightparenthesis);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expr);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (22);
	  pushq (make_normal_node);
	  callq ();
	  pop (20);
	  };
	  };
	  pushq (red_leftparenthesis);
	}

static void rule_layout ()
	{
	  if (in_lookahead (" \n\t"))
	     { pushi (goal_layout);
	       pushq (get_layout);
	       callq ();
	       pop(2);
	     };
	  if (in_lookahead ("-+abcdefghijklmnopqrstuvwxyz0123456789(*/\204)"))
	     { pushq (empv_layout);
	       callq ();
	       pop(1);
	     };
	  pushq (rule_layout);
	}

static void empv_layout ()
	{
	  {
	  affix_node a_gen_2 = new_affix_node ("a_gen_2");
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_2);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushq (make_empty_strstore);
	  pushs (" \n\t");
	  pushq (fail_if_iptr_at_set);
	  pushi (0);
	  pushi (1);
	  pushi (11);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_gen_2 -> val);
	  free_affix_node (a_gen_2);
	  };
	  pushq (empv_layout);
	}

static void emp_layout ()
	{
	  {
	  affix_node a_gen_2 = new_affix_node ("a_gen_2");
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_2);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushq (make_empty_strstore);
	  pushi (0);
	  pushi (1);
	  pushi (11);
	  pushq (make_normal_node);
	  callq ();
	  pop (12);
	  rfre_value (a_gen_2 -> val);
	  free_affix_node (a_gen_2);
	  };
	  pushq (emp_layout);
	}

static void get_layout ()
	{
	  if (in_lookahead (" \n\t"))
	  {
	  affix_node a_gen_2 = new_affix_node ("a_gen_2");
	  pushq (red_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_2);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs (" \n\t");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (1);
	  pushi (11);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_gen_2 -> val);
	  free_affix_node (a_gen_2);
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
	  if (on_spine_to_goal (goal_start)){
	  if (in_lookahead ("-+abcdefghijklmnopqrstuvwxyz0123456789("))
	  {
	  pushq (red_start);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expr);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (16);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  pushq (red_layout);
	}

static void rule_minussymbol ()
	{
	  if (iptr_at ('-'))
	     { pushi (goal_minussymbol);
	       pushq (get_minussymbol);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_minussymbol);
	}

static void get_minussymbol ()
	{
	  if (iptr_at ('-'))
	  {
	  pushq (red_minussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (12);
	  pushq (make_normal_node);
	  pushs ("-");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_minussymbol);
	}

static void red_minussymbol ()
	{
	  if (topi () == goal_minussymbol)
	     { pop (1);
	       callq ();
	       pushi (goal_minussymbol);
	     };
	  if (on_spine_to_goal (goal_factor)){
	  if (in_lookahead ("-+abcdefghijklmnopqrstuvwxyz0123456789("))
	  {
	  pushq (red_factor);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_factor);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (6);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  pushq (red_minussymbol);
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
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  pushq (red_number);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (13);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
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
	  if (on_spine_to_goal (goal_unsignedfactor)){
	  if (in_lookahead ("+-*/\204)"))
	  {
	  pushq (red_unsignedfactor);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (21);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_number);
	}

static void rule_plussymbol ()
	{
	  if (iptr_at ('+'))
	     { pushi (goal_plussymbol);
	       pushq (get_plussymbol);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_plussymbol);
	}

static void get_plussymbol ()
	{
	  if (iptr_at ('+'))
	  {
	  pushq (red_plussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (14);
	  pushq (make_normal_node);
	  pushs ("+");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_plussymbol);
	}

static void red_plussymbol ()
	{
	  if (topi () == goal_plussymbol)
	     { pop (1);
	       callq ();
	       pushi (goal_plussymbol);
	     };
	  if (on_spine_to_goal (goal_factor)){
	  if (in_lookahead ("-+abcdefghijklmnopqrstuvwxyz0123456789("))
	  {
	  pushq (red_factor);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_factor);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (7);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  pushq (red_plussymbol);
	}

static void rule_rightparenthesis ()
	{
	  if (iptr_at (')'))
	     { pushi (goal_rightparenthesis);
	       pushq (get_rightparenthesis);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_rightparenthesis);
	}

static void get_rightparenthesis ()
	{
	  if (iptr_at (')'))
	  {
	  pushq (red_rightparenthesis);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (15);
	  pushq (make_normal_node);
	  pushs (")");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_rightparenthesis);
	}

static void red_rightparenthesis ()
	{
	  if (topi () == goal_rightparenthesis)
	     { pop (1);
	       callq ();
	       pushi (goal_rightparenthesis);
	     };
	  pushq (red_rightparenthesis);
	}

static void rule_start ()
	{
	  if (in_lookahead (" \n\t-+abcdefghijklmnopqrstuvwxyz0123456789("))
	     { pushi (goal_start);
	       pushq (get_start);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_start);
	}

static void get_start ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_1 = new_affix_node ("a_gen_1");
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_gen_1);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
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
	  pushi (9);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_gen_1 -> val);
	  free_affix_node (a_gen_1);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  if (iptr_at ('('))
	  {
	  pushq (red_leftparenthesis);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (10);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (in_lookahead (" \n\t"))
	  {
	  affix_node a_gen_2 = new_affix_node ("a_gen_2");
	  pushq (red_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_2);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs (" \n\t");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (1);
	  pushi (11);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_gen_2 -> val);
	  free_affix_node (a_gen_2);
	  };
	  if (iptr_at ('-'))
	  {
	  pushq (red_minussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (12);
	  pushq (make_normal_node);
	  pushs ("-");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  pushq (red_number);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (13);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  };
	  if (iptr_at ('+'))
	  {
	  pushq (red_plussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (14);
	  pushq (make_normal_node);
	  pushs ("+");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_start);
	}

static void red_start ()
	{
	  if (topi () == goal_start)
	     { pop (1);
	       callq ();
	       pushi (goal_start);
	     };
	  pushq (red_start);
	}

static void rule_term ()
	{
	  if (in_lookahead ("-+abcdefghijklmnopqrstuvwxyz0123456789("))
	     { pushi (goal_term);
	       pushq (get_term);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_term);
	}

static void get_term ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_1 = new_affix_node ("a_gen_1");
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_gen_1);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
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
	  pushi (9);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_gen_1 -> val);
	  free_affix_node (a_gen_1);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  if (iptr_at ('('))
	  {
	  pushq (red_leftparenthesis);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (10);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('-'))
	  {
	  pushq (red_minussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (12);
	  pushq (make_normal_node);
	  pushs ("-");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  pushq (red_number);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (13);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  };
	  if (iptr_at ('+'))
	  {
	  pushq (red_plussymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (14);
	  pushq (make_normal_node);
	  pushs ("+");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
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
	  if (on_spine_to_goal (goal_expr)){
	  if (in_lookahead ("+-\204)"))
	  {
	  pushq (red_expr);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (5);
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
	  pushq (rule_timessymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (17);
	  pushq (make_normal_node);
	  callq ();
	  pop (20);
	  };
	  };
	  if (on_spine_to_goal (goal_term)){
	  if (iptr_at ('/'))
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
	  pushq (rule_dividesbysymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (18);
	  pushq (make_normal_node);
	  callq ();
	  pop (20);
	  };
	  };
	  pushq (red_term);
	}

static void rule_unsignedfactor ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz0123456789("))
	     { pushi (goal_unsignedfactor);
	       pushq (get_unsignedfactor);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_unsignedfactor);
	}

static void get_unsignedfactor ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_gen_1 = new_affix_node ("a_gen_1");
	  affix_node a_gen_0 = new_affix_node ("a_gen_0");
	  pushq (red_identifier);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_gen_1);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
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
	  pushi (9);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_gen_1 -> val);
	  free_affix_node (a_gen_1);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  if (iptr_at ('('))
	  {
	  pushq (red_leftparenthesis);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (10);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_gen_3 = new_affix_node ("a_gen_3");
	  pushq (red_number);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_gen_3);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (13);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  };
	  pushq (get_unsignedfactor);
	}

static void red_unsignedfactor ()
	{
	  if (topi () == goal_unsignedfactor)
	     { pop (1);
	       callq ();
	       pushi (goal_unsignedfactor);
	     };
	  if (on_spine_to_goal (goal_factor)){
	  if (in_lookahead ("+-*/\204)"))
	  {
	  pushq (red_factor);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (8);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_unsignedfactor);
	}

static void rule_timessymbol ()
	{
	  if (iptr_at ('*'))
	     { pushi (goal_timessymbol);
	       pushq (get_timessymbol);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_timessymbol);
	}

static void get_timessymbol ()
	{
	  if (iptr_at ('*'))
	  {
	  pushq (red_timessymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (23);
	  pushq (make_normal_node);
	  pushs ("*");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_timessymbol);
	}

static void red_timessymbol ()
	{
	  if (topi () == goal_timessymbol)
	     { pop (1);
	       callq ();
	       pushi (goal_timessymbol);
	     };
	  pushq (red_timessymbol);
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
	  pushq (rule_start);
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
	     { case 0: return ("expr");
	       case 1: return ("eag_stddefs");
	       default: panic ("strange module nr %d", mnr);
	     };
	  return (NULL);
	};

char *name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return (expr_name_from_nodenr (nodenr));
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

