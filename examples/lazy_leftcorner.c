/*
   File: lazy_leftcorner.c
   Generated on Mon Aug  4 17:12:22 2025
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
affix_node make_nil_affix ();
void rec_nil_value ();
affix_node make_string_affix ();
void rec_string_value ();
affix_node make_tuple_affix ();
void rec_tuple_value ();

static affix_node make_empty_affix ()
	{
	  value v1 = new_string_value ("");
	  return (value_to_affix ("meta_empty", v1));
	};

static void rule_application ();
static void get_application ();
static void red_application ();
static void rule_applysymbol ();
static void get_applysymbol ();
static void red_applysymbol ();
static void pred_codedecls ();
static void pred_codeapplies ();
static void rule_beginsymbol ();
static void get_beginsymbol ();
static void red_beginsymbol ();
static void rule_declaration ();
static void get_declaration ();
static void red_declaration ();
static void rule_declsymbol ();
static void get_declsymbol ();
static void red_declsymbol ();
 void semipred_endofsentence ();
static void rule_endsymbol ();
static void get_endsymbol ();
static void red_endsymbol ();
static void pred_inlist ();
static void rule_identifier ();
static void get_identifier ();
static void red_identifier ();
static void rule_layout ();
static void get_layout ();
static void red_layout ();
static void empv_layout ();
static void emp_layout ();
 void pred_notequal ();
static void pred_notinlist ();
static void rule_start ();
static void get_start ();
static void red_start ();
static void rule_semicolon ();
static void get_semicolon ();
static void red_semicolon ();
static void rule_units ();
static void get_units ();
static void red_units ();
static void rule_unit ();
static void get_unit ();
static void red_unit ();

string lazy_name_from_nodenr (int nodenr)
	{ switch (nodenr)
	     { case 0: return ("root_node");
	       case 1: return ("leaf_node");
	       case 2: return ("rule_application");
	       case 3: return ("rule_applysymbol");
	       case 4: return ("pred_codedecls");
	       case 5: return ("pred_codedecls");
	       case 6: return ("pred_codedecls");
	       case 7: return ("pred_codeapplies");
	       case 8: return ("pred_codeapplies");
	       case 9: return ("pred_codeapplies");
	       case 10: return ("rule_beginsymbol");
	       case 11: return ("rule_declaration");
	       case 12: return ("rule_declsymbol");
	       case 13: return ("rule_endsymbol");
	       case 14: return ("pred_inlist");
	       case 15: return ("pred_inlist");
	       case 16: return ("pred_inlist");
	       case 17: return ("rule_identifier");
	       case 18: return ("rule_layout");
	       case 19: return ("pred_notinlist");
	       case 20: return ("pred_notinlist");
	       case 21: return ("pred_notinlist");
	       case 22: return ("rule_start");
	       case 23: return ("rule_semicolon");
	       case 24: return ("rule_units");
	       case 25: return ("rule_units");
	       case 26: return ("rule_unit");
	       case 27: return ("rule_unit");
	       default: panic ("strange node %d in lazy", nodenr);
	     };
	  return (NULL);
	};

/* code goals for non predicate syntax rules */
#define goal_application 0
#define goal_applysymbol 1
#define goal_beginsymbol 2
#define goal_declaration 3
#define goal_declsymbol 4
#define goal_endsymbol 5
#define goal_identifier 6
#define goal_layout 7
#define goal_start 8
#define goal_semicolon 9
#define goal_units 10
#define goal_unit 11
#define on_spine_to_goal(x) lc_rel[x * 12 + topi()]
static char lc_rel[] =
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 
	  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 
	  0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 
	  0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 
	  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 
	  };

static void rule_application ()
	{
	  if (iptr_at ('A'))
	     { pushi (goal_application);
	       pushq (get_application);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_application);
	}

