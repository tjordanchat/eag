/*
   File: turf_leftcorner.c
   Generated on Tue Aug  5 14:42:30 2025
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
affix_node make_int_affix ();
void rec_int_value ();
static affix_node make_ID_affix ();
static void rec_ID_value ();
static affix_node make_INT_affix ();
static void rec_INT_value ();
static affix_node make_REAL_affix ();
static void rec_REAL_value ();
static affix_node make_filter_affix ();
static void rec_filter_value ();
affix_node make_real_affix ();
void rec_real_value ();
static affix_node make_nlcr_affix ();
affix_node make_string_affix ();
void rec_string_value ();
static affix_node make_unit_affix ();
static void rec_unit_value ();

static affix_node make_ID_affix ()
	{
	  value v1 = new_string_value ("ID");
	  return (value_to_affix ("meta_ID", v1));
	};

static affix_node make_INT_affix ()
	{
	  value v1 = new_string_value ("INT");
	  return (value_to_affix ("meta_INT", v1));
	};

static affix_node make_REAL_affix ()
	{
	  value v1 = new_string_value ("REAL");
	  return (value_to_affix ("meta_REAL", v1));
	};

static affix_node make_filter_affix ()
	{ affix_node new = new_affix_node ("meta_filter");
	  new -> mfunc = rec_filter_value;
	  return (new);
	};

static affix_node make_nlcr_affix ()
	{
	  value v1 = new_string_value ("\n");
	  return (value_to_affix ("meta_nlcr", v1));
	};

static affix_node make_unit_affix ()
	{ affix_node new = new_affix_node ("meta_unit");
	  new -> mfunc = rec_unit_value;
	  return (new);
	};

static void rec_ID_value ()
	{ value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { if (strcmp (v -> u.str, "ID") == 0) callq ();
	     };
	  pushv (v);
	  pushq (rec_ID_value);
	};

static void rec_INT_value ()
	{ value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { if (strcmp (v -> u.str, "INT") == 0) callq ();
	     };
	  pushv (v);
	  pushq (rec_INT_value);
	};

static void rec_REAL_value ()
	{ value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { if (strcmp (v -> u.str, "REAL") == 0) callq ();
	     };
	  pushv (v);
	  pushq (rec_REAL_value);
	};

static void rec_filter_value ()
	{ value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     {
	      {
		pushv (v);
		pushs ("abcdef");
		pushq (rec_parse_set_star_strict);
		callq ();
		pop (3);
	      };
	     };
	  pushv (v);
	  pushq (rec_filter_value);
	};

static void rec_unit_value ()
	{ value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == tuple_value)
	     {
	      {
		if (v -> u.tuple -> size == 2)
		   {
		pushv (v -> u.tuple -> array[1]);
		pushq (rec_string_value);
		pushv (v -> u.tuple -> array[0]);
		pushq (rec_ID_value);
		     callq ();
		     pop (4);
		   };
	      };
	      {
		if (v -> u.tuple -> size == 2)
		   {
		pushv (v -> u.tuple -> array[1]);
		pushq (rec_int_value);
		pushv (v -> u.tuple -> array[0]);
		pushq (rec_INT_value);
		     callq ();
		     pop (4);
		   };
	      };
	      {
		if (v -> u.tuple -> size == 2)
		   {
		pushv (v -> u.tuple -> array[1]);
		pushq (rec_real_value);
		pushv (v -> u.tuple -> array[0]);
		pushq (rec_REAL_value);
		     callq ();
		     pop (4);
		   };
	      };
	     };
	  pushv (v);
	  pushq (rec_unit_value);
	};

static void rule_beginsymbol ();
static void get_beginsymbol ();
static void red_beginsymbol ();
 void semipred_endofsentence ();
static void rule_endsymbol ();
static void get_endsymbol ();
static void red_endsymbol ();
 void pred_inttostring ();
static void rule_identifier ();
static void get_identifier ();
static void red_identifier ();
static void rule_integernumber ();
static void get_integernumber ();
static void red_integernumber ();
static void rule_layout ();
static void get_layout ();
static void red_layout ();
static void empv_layout ();
static void emp_layout ();
 void pred_realtostring ();
static void rule_realnumber ();
static void get_realnumber ();
static void red_realnumber ();
 void pred_stringtoint ();
static void rule_start ();
static void get_start ();
static void red_start ();
static void rule_semicolonsymbol ();
static void get_semicolonsymbol ();
static void red_semicolonsymbol ();
 void pred_stringtoreal ();
static void rule_units ();
static void get_units ();
static void red_units ();
static void rule_unit ();
static void get_unit ();
static void red_unit ();
static void pred_xform ();
static void pred_xformunits ();
static void pred_xformunit ();

string turf_name_from_nodenr (int nodenr)
	{ switch (nodenr)
	     { case 0: return ("root_node");
	       case 1: return ("leaf_node");
	       case 2: return ("rule_beginsymbol");
	       case 3: return ("rule_endsymbol");
	       case 4: return ("rule_identifier");
	       case 5: return ("rule_integernumber");
	       case 6: return ("rule_layout");
	       case 7: return ("rule_realnumber");
	       case 8: return ("rule_start");
	       case 9: return ("rule_semicolonsymbol");
	       case 10: return ("rule_units");
	       case 11: return ("rule_units");
	       case 12: return ("rule_unit");
	       case 13: return ("rule_unit");
	       case 14: return ("rule_unit");
	       case 15: return ("pred_xform");
	       case 16: return ("pred_xform");
	       case 17: return ("pred_xformunits");
	       case 18: return ("pred_xformunits");
	       case 19: return ("pred_xformunits");
	       case 20: return ("pred_xformunit");
	       case 21: return ("pred_xformunit");
	       case 22: return ("pred_xformunit");
	       case 23: return ("pred_xformunit");
	       default: panic ("strange node %d in turf", nodenr);
	     };
	  return (NULL);
	};

/* code goals for non predicate syntax rules */
#define goal_beginsymbol 0
#define goal_endsymbol 1
#define goal_identifier 2
#define goal_integernumber 3
#define goal_layout 4
#define goal_realnumber 5
#define goal_start 6
#define goal_semicolonsymbol 7
#define goal_units 8
#define goal_unit 9
#define on_spine_to_goal(x) lc_rel[x * 10 + topi()]
static char lc_rel[] =
	{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 
	  0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 
	  0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 
	  0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 
	  };