static void get_application ()
	{
	  if (iptr_at ('A'))
	  {
	  pushq (red_applysymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (3);
	  pushq (make_normal_node);
	  pushs ("APPLY");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_application);
	}

static void red_application ()
	{
	  if (topi () == goal_application)
	     { pop (1);
	       callq ();
	       pushi (goal_application);
	     };
	  if (on_spine_to_goal (goal_unit)){
	  if (in_lookahead (";E"))
	  {
	  affix_node a_newapplies = new_affix_node ("a_newapplies");
	  affix_node a_oldapplies = new_affix_node ("a_oldapplies");
	  affix_node a_decls = new_affix_node ("a_decls");
	  affix_node a_alldecls = new_affix_node ("a_alldecls");
	  pushq (red_unit);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_newapplies);
	  pushi (tag_single);
	  pusha (a_oldapplies);
	  pushi (tag_single);
	  pusha (a_alldecls);
	  pushi (tag_single);
	  pushi (3);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_newapplies);
	  pushi (tag_single);
	  pusha (a_decls);
	  pushi (tag_single);
	  pusha (a_oldapplies);
	  pushi (tag_single);
	  pusha (a_decls);
	  pushi (tag_single);
	  pusha (a_alldecls);
	  pushi (tag_single);
	  pushi (5);
	  pushi (1);
	  pushi (27);
	  pushq (make_normal_node);
	  callq ();
	  pop (26);
	  rfre_value (a_newapplies -> val);
	  free_affix_node (a_newapplies);
	  rfre_value (a_oldapplies -> val);
	  free_affix_node (a_oldapplies);
	  rfre_value (a_decls -> val);
	  free_affix_node (a_decls);
	  rfre_value (a_alldecls -> val);
	  free_affix_node (a_alldecls);
	  };
	  };
	  pushq (red_application);
	}

static void rule_applysymbol ()
	{
	  if (iptr_at ('A'))
	     { pushi (goal_applysymbol);
	       pushq (get_applysymbol);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_applysymbol);
	}

static void get_applysymbol ()
	{
	  if (iptr_at ('A'))
	  {
	  pushq (red_applysymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (3);
	  pushq (make_normal_node);
	  pushs ("APPLY");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_applysymbol);
	}

static void red_applysymbol ()
	{
	  if (topi () == goal_applysymbol)
	     { pop (1);
	       callq ();
	       pushi (goal_applysymbol);
	     };
	  if (on_spine_to_goal (goal_application)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_applies = new_affix_node ("a_applies");
	  affix_node a_alldecls = new_affix_node ("a_alldecls");
	  pushq (red_application);
	  pushi (3);
	  pushq (link_son);
	  pusha (a_alldecls);
	  pushi (tag_single);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_inlist);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (rule_identifier);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_ls);
	  pusha (a_applies);
	  pushi (2);
	  pushi (tag_compos);
	  pusha (a_applies);
	  pushi (tag_single);
	  pusha (a_alldecls);
	  pushi (tag_single);
	  pushi (3);
	  pushi (3);
	  pushi (2);
	  pushq (make_normal_node);
	  callq ();
	  pop (34);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_applies -> val);
	  free_affix_node (a_applies);
	  rfre_value (a_alldecls -> val);
	  free_affix_node (a_alldecls);
	  };
	  };
	  pushq (red_applysymbol);
	}

static void act_codedecls (pos_node *args)
	{ tree_node pnode = args[0] -> node;
	  {
	  affix_node a_empty = make_empty_affix ();
	  affix_node a_nil = make_nil_affix ();
	  pusha (a_empty);
	  pushi (tag_single);
	  pusha (a_nil);
	  pushi (tag_single);
	  pushi (2);
	  pushpp (args);
	  pushi (0);
	  pushi (5);
	  pushq (update_predicate_node);
	  callq ();
	  pop (9);
	  rfre_value (a_empty -> val);
	  free_affix_node (a_empty);
	  rfre_value (a_nil -> val);
	  free_affix_node (a_nil);
	  };
	  {
	  value v2 = new_string_value (";\n");
	  affix_node a_gen_1 = value_to_affix ("a_gen_1", v2);
	  value v3 = new_string_value ("\t  decl ");
	  affix_node a_gen_0 = value_to_affix ("a_gen_0", v3);
	  affix_node a_cdecls = new_affix_node ("a_cdecls");
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_list = new_affix_node ("a_list");
	  pushi (1);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_cdecls);
	  pushi (tag_single);
	  pusha (a_list);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_codedecls);
	  pusha (a_gen_1);
	  pusha (a_ls);
	  pusha (a_gen_0);
	  pusha (a_cdecls);
	  pushi (4);
	  pushi (tag_concat);
	  pusha (a_ls);
	  pusha (a_list);
	  pushi (2);
	  pushi (tag_compos);
	  pushi (2);
	  pushpp (args);
	  pushi (1);
	  pushi (6);
	  pushq (update_predicate_node);
	  callq ();
	  pop (25);
	  rfre_value (a_gen_1 -> val);
	  free_affix_node (a_gen_1);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  rfre_value (a_cdecls -> val);
	  free_affix_node (a_cdecls);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_list -> val);
	  free_affix_node (a_list);
	  };
	}

static void delayed_codedecls (pos_node *args)
	{
	  pos_node paf0 = args[0];
	  pos_node paf1 = args[1];
	  if (!crit_pos_has_value (paf0))
	     callq();
	  else {
	     paf0 -> delayed = 0;
	     paf1 -> delayed = 0;
	     act_codedecls (args);
	     paf0 -> delayed = 1;
	     paf1 -> delayed = 1;
	  };
	}

static void pred_codedecls ()
	{
	  affix_node paf0 = new_affix_node ("pred_codedecls_paf0");
	  affix_node paf1 = new_affix_node ("pred_codedecls_paf1");
	  pushq (delayed_codedecls);
	  pushq (make_node_delayed);
	  pusha (paf1);
	  pushi (tag_single);
	  pusha (paf0);
	  pushi (tag_single);
	  pushi (2);
	  pushi (0);
	  pushi (4);
	  pushq (make_predicate_node);
	  callq ();
	  pop (10);
	  rfre_value (paf0 -> val);
	  free_affix_node (paf0);
	  rfre_value (paf1 -> val);
	  free_affix_node (paf1);
	  pushq (pred_codedecls);
	};

static void act_codeapplies (pos_node *args)
	{ tree_node pnode = args[0] -> node;
	  {
	  affix_node a_empty = make_empty_affix ();
	  affix_node a_nil = make_nil_affix ();
	  pusha (a_empty);
	  pushi (tag_single);
	  pusha (a_nil);
	  pushi (tag_single);
	  pushi (2);
	  pushpp (args);
	  pushi (0);
	  pushi (8);
	  pushq (update_predicate_node);
	  callq ();
	  pop (9);
	  rfre_value (a_empty -> val);
	  free_affix_node (a_empty);
	  rfre_value (a_nil -> val);
	  free_affix_node (a_nil);
	  };
	  {
	  value v4 = new_string_value (";\n");
	  affix_node a_gen_3 = value_to_affix ("a_gen_3", v4);
	  value v5 = new_string_value ("\t  apply ");
	  affix_node a_gen_2 = value_to_affix ("a_gen_2", v5);
	  affix_node a_capplies = new_affix_node ("a_capplies");
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_list = new_affix_node ("a_list");
	  pushi (1);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_capplies);
	  pushi (tag_single);
	  pusha (a_list);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_codeapplies);
	  pusha (a_gen_3);
	  pusha (a_ls);
	  pusha (a_gen_2);
	  pusha (a_capplies);
	  pushi (4);
	  pushi (tag_concat);
	  pusha (a_ls);
	  pusha (a_list);
	  pushi (2);
	  pushi (tag_compos);
	  pushi (2);
	  pushpp (args);
	  pushi (1);
	  pushi (9);
	  pushq (update_predicate_node);
	  callq ();
	  pop (25);
	  rfre_value (a_gen_3 -> val);
	  free_affix_node (a_gen_3);
	  rfre_value (a_gen_2 -> val);
	  free_affix_node (a_gen_2);
	  rfre_value (a_capplies -> val);
	  free_affix_node (a_capplies);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_list -> val);
	  free_affix_node (a_list);
	  };
	}

static void delayed_codeapplies (pos_node *args)
	{
	  pos_node paf0 = args[0];
	  pos_node paf1 = args[1];
	  if (!crit_pos_has_value (paf0))
	     callq();
	  else {
	     paf0 -> delayed = 0;
	     paf1 -> delayed = 0;
	     act_codeapplies (args);
	     paf0 -> delayed = 1;
	     paf1 -> delayed = 1;
	  };
	}