static void rule_beginsymbol ()
	{
	  if (iptr_at ('B'))
	     { pushi (goal_beginsymbol);
	       pushq (get_beginsymbol);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_beginsymbol);
	}

static void get_beginsymbol ()
	{
	  if (iptr_at ('B'))
	  {
	  pushq (red_beginsymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (2);
	  pushq (make_normal_node);
	  pushs ("BEGIN");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_beginsymbol);
	}

static void red_beginsymbol ()
	{
	  if (topi () == goal_beginsymbol)
	     { pop (1);
	       callq ();
	       pushi (goal_beginsymbol);
	     };
	  if (on_spine_to_goal (goal_start)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz0123456789"))
	  {
	  affix_node a_out = new_affix_node ("a_out");
	  affix_node a_xout = new_affix_node ("a_xout");
	  affix_node a_burp = new_affix_node ("a_burp");
	  pushq (red_start);
	  pushi (6);
	  pushq (link_son);
	  pusha (a_xout);
	  pushi (tag_single);
	  pusha (a_out);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_xform);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (semipred_endofsentence);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_endsymbol);
	  pushi (3);
	  pushq (link_son);
	  pusha (a_out);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (rule_units);
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
	  pusha (a_xout);
	  pusha (a_burp);
	  pushi (2);
	  pushi (tag_concat);
	  pusha (a_burp);
	  pushi (tag_single);
	  pushi (2);
	  pushi (6);
	  pushi (8);
	  pushq (make_normal_node);
	  callq ();
	  pop (47);
	  rfre_value (a_out -> val);
	  free_affix_node (a_out);
	  rfre_value (a_xout -> val);
	  free_affix_node (a_xout);
	  rfre_value (a_burp -> val);
	  free_affix_node (a_burp);
	  };
	  };
	  pushq (red_beginsymbol);
	}

static void rule_endsymbol ()
	{
	  if (iptr_at ('E'))
	     { pushi (goal_endsymbol);
	       pushq (get_endsymbol);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_endsymbol);
	}