static void pred_codeapplies ()
	{
	  affix_node paf0 = new_affix_node ("pred_codeapplies_paf0");
	  affix_node paf1 = new_affix_node ("pred_codeapplies_paf1");
	  pushq (delayed_codeapplies);
	  pushq (make_node_delayed);
	  pusha (paf1);
	  pushi (tag_single);
	  pusha (paf0);
	  pushi (tag_single);
	  pushi (2);
	  pushi (0);
	  pushi (7);
	  pushq (make_predicate_node);
	  callq ();
	  pop (10);
	  rfre_value (paf0 -> val);
	  free_affix_node (paf0);
	  rfre_value (paf1 -> val);
	  free_affix_node (paf1);
	  pushq (pred_codeapplies);
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
	  pushi (10);
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
	  if (in_lookahead ("DA"))
	  {
	  affix_node a_applies = new_affix_node ("a_applies");
	  affix_node a_nil = make_nil_affix ();
	  affix_node a_decls = new_affix_node ("a_decls");
	  value v6 = new_string_value ("\t}\n");
	  affix_node a_gen_5 = value_to_affix ("a_gen_5", v6);
	  affix_node a_capplies = new_affix_node ("a_capplies");
	  affix_node a_cdecls = new_affix_node ("a_cdecls");
	  value v7 = new_string_value ("program\n\t{\n");
	  affix_node a_gen_4 = value_to_affix ("a_gen_4", v7);
	  pushq (red_start);
	  pushi (7);
	  pushq (link_son);
	  pusha (a_capplies);
	  pushi (tag_single);
	  pusha (a_applies);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_codeapplies);
	  pushi (6);
	  pushq (link_son);
	  pusha (a_cdecls);
	  pushi (tag_single);
	  pusha (a_decls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_codedecls);
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
	  pusha (a_applies);
	  pushi (tag_single);
	  pusha (a_decls);
	  pushi (tag_single);
	  pusha (a_nil);
	  pushi (tag_single);
	  pusha (a_nil);
	  pushi (tag_single);
	  pusha (a_decls);
	  pushi (tag_single);
	  pushi (5);
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
	  pusha (a_gen_5);
	  pusha (a_capplies);
	  pusha (a_cdecls);
	  pusha (a_gen_4);
	  pushi (4);
	  pushi (tag_concat);
	  pushi (1);
	  pushi (7);
	  pushi (22);
	  pushq (make_normal_node);
	  callq ();
	  pop (64);
	  rfre_value (a_applies -> val);
	  free_affix_node (a_applies);
	  rfre_value (a_nil -> val);
	  free_affix_node (a_nil);
	  rfre_value (a_decls -> val);
	  free_affix_node (a_decls);
	  rfre_value (a_gen_5 -> val);
	  free_affix_node (a_gen_5);
	  rfre_value (a_capplies -> val);
	  free_affix_node (a_capplies);
	  rfre_value (a_cdecls -> val);
	  free_affix_node (a_cdecls);
	  rfre_value (a_gen_4 -> val);
	  free_affix_node (a_gen_4);
	  };
	  };
	  pushq (red_beginsymbol);
	}

static void rule_declaration ()
	{
	  if (iptr_at ('D'))
	     { pushi (goal_declaration);
	       pushq (get_declaration);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_declaration);
	}

static void get_declaration ()
	{
	  if (iptr_at ('D'))
	  {
	  pushq (red_declsymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (12);
	  pushq (make_normal_node);
	  pushs ("DECL");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_declaration);
	}

static void red_declaration ()
	{
	  if (topi () == goal_declaration)
	     { pop (1);
	       callq ();
	       pushi (goal_declaration);
	     };
	  if (on_spine_to_goal (goal_unit)){
	  if (in_lookahead (";E"))
	  {
	  affix_node a_newdecls = new_affix_node ("a_newdecls");
	  affix_node a_applies = new_affix_node ("a_applies");
	  affix_node a_olddecls = new_affix_node ("a_olddecls");
	  affix_node a_alldecls = new_affix_node ("a_alldecls");
	  pushq (red_unit);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_newdecls);
	  pushi (tag_single);
	  pusha (a_olddecls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_applies);
	  pushi (tag_single);
	  pusha (a_newdecls);
	  pushi (tag_single);
	  pusha (a_applies);
	  pushi (tag_single);
	  pusha (a_olddecls);
	  pushi (tag_single);
	  pusha (a_alldecls);
	  pushi (tag_single);
	  pushi (5);
	  pushi (1);
	  pushi (26);
	  pushq (make_normal_node);
	  callq ();
	  pop (24);
	  rfre_value (a_newdecls -> val);
	  free_affix_node (a_newdecls);
	  rfre_value (a_applies -> val);
	  free_affix_node (a_applies);
	  rfre_value (a_olddecls -> val);
	  free_affix_node (a_olddecls);
	  rfre_value (a_alldecls -> val);
	  free_affix_node (a_alldecls);
	  };
	  };
	  pushq (red_declaration);
	}

static void rule_declsymbol ()
	{
	  if (iptr_at ('D'))
	     { pushi (goal_declsymbol);
	       pushq (get_declsymbol);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_declsymbol);
	}

static void get_declsymbol ()
	{
	  if (iptr_at ('D'))
	  {
	  pushq (red_declsymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (12);
	  pushq (make_normal_node);
	  pushs ("DECL");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_declsymbol);
	}

static void red_declsymbol ()
	{
	  if (topi () == goal_declsymbol)
	     { pop (1);
	       callq ();
	       pushi (goal_declsymbol);
	     };
	  if (on_spine_to_goal (goal_declaration)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_ls = new_affix_node ("a_ls");
	  affix_node a_decls = new_affix_node ("a_decls");
	  pushq (red_declaration);
	  pushi (3);
	  pushq (link_son);
	  pusha (a_decls);
	  pushi (tag_single);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_notinlist);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (rule_identifier);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_ls);
	  pusha (a_decls);
	  pushi (2);
	  pushi (tag_compos);
	  pusha (a_decls);
	  pushi (tag_single);
	  pushi (2);
	  pushi (3);
	  pushi (11);
	  pushq (make_normal_node);
	  callq ();
	  pop (32);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  rfre_value (a_decls -> val);
	  free_affix_node (a_decls);
	  };
	  };
	  pushq (red_declsymbol);
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
	  pushi (13);
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

static void act_inlist (pos_node *args)
	{ tree_node pnode = args[0] -> node;
	  {
	  affix_node a_tuple = make_tuple_affix ();
	  affix_node a_ls = new_affix_node ("a_ls");
	  pusha (a_ls);
	  pusha (a_tuple);
	  pushi (2);
	  pushi (tag_compos);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushpp (args);
	  pushi (0);
	  pushi (15);
	  pushq (update_predicate_node);
	  callq ();
	  pop (11);
	  rfre_value (a_tuple -> val);
	  free_affix_node (a_tuple);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  };
	  {
	  affix_node a_string = make_string_affix ();
	  affix_node a_tuple = make_tuple_affix ();
	  affix_node a_ls = new_affix_node ("a_ls");
	  pushi (2);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_tuple);
	  pushi (tag_single);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_inlist);
	  pushi (1);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_string);
	  pushi (tag_single);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_notequal);
	  pusha (a_string);
	  pusha (a_tuple);
	  pushi (2);
	  pushi (tag_compos);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushpp (args);
	  pushi (2);
	  pushi (16);
	  pushq (update_predicate_node);
	  callq ();
	  pop (31);
	  rfre_value (a_string -> val);
	  free_affix_node (a_string);
	  rfre_value (a_tuple -> val);
	  free_affix_node (a_tuple);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  };
	}

static void delayed_inlist (pos_node *args)
	{
	  pos_node paf0 = args[0];
	  pos_node paf1 = args[1];
	  if (!crit_pos_has_value (paf0) || !crit_pos_has_value (paf1))
	     callq();
	  else {
	     paf0 -> delayed = 0;
	     paf1 -> delayed = 0;
	     act_inlist (args);
	     paf0 -> delayed = 1;
	     paf1 -> delayed = 1;
	  };
	}