static void get_endsymbol ()
	{
	  if (iptr_at ('E'))
	  {
	  pushq (red_endsymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (3);
	  pushq (make_normal_node);
	  pushs ("END");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_endsymbol);
	}

static void red_endsymbol ()
	{
	  if (topi () == goal_endsymbol)
	     { pop (1);
	       callq ();
	       pushi (goal_endsymbol);
	     };
	  pushq (red_endsymbol);
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
	  pushq (red_identifier);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set_plus_strict);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushi (2);
	  pushi (4);
	  pushq (make_normal_node);
	  callq ();
	  pop (21);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
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
	  if (on_spine_to_goal (goal_unit)){
	  if (in_lookahead (";E"))
	  {
	  affix_node a_filter = make_filter_affix ();
	  affix_node a_ID = make_ID_affix ();
	  pushq (red_unit);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_filter);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_filter);
	  pusha (a_ID);
	  pushi (2);
	  pushi (tag_compos);
	  pushi (1);
	  pushi (1);
	  pushi (12);
	  pushq (make_normal_node);
	  callq ();
	  pop (16);
	  rfre_value (a_filter -> val);
	  free_affix_node (a_filter);
	  rfre_value (a_ID -> val);
	  free_affix_node (a_ID);
	  };
	  };
	  pushq (red_identifier);
	}

static void rule_integernumber ()
	{
	  if (in_lookahead ("0123456789"))
	     { pushi (goal_integernumber);
	       pushq (get_integernumber);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_integernumber);
	}

static void get_integernumber ()
	{
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_inum = new_affix_node ("a_inum");
	  pushq (red_integernumber);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_inum);
	  pushi (tag_single);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_stringtoint);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pusha (a_inum);
	  pushi (tag_single);
	  pushi (1);
	  pushi (3);
	  pushi (5);
	  pushq (make_normal_node);
	  callq ();
	  pop (30);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_inum -> val);
	  free_affix_node (a_inum);
	  };
	  pushq (get_integernumber);
	}

static void red_integernumber ()
	{
	  if (topi () == goal_integernumber)
	     { pop (1);
	       callq ();
	       pushi (goal_integernumber);
	     };
	  if (on_spine_to_goal (goal_unit)){
	  if (in_lookahead (";E"))
	  {
	  affix_node a_int = make_int_affix ();
	  affix_node a_INT = make_INT_affix ();
	  pushq (red_unit);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_int);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_int);
	  pusha (a_INT);
	  pushi (2);
	  pushi (tag_compos);
	  pushi (1);
	  pushi (1);
	  pushi (13);
	  pushq (make_normal_node);
	  callq ();
	  pop (16);
	  rfre_value (a_int -> val);
	  free_affix_node (a_int);
	  rfre_value (a_INT -> val);
	  free_affix_node (a_INT);
	  };
	  };
	  pushq (red_integernumber);
	}

static void rule_layout ()
	{
	  if (in_lookahead (" \n\t"))
	     { pushi (goal_layout);
	       pushq (get_layout);
	       callq ();
	       pop(2);
	     };
	  if (in_lookahead ("Babcdefghijklmnopqrstuvwxyz0123456789\204;E"))
	     { pushq (empv_layout);
	       callq ();
	       pop(1);
	     };
	  pushq (rule_layout);
	}

static void empv_layout ()
	{
	  {
	  affix_node a_ignore = new_affix_node ("a_ignore");
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ignore);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushq (make_empty_strstore);
	  pushs (" \n\t");
	  pushq (fail_if_iptr_at_set);
	  pushi (0);
	  pushi (1);
	  pushi (6);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_ignore -> val);
	  free_affix_node (a_ignore);
	  };
	  pushq (empv_layout);
	}

static void emp_layout ()
	{
	  {
	  affix_node a_ignore = new_affix_node ("a_ignore");
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ignore);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushq (make_empty_strstore);
	  pushi (0);
	  pushi (1);
	  pushi (6);
	  pushq (make_normal_node);
	  callq ();
	  pop (12);
	  rfre_value (a_ignore -> val);
	  free_affix_node (a_ignore);
	  };
	  pushq (emp_layout);
	}

static void get_layout ()
	{
	  if (in_lookahead (" \n\t"))
	  {
	  affix_node a_ignore = new_affix_node ("a_ignore");
	  pushq (red_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ignore);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs (" \n\t");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (1);
	  pushi (6);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_ignore -> val);
	  free_affix_node (a_ignore);
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
	  if (iptr_at ('B'))
	  {
	  affix_node a_out = new_affix_node ("a_out");
	  affix_node a_xout = new_affix_node ("a_xout");
	  affix_node a_burp = new_affix_node ("a_burp");
	  pushq (red_start);
	  pushi (6);
	  pushq (link_son);
	  pusha (a_xout);
	  pushi (tag_single);
	  pusha (a_out);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_xform);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (semipred_endofsentence);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_endsymbol);
	  pushi (3);
	  pushq (link_son);
	  pusha (a_out);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (rule_units);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_beginsymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_xout);
	  pusha (a_burp);
	  pushi (2);
	  pushi (tag_concat);
	  pusha (a_burp);
	  pushi (tag_single);
	  pushi (2);
	  pushi (6);
	  pushi (8);
	  pushq (make_normal_node);
	  callq ();
	  pop (47);
	  rfre_value (a_out -> val);
	  free_affix_node (a_out);
	  rfre_value (a_xout -> val);
	  free_affix_node (a_xout);
	  rfre_value (a_burp -> val);
	  free_affix_node (a_burp);
	  };
	  };
	  pushq (red_layout);
	}

static void rule_realnumber ()
	{
	  if (in_lookahead ("0123456789"))
	     { pushi (goal_realnumber);
	       pushq (get_realnumber);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_realnumber);
	}

static void get_realnumber ()
	{
	  if (in_lookahead ("0123456789"))
	  {
	  value v1 = new_string_value (".");
	  affix_node a_gen_0 = value_to_affix ("a_gen_0", v1);
	  affix_node a_ls2 = new_affix_node ("a_ls2");
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_rnum = new_affix_node ("a_rnum");
	  pushq (red_realnumber);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (3);
	  pushq (link_son);
	  pusha (a_rnum);
	  pushi (tag_single);
	  pusha (a_ls2);
	  pusha (a_gen_0);
	  pusha (a_ls);
	  pushi (3);
	  pushi (tag_concat);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_stringtoreal);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_ls2);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushs (".");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pusha (a_rnum);
	  pushi (tag_single);
	  pushi (1);
	  pushi (4);
	  pushi (7);
	  pushq (make_normal_node);
	  callq ();
	  pop (44);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  rfre_value (a_ls2 -> val);
	  free_affix_node (a_ls2);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_rnum -> val);
	  free_affix_node (a_rnum);
	  };
	  pushq (get_realnumber);
	}

static void red_realnumber ()
	{
	  if (topi () == goal_realnumber)
	     { pop (1);
	       callq ();
	       pushi (goal_realnumber);
	     };
	  if (on_spine_to_goal (goal_unit)){
	  if (in_lookahead (";E"))
	  {
	  affix_node a_real = make_real_affix ();
	  affix_node a_REAL = make_REAL_affix ();
	  pushq (red_unit);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_real);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_real);
	  pusha (a_REAL);
	  pushi (2);
	  pushi (tag_compos);
	  pushi (1);
	  pushi (1);
	  pushi (14);
	  pushq (make_normal_node);
	  callq ();
	  pop (16);
	  rfre_value (a_real -> val);
	  free_affix_node (a_real);
	  rfre_value (a_REAL -> val);
	  free_affix_node (a_REAL);
	  };
	  };
	  pushq (red_realnumber);
	}

static void rule_start ()
	{
	  if (in_lookahead (" \n\tB"))
	     { pushi (goal_start);
	       pushq (get_start);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_start);
	}