static void pred_inlist ()
	{
	  affix_node paf0 = new_affix_node ("pred_inlist_paf0");
	  affix_node paf1 = new_affix_node ("pred_inlist_paf1");
	  pushq (delayed_inlist);
	  pushq (make_node_delayed);
	  pusha (paf1);
	  pushi (tag_single);
	  pusha (paf0);
	  pushi (tag_single);
	  pushi (2);
	  pushi (0);
	  pushi (14);
	  pushq (make_predicate_node);
	  callq ();
	  pop (10);
	  rfre_value (paf0 -> val);
	  free_affix_node (paf0);
	  rfre_value (paf1 -> val);
	  free_affix_node (paf1);
	  pushq (pred_inlist);
	};

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
	  pushi (17);
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
	  pushq (red_identifier);
	}

static void rule_layout ()
	{
	  if (in_lookahead (" \n\t"))
	     { pushi (goal_layout);
	       pushq (get_layout);
	       callq ();
	       pop(2);
	     };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzBDA\204;E"))
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
	  pushi (18);
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
	  pushi (18);
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
	  pushi (18);
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
	  affix_node a_applies = new_affix_node ("a_applies");
	  affix_node a_nil = make_nil_affix ();
	  affix_node a_decls = new_affix_node ("a_decls");
	  value v8 = new_string_value ("\t}\n");
	  affix_node a_gen_5 = value_to_affix ("a_gen_5", v8);
	  affix_node a_capplies = new_affix_node ("a_capplies");
	  affix_node a_cdecls = new_affix_node ("a_cdecls");
	  value v9 = new_string_value ("program\n\t{\n");
	  affix_node a_gen_4 = value_to_affix ("a_gen_4", v9);
	  pushq (red_start);
	  pushi (7);
	  pushq (link_son);
	  pusha (a_capplies);
	  pushi (tag_single);
	  pusha (a_applies);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_codeapplies);
	  pushi (6);
	  pushq (link_son);
	  pusha (a_cdecls);
	  pushi (tag_single);
	  pusha (a_decls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_codedecls);
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
	  pusha (a_applies);
	  pushi (tag_single);
	  pusha (a_decls);
	  pushi (tag_single);
	  pusha (a_nil);
	  pushi (tag_single);
	  pusha (a_nil);
	  pushi (tag_single);
	  pusha (a_decls);
	  pushi (tag_single);
	  pushi (5);
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
	  pusha (a_gen_5);
	  pusha (a_capplies);
	  pusha (a_cdecls);
	  pusha (a_gen_4);
	  pushi (4);
	  pushi (tag_concat);
	  pushi (1);
	  pushi (7);
	  pushi (22);
	  pushq (make_normal_node);
	  callq ();
	  pop (64);
	  rfre_value (a_applies -> val);
	  free_affix_node (a_applies);
	  rfre_value (a_nil -> val);
	  free_affix_node (a_nil);
	  rfre_value (a_decls -> val);
	  free_affix_node (a_decls);
	  rfre_value (a_gen_5 -> val);
	  free_affix_node (a_gen_5);
	  rfre_value (a_capplies -> val);
	  free_affix_node (a_capplies);
	  rfre_value (a_cdecls -> val);
	  free_affix_node (a_cdecls);
	  rfre_value (a_gen_4 -> val);
	  free_affix_node (a_gen_4);
	  };
	  };
	  pushq (red_layout);
	}

static void act_notinlist (pos_node *args)
	{ tree_node pnode = args[0] -> node;
	  {
	  affix_node a_nil = make_nil_affix ();
	  affix_node a_ls = new_affix_node ("a_ls");
	  pusha (a_nil);
	  pushi (tag_single);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushpp (args);
	  pushi (0);
	  pushi (20);
	  pushq (update_predicate_node);
	  callq ();
	  pop (9);
	  rfre_value (a_nil -> val);
	  free_affix_node (a_nil);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  };
	  {
	  affix_node a_string = make_string_affix ();
	  affix_node a_list = new_affix_node ("a_list");
	  affix_node a_ls = new_affix_node ("a_ls");
	  pushi (2);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_list);
	  pushi (tag_single);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_notinlist);
	  pushi (1);
	  pusht (pnode);
	  pushq (link_predicate_son);
	  pusha (a_string);
	  pushi (tag_single);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushq (make_affix_link);
	  pushq (pred_notequal);
	  pusha (a_string);
	  pusha (a_list);
	  pushi (2);
	  pushi (tag_compos);
	  pusha (a_ls);
	  pushi (tag_single);
	  pushi (2);
	  pushpp (args);
	  pushi (2);
	  pushi (21);
	  pushq (update_predicate_node);
	  callq ();
	  pop (31);
	  rfre_value (a_string -> val);
	  free_affix_node (a_string);
	  rfre_value (a_list -> val);
	  free_affix_node (a_list);
	  rfre_value (a_ls -> val);
	  free_affix_node (a_ls);
	  };
	}