static void get_start ()
	{
	  if (iptr_at ('B'))
	  {
	  pushq (red_beginsymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (2);
	  pushq (make_normal_node);
	  pushs ("BEGIN");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (in_lookahead (" \n\t"))
	  {
	  affix_node a_ignore = new_affix_node ("a_ignore");
	  pushq (red_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ignore);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs (" \n\t");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (1);
	  pushi (6);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_ignore -> val);
	  free_affix_node (a_ignore);
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

static void rule_semicolonsymbol ()
	{
	  if (iptr_at (';'))
	     { pushi (goal_semicolonsymbol);
	       pushq (get_semicolonsymbol);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_semicolonsymbol);
	}

static void get_semicolonsymbol ()
	{
	  if (iptr_at (';'))
	  {
	  pushq (red_semicolonsymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (9);
	  pushq (make_normal_node);
	  pushs (";");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_semicolonsymbol);
	}

static void red_semicolonsymbol ()
	{
	  if (topi () == goal_semicolonsymbol)
	     { pop (1);
	       callq ();
	       pushi (goal_semicolonsymbol);
	     };
	  pushq (red_semicolonsymbol);
	}

static void rule_units ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz0123456789"))
	     { pushi (goal_units);
	       pushq (get_units);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_units);
	}

static void get_units ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_ls = new_affix_node ("a_ls");
	  pushq (red_identifier);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set_plus_strict);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushi (2);
	  pushi (4);
	  pushq (make_normal_node);
	  callq ();
	  pop (21);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_inum = new_affix_node ("a_inum");
	  pushq (red_integernumber);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_inum);
	  pushi (tag_single);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_stringtoint);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pusha (a_inum);
	  pushi (tag_single);
	  pushi (1);
	  pushi (3);
	  pushi (5);
	  pushq (make_normal_node);
	  callq ();
	  pop (30);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_inum -> val);
	  free_affix_node (a_inum);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  value v2 = new_string_value (".");
	  affix_node a_gen_0 = value_to_affix ("a_gen_0", v2);
	  affix_node a_ls2 = new_affix_node ("a_ls2");
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_rnum = new_affix_node ("a_rnum");
	  pushq (red_realnumber);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (3);
	  pushq (link_son);
	  pusha (a_rnum);
	  pushi (tag_single);
	  pusha (a_ls2);
	  pusha (a_gen_0);
	  pusha (a_ls);
	  pushi (3);
	  pushi (tag_concat);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_stringtoreal);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_ls2);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushs (".");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pusha (a_rnum);
	  pushi (tag_single);
	  pushi (1);
	  pushi (4);
	  pushi (7);
	  pushq (make_normal_node);
	  callq ();
	  pop (44);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  rfre_value (a_ls2 -> val);
	  free_affix_node (a_ls2);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_rnum -> val);
	  free_affix_node (a_rnum);
	  };
	  pushq (get_units);
	}

static void red_units ()
	{
	  if (topi () == goal_units)
	     { pop (1);
	       callq ();
	       pushi (goal_units);
	     };
	  pushq (red_units);
	}

static void rule_unit ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz0123456789"))
	     { pushi (goal_unit);
	       pushq (get_unit);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_unit);
	}

static void get_unit ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_ls = new_affix_node ("a_ls");
	  pushq (red_identifier);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set_plus_strict);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushi (2);
	  pushi (4);
	  pushq (make_normal_node);
	  callq ();
	  pop (21);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_inum = new_affix_node ("a_inum");
	  pushq (red_integernumber);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_inum);
	  pushi (tag_single);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_stringtoint);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pusha (a_inum);
	  pushi (tag_single);
	  pushi (1);
	  pushi (3);
	  pushi (5);
	  pushq (make_normal_node);
	  callq ();
	  pop (30);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_inum -> val);
	  free_affix_node (a_inum);
	  };
	  if (in_lookahead ("0123456789"))
	  {
	  value v3 = new_string_value (".");
	  affix_node a_gen_0 = value_to_affix ("a_gen_0", v3);
	  affix_node a_ls2 = new_affix_node ("a_ls2");
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_rnum = new_affix_node ("a_rnum");
	  pushq (red_realnumber);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (3);
	  pushq (link_son);
	  pusha (a_rnum);
	  pushi (tag_single);
	  pusha (a_ls2);
	  pusha (a_gen_0);
	  pusha (a_ls);
	  pushi (3);
	  pushi (tag_concat);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_stringtoreal);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_ls2);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pushs (".");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_plus_strict);
	  pusha (a_rnum);
	  pushi (tag_single);
	  pushi (1);
	  pushi (4);
	  pushi (7);
	  pushq (make_normal_node);
	  callq ();
	  pop (44);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  rfre_value (a_ls2 -> val);
	  free_affix_node (a_ls2);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_rnum -> val);
	  free_affix_node (a_rnum);
	  };
	  pushq (get_unit);
	}