static void delayed_notinlist (pos_node *args)
	{
	  pos_node paf0 = args[0];
	  pos_node paf1 = args[1];
	  if (!crit_pos_has_value (paf0) || !crit_pos_has_value (paf1))
	     callq();
	  else {
	     paf0 -> delayed = 0;
	     paf1 -> delayed = 0;
	     act_notinlist (args);
	     paf0 -> delayed = 1;
	     paf1 -> delayed = 1;
	  };
	}

static void pred_notinlist ()
	{
	  affix_node paf0 = new_affix_node ("pred_notinlist_paf0");
	  affix_node paf1 = new_affix_node ("pred_notinlist_paf1");
	  pushq (delayed_notinlist);
	  pushq (make_node_delayed);
	  pusha (paf1);
	  pushi (tag_single);
	  pusha (paf0);
	  pushi (tag_single);
	  pushi (2);
	  pushi (0);
	  pushi (19);
	  pushq (make_predicate_node);
	  callq ();
	  pop (10);
	  rfre_value (paf0 -> val);
	  free_affix_node (paf0);
	  rfre_value (paf1 -> val);
	  free_affix_node (paf1);
	  pushq (pred_notinlist);
	};

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
	  pushi (10);
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
	  pushi (18);
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

static void rule_semicolon ()
	{
	  if (iptr_at (';'))
	     { pushi (goal_semicolon);
	       pushq (get_semicolon);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_semicolon);
	}

static void get_semicolon ()
	{
	  if (iptr_at (';'))
	  {
	  pushq (red_semicolon);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (23);
	  pushq (make_normal_node);
	  pushs (";");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_semicolon);
	}

static void red_semicolon ()
	{
	  if (topi () == goal_semicolon)
	     { pop (1);
	       callq ();
	       pushi (goal_semicolon);
	     };
	  pushq (red_semicolon);
	}

static void rule_units ()
	{
	  if (in_lookahead ("DA"))
	     { pushi (goal_units);
	       pushq (get_units);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_units);
	}