static void red_unit ()
	{
	  if (topi () == goal_unit)
	     { pop (1);
	       callq ();
	       pushi (goal_unit);
	     };
	  if (on_spine_to_goal (goal_units)){
	  if (iptr_at (';'))
	  {
	  affix_node a_units = new_affix_node ("a_units");
	  affix_node a_unit = make_unit_affix ();
	  pushq (red_units);
	  pushi (3);
	  pushq (link_son);
	  pusha (a_units);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (rule_units);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_semicolonsymbol);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_unit);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_units);
	  pusha (a_unit);
	  pushi (2);
	  pushi (tag_compos);
	  pushi (1);
	  pushi (3);
	  pushi (10);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_units -> val);
	  free_affix_node (a_units);
	  rfre_value (a_unit -> val);
	  free_affix_node (a_unit);
	  };
	  };
	  if (on_spine_to_goal (goal_units)){
	  if (iptr_at ('E'))
	  {
	  affix_node a_unit = make_unit_affix ();
	  pushq (red_units);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_unit);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_unit);
	  pushi (tag_single);
	  pushi (1);
	  pushi (1);
	  pushi (11);
	  pushq (make_normal_node);
	  callq ();
	  pop (14);
	  rfre_value (a_unit -> val);
	  free_affix_node (a_unit);
	  };
	  };
	  pushq (red_unit);
	}

static void act_xform (pos_node *args)
	{ tree_node pnode = args[0] -> node;
	  {
	  affix_node a_xout = new_affix_node ("a_xout");
	  affix_node a_out = new_affix_node ("a_out");
	  pushi (1);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_xout);
	  pushi (tag_single);
	  pusha (a_out);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_xformunits);
	  pusha (a_xout);
	  pushi (tag_single);
	  pusha (a_out);
	  pushi (tag_single);
	  pushi (2);
	  pushpp (args);
	  pushi (1);
	  pushi (16);
	  pushq (update_predicate_node);
	  callq ();
	  pop (19);
	  rfre_value (a_xout -> val);
	  free_affix_node (a_xout);
	  rfre_value (a_out -> val);
	  free_affix_node (a_out);
	  };
	}

static void delayed_xform (pos_node *args)
	{
	  pos_node paf0 = args[0];
	  pos_node paf1 = args[1];
	  if (!crit_pos_has_value (paf0))
	     callq();
	  else {
	     paf0 -> delayed = 0;
	     paf1 -> delayed = 0;
	     act_xform (args);
	     paf0 -> delayed = 1;
	     paf1 -> delayed = 1;
	  };
	}

static void pred_xform ()
	{
	  affix_node paf0 = new_affix_node ("pred_xform_paf0");
	  affix_node paf1 = new_affix_node ("pred_xform_paf1");
	  pushq (delayed_xform);
	  pushq (make_node_delayed);
	  pusha (paf1);
	  pushi (tag_single);
	  pusha (paf0);
	  pushi (tag_single);
	  pushi (2);
	  pushi (0);
	  pushi (15);
	  pushq (make_predicate_node);
	  callq ();
	  pop (10);
	  rfre_value (paf0 -> val);
	  free_affix_node (paf0);
	  rfre_value (paf1 -> val);
	  free_affix_node (paf1);
	  pushq (pred_xform);
	};

static void act_xformunits (pos_node *args)
	{ tree_node pnode = args[0] -> node;
	  {
	  affix_node a_xunits = new_affix_node ("a_xunits");
	  affix_node a_xunit = new_affix_node ("a_xunit");
	  affix_node a_units = new_affix_node ("a_units");
	  affix_node a_unit = make_unit_affix ();
	  pushi (2);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_xunits);
	  pushi (tag_single);
	  pusha (a_units);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_xformunits);
	  pushi (1);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_xunit);
	  pushi (tag_single);
	  pusha (a_unit);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_xformunit);
	  pusha (a_xunits);
	  pusha (a_xunit);
	  pushi (2);
	  pushi (tag_concat);
	  pusha (a_units);
	  pusha (a_unit);
	  pushi (2);
	  pushi (tag_compos);
	  pushi (2);
	  pushpp (args);
	  pushi (2);
	  pushi (18);
	  pushq (update_predicate_node);
	  callq ();
	  pop (33);
	  rfre_value (a_xunits -> val);
	  free_affix_node (a_xunits);
	  rfre_value (a_xunit -> val);
	  free_affix_node (a_xunit);
	  rfre_value (a_units -> val);
	  free_affix_node (a_units);
	  rfre_value (a_unit -> val);
	  free_affix_node (a_unit);
	  };
	  {
	  affix_node a_xunit = new_affix_node ("a_xunit");
	  affix_node a_unit = make_unit_affix ();
	  pushi (1);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_xunit);
	  pushi (tag_single);
	  pusha (a_unit);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_xformunit);
	  pusha (a_xunit);
	  pushi (tag_single);
	  pusha (a_unit);
	  pushi (tag_single);
	  pushi (2);
	  pushpp (args);
	  pushi (1);
	  pushi (19);
	  pushq (update_predicate_node);
	  callq ();
	  pop (19);
	  rfre_value (a_xunit -> val);
	  free_affix_node (a_xunit);
	  rfre_value (a_unit -> val);
	  free_affix_node (a_unit);
	  };
	}

static void delayed_xformunits (pos_node *args)
	{
	  pos_node paf0 = args[0];
	  pos_node paf1 = args[1];
	  if (!crit_pos_has_value (paf0))
	     callq();
	  else {
	     paf0 -> delayed = 0;
	     paf1 -> delayed = 0;
	     act_xformunits (args);
	     paf0 -> delayed = 1;
	     paf1 -> delayed = 1;
	  };
	}

static void pred_xformunits ()
	{
	  affix_node paf0 = new_affix_node ("pred_xformunits_paf0");
	  affix_node paf1 = new_affix_node ("pred_xformunits_paf1");
	  pushq (delayed_xformunits);
	  pushq (make_node_delayed);
	  pusha (paf1);
	  pushi (tag_single);
	  pusha (paf0);
	  pushi (tag_single);
	  pushi (2);
	  pushi (0);
	  pushi (17);
	  pushq (make_predicate_node);
	  callq ();
	  pop (10);
	  rfre_value (paf0 -> val);
	  free_affix_node (paf0);
	  rfre_value (paf1 -> val);
	  free_affix_node (paf1);
	  pushq (pred_xformunits);
	};

static void act_xformunit (pos_node *args)
	{ tree_node pnode = args[0] -> node;
	  {
	  affix_node a_nlcr = make_nlcr_affix ();
	  value v4 = new_string_value (";");
	  affix_node a_gen_1 = value_to_affix ("a_gen_1", v4);
	  affix_node a_string = make_string_affix ();
	  affix_node a_ID = make_ID_affix ();
	  pusha (a_nlcr);
	  pusha (a_gen_1);
	  pusha (a_string);
	  pushi (3);
	  pushi (tag_concat);
	  pusha (a_string);
	  pusha (a_ID);
	  pushi (2);
	  pushi (tag_compos);
	  pushi (2);
	  pushpp (args);
	  pushi (0);
	  pushi (21);
	  pushq (update_predicate_node);
	  callq ();
	  pop (14);
	  rfre_value (a_nlcr -> val);
	  free_affix_node (a_nlcr);
	  rfre_value (a_gen_1 -> val);
	  free_affix_node (a_gen_1);
	  rfre_value (a_string -> val);
	  free_affix_node (a_string);
	  rfre_value (a_ID -> val);
	  free_affix_node (a_ID);
	  };
	  {
	  affix_node a_nlcr = make_nlcr_affix ();
	  value v5 = new_string_value (";");
	  affix_node a_gen_2 = value_to_affix ("a_gen_2", v5);
	  affix_node a_string = make_string_affix ();
	  affix_node a_int = make_int_affix ();
	  affix_node a_INT = make_INT_affix ();
	  pushi (1);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_string);
	  pushi (tag_single);
	  pusha (a_int);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_inttostring);
	  pusha (a_nlcr);
	  pusha (a_gen_2);
	  pusha (a_string);
	  pushi (3);
	  pushi (tag_concat);
	  pusha (a_int);
	  pusha (a_INT);
	  pushi (2);
	  pushi (tag_compos);
	  pushi (2);
	  pushpp (args);
	  pushi (1);
	  pushi (22);
	  pushq (update_predicate_node);
	  callq ();
	  pop (24);
	  rfre_value (a_nlcr -> val);
	  free_affix_node (a_nlcr);
	  rfre_value (a_gen_2 -> val);
	  free_affix_node (a_gen_2);
	  rfre_value (a_string -> val);
	  free_affix_node (a_string);
	  rfre_value (a_int -> val);
	  free_affix_node (a_int);
	  rfre_value (a_INT -> val);
	  free_affix_node (a_INT);
	  };
	  {
	  affix_node a_nlcr = make_nlcr_affix ();
	  value v6 = new_string_value (";");
	  affix_node a_gen_3 = value_to_affix ("a_gen_3", v6);
	  affix_node a_string = make_string_affix ();
	  affix_node a_real = make_real_affix ();
	  affix_node a_REAL = make_REAL_affix ();
	  pushi (1);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_string);
	  pushi (tag_single);
	  pusha (a_real);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_realtostring);
	  pusha (a_nlcr);
	  pusha (a_gen_3);
	  pusha (a_string);
	  pushi (3);
	  pushi (tag_concat);
	  pusha (a_real);
	  pusha (a_REAL);
	  pushi (2);
	  pushi (tag_compos);
	  pushi (2);
	  pushpp (args);
	  pushi (1);
	  pushi (23);
	  pushq (update_predicate_node);
	  callq ();
	  pop (24);
	  rfre_value (a_nlcr -> val);
	  free_affix_node (a_nlcr);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  rfre_value (a_string -> val);
	  free_affix_node (a_string);
	  rfre_value (a_real -> val);
	  free_affix_node (a_real);
	  rfre_value (a_REAL -> val);
	  free_affix_node (a_REAL);
	  };
	}