static void get_units ()
	{
	  if (iptr_at ('A'))
	  {
	  pushq (red_applysymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (3);
	  pushq (make_normal_node);
	  pushs ("APPLY");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('D'))
	  {
	  pushq (red_declsymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (12);
	  pushq (make_normal_node);
	  pushs ("DECL");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
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
	  if (in_lookahead ("DA"))
	     { pushi (goal_unit);
	       pushq (get_unit);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_unit);
	}

static void get_unit ()
	{
	  if (iptr_at ('A'))
	  {
	  pushq (red_applysymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (3);
	  pushq (make_normal_node);
	  pushs ("APPLY");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('D'))
	  {
	  pushq (red_declsymbol);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (12);
	  pushq (make_normal_node);
	  pushs ("DECL");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
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
	  affix_node a_iapplies = new_affix_node ("a_iapplies");
	  affix_node a_idecls = new_affix_node ("a_idecls");
	  affix_node a_newapplies = new_affix_node ("a_newapplies");
	  affix_node a_newdecls = new_affix_node ("a_newdecls");
	  affix_node a_oldapplies = new_affix_node ("a_oldapplies");
	  affix_node a_olddecls = new_affix_node ("a_olddecls");
	  affix_node a_alldecls = new_affix_node ("a_alldecls");
	  pushq (red_units);
	  pushi (3);
	  pushq (link_son);
	  pusha (a_newapplies);
	  pushi (tag_single);
	  pusha (a_newdecls);
	  pushi (tag_single);
	  pusha (a_iapplies);
	  pushi (tag_single);
	  pusha (a_idecls);
	  pushi (tag_single);
	  pusha (a_alldecls);
	  pushi (tag_single);
	  pushi (5);
	  pushq (make_affix_link);
	  pushq (rule_units);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_semicolon);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_iapplies);
	  pushi (tag_single);
	  pusha (a_idecls);
	  pushi (tag_single);
	  pusha (a_oldapplies);
	  pushi (tag_single);
	  pusha (a_olddecls);
	  pushi (tag_single);
	  pusha (a_alldecls);
	  pushi (tag_single);
	  pushi (5);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_newapplies);
	  pushi (tag_single);
	  pusha (a_newdecls);
	  pushi (tag_single);
	  pusha (a_oldapplies);
	  pushi (tag_single);
	  pusha (a_olddecls);
	  pushi (tag_single);
	  pusha (a_alldecls);
	  pushi (tag_single);
	  pushi (5);
	  pushi (3);
	  pushi (24);
	  pushq (make_normal_node);
	  callq ();
	  pop (50);
	  rfre_value (a_iapplies -> val);
	  free_affix_node (a_iapplies);
	  rfre_value (a_idecls -> val);
	  free_affix_node (a_idecls);
	  rfre_value (a_newapplies -> val);
	  free_affix_node (a_newapplies);
	  rfre_value (a_newdecls -> val);
	  free_affix_node (a_newdecls);
	  rfre_value (a_oldapplies -> val);
	  free_affix_node (a_oldapplies);
	  rfre_value (a_olddecls -> val);
	  free_affix_node (a_olddecls);
	  rfre_value (a_alldecls -> val);
	  free_affix_node (a_alldecls);
	  };
	  };
	  if (on_spine_to_goal (goal_units)){
	  if (iptr_at ('E'))
	  {
	  affix_node a_newapplies = new_affix_node ("a_newapplies");
	  affix_node a_newdecls = new_affix_node ("a_newdecls");
	  affix_node a_oldapplies = new_affix_node ("a_oldapplies");
	  affix_node a_olddecls = new_affix_node ("a_olddecls");
	  affix_node a_alldecls = new_affix_node ("a_alldecls");
	  pushq (red_units);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_newapplies);
	  pushi (tag_single);
	  pusha (a_newdecls);
	  pushi (tag_single);
	  pusha (a_oldapplies);
	  pushi (tag_single);
	  pusha (a_olddecls);
	  pushi (tag_single);
	  pusha (a_alldecls);
	  pushi (tag_single);
	  pushi (5);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pusha (a_newapplies);
	  pushi (tag_single);
	  pusha (a_newdecls);
	  pushi (tag_single);
	  pusha (a_oldapplies);
	  pushi (tag_single);
	  pusha (a_olddecls);
	  pushi (tag_single);
	  pusha (a_alldecls);
	  pushi (tag_single);
	  pushi (5);
	  pushi (1);
	  pushi (25);
	  pushq (make_normal_node);
	  callq ();
	  pop (30);
	  rfre_value (a_newapplies -> val);
	  free_affix_node (a_newapplies);
	  rfre_value (a_newdecls -> val);
	  free_affix_node (a_newdecls);
	  rfre_value (a_oldapplies -> val);
	  free_affix_node (a_oldapplies);
	  rfre_value (a_olddecls -> val);
	  free_affix_node (a_olddecls);
	  rfre_value (a_alldecls -> val);
	  free_affix_node (a_alldecls);
	  };
	  };
	  pushq (red_unit);
	}

void transduce ()
	{
	  affix_node a_out = new_affix_node ("a_out");
	  pushq (dummy_continuation);
	  pushq (increment_nrparses);
	  pusha (a_out);
	  pushq (output_affix);
	  pushq (endofsentence);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_out);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (rule_start);
	  pusha (a_out);
	  pushi (tag_single);
	  pushi (1);
	  pushi (1);	/* one son only */
	  pushi (0);	/* the rootnode */
	  pushq (make_normal_node);
	  init_eag_stddefs (1);
	  callq ();
	  pop (17);
	  rfre_value (a_out -> val);
	  free_affix_node (a_out);
	};

string module_name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return ("lazy");
	       case 1: return ("eag_stddefs");
	       default: panic ("strange module nr %d", mnr);
	     };
	  return (NULL);
	};

char *name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return (lazy_name_from_nodenr (nodenr));
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