static void delayed_xformunit (pos_node *args)
	{
	  pos_node paf0 = args[0];
	  pos_node paf1 = args[1];
	  if (!crit_pos_has_value (paf0))
	     callq();
	  else {
	     paf0 -> delayed = 0;
	     paf1 -> delayed = 0;
	     act_xformunit (args);
	     paf0 -> delayed = 1;
	     paf1 -> delayed = 1;
	  };
	}

static void pred_xformunit ()
	{
	  affix_node paf0 = new_affix_node ("pred_xformunit_paf0");
	  affix_node paf1 = new_affix_node ("pred_xformunit_paf1");
	  pushq (delayed_xformunit);
	  pushq (make_node_delayed);
	  pusha (paf1);
	  pushi (tag_single);
	  pusha (paf0);
	  pushi (tag_single);
	  pushi (2);
	  pushi (0);
	  pushi (20);
	  pushq (make_predicate_node);
	  callq ();
	  pop (10);
	  rfre_value (paf0 -> val);
	  free_affix_node (paf0);
	  rfre_value (paf1 -> val);
	  free_affix_node (paf1);
	  pushq (pred_xformunit);
	};

void transduce ()
	{
	  affix_node a_xout = new_affix_node ("a_xout");
	  value v7 = new_string_value ("This is a burp\n");
	  affix_node a_gen_4 = value_to_affix ("a_gen_4", v7);
	  pushq (dummy_continuation);
	  pushq (increment_nrparses);
	  pusha (a_xout);
	  pushq (output_affix);
	  pushq (endofsentence);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_xout);
	  pushi (tag_single);
	  pusha (a_gen_4);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (rule_start);
	  pusha (a_xout);
	  pushi (tag_single);
	  pusha (a_gen_4);
	  pushi (tag_single);
	  pushi (2);
	  pushi (1);	/* one son only */
	  pushi (0);	/* the rootnode */
	  pushq (make_normal_node);
	  init_eag_stddefs (1);
	  callq ();
	  pop (21);
	  rfre_value (a_xout -> val);
	  free_affix_node (a_xout);
	  rfre_value (a_gen_4 -> val);
	  free_affix_node (a_gen_4);
	};

string module_name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return ("turf");
	       case 1: return ("eag_stddefs");
	       default: panic ("strange module nr %d", mnr);
	     };
	  return (NULL);
	};

char *name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return (turf_name_from_nodenr (nodenr));
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

