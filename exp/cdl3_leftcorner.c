/*
   File: cdl3_leftcorner.c
   Generated on Sun Aug  3 16:28:58 2025
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

static void rule_alternatives ();
static void get_alternatives ();
static void red_alternatives ();
static void rule_alternative ();
static void get_alternative ();
static void red_alternative ();
static void rule_cdl3module ();
static void get_cdl3module ();
static void red_cdl3module ();
static void rule_boldnamelist ();
static void get_boldnamelist ();
static void red_boldnamelist ();
static void rule_boldname ();
static void get_boldname ();
static void red_boldname ();
static void rule_characters ();
static void get_characters ();
static void red_characters ();
static void empv_characters ();
static void emp_characters ();
static void rule_declarations ();
static void get_declarations ();
static void red_declarations ();
static void rule_element ();
static void get_element ();
static void red_element ();
static void rule_globaldeclarations ();
static void get_globaldeclarations ();
static void red_globaldeclarations ();
static void empv_globaldeclarations ();
static void emp_globaldeclarations ();
static void rule_export ();
static void get_export ();
static void red_export ();
static void rule_exportlist ();
static void get_exportlist ();
static void red_exportlist ();
static void rule_formalparameters ();
static void get_formalparameters ();
static void red_formalparameters ();
static void rule_formalparameterlist ();
static void get_formalparameterlist ();
static void red_formalparameterlist ();
static void rule_formalparameter ();
static void get_formalparameter ();
static void red_formalparameter ();
static void rule_expression ();
static void get_expression ();
static void red_expression ();
static void rule_import ();
static void get_import ();
static void red_import ();
static void empv_import ();
static void emp_import ();
static void rule_guardtype ();
static void get_guardtype ();
static void red_guardtype ();
static void rule_memberlist ();
static void get_memberlist ();
static void red_memberlist ();
static void rule_member ();
static void get_member ();
static void red_member ();
static void rule_layout ();
static void get_layout ();
static void red_layout ();
static void empv_layout ();
static void emp_layout ();
static void rule_moduleheader ();
static void get_moduleheader ();
static void red_moduleheader ();
static void rule_namelist ();
static void get_namelist ();
static void red_namelist ();
static void rule_name ();
static void get_name ();
static void red_name ();
static void rule_parameters ();
static void get_parameters ();
static void red_parameters ();
static void rule_parameterlist ();
static void get_parameterlist ();
static void red_parameterlist ();
static void rule_number ();
static void get_number ();
static void red_number ();
static void rule_ruledeclaration ();
static void get_ruledeclaration ();
static void red_ruledeclaration ();
static void rule_ruleheader ();
static void get_ruleheader ();
static void red_ruleheader ();
static void rule_ruletype ();
static void get_ruletype ();
static void red_ruletype ();
static void rule_rulename ();
static void get_rulename ();
static void red_rulename ();
static void rule_typedeclaration ();
static void get_typedeclaration ();
static void red_typedeclaration ();
static void rule_typealternatives ();
static void get_typealternatives ();
static void red_typealternatives ();
static void rule_typealternative ();
static void get_typealternative ();
static void red_typealternative ();
static void rule_text ();
static void get_text ();
static void red_text ();
static void rule_typeinstance ();
static void get_typeinstance ();
static void red_typeinstance ();

string cdl3_name_from_nodenr (int nodenr)
	{ switch (nodenr)
	     { case 0: return ("root_node");
	       case 1: return ("leaf_node");
	       case 2: return ("rule_alternatives");
	       case 3: return ("rule_alternatives");
	       case 4: return ("rule_alternative");
	       case 5: return ("rule_alternative");
	       case 6: return ("rule_alternative");
	       case 7: return ("rule_alternative");
	       case 8: return ("rule_alternative");
	       case 9: return ("rule_cdl3module");
	       case 10: return ("rule_boldnamelist");
	       case 11: return ("rule_boldnamelist");
	       case 12: return ("rule_boldname");
	       case 13: return ("rule_characters");
	       case 14: return ("rule_characters");
	       case 15: return ("rule_characters");
	       case 16: return ("rule_characters");
	       case 17: return ("rule_declarations");
	       case 18: return ("rule_declarations");
	       case 19: return ("rule_declarations");
	       case 20: return ("rule_declarations");
	       case 21: return ("rule_element");
	       case 22: return ("rule_element");
	       case 23: return ("rule_element");
	       case 24: return ("rule_element");
	       case 25: return ("rule_globaldeclarations");
	       case 26: return ("rule_globaldeclarations");
	       case 27: return ("rule_export");
	       case 28: return ("rule_exportlist");
	       case 29: return ("rule_exportlist");
	       case 30: return ("rule_exportlist");
	       case 31: return ("rule_exportlist");
	       case 32: return ("rule_formalparameters");
	       case 33: return ("rule_formalparameters");
	       case 34: return ("rule_formalparameters");
	       case 35: return ("rule_formalparameterlist");
	       case 36: return ("rule_formalparameterlist");
	       case 37: return ("rule_formalparameter");
	       case 38: return ("rule_formalparameter");
	       case 39: return ("rule_formalparameter");
	       case 40: return ("rule_expression");
	       case 41: return ("rule_expression");
	       case 42: return ("rule_import");
	       case 43: return ("rule_import");
	       case 44: return ("rule_guardtype");
	       case 45: return ("rule_guardtype");
	       case 46: return ("rule_memberlist");
	       case 47: return ("rule_memberlist");
	       case 48: return ("rule_memberlist");
	       case 49: return ("rule_member");
	       case 50: return ("rule_member");
	       case 51: return ("rule_member");
	       case 52: return ("rule_member");
	       case 53: return ("rule_layout");
	       case 54: return ("rule_moduleheader");
	       case 55: return ("rule_namelist");
	       case 56: return ("rule_namelist");
	       case 57: return ("rule_name");
	       case 58: return ("rule_parameters");
	       case 59: return ("rule_parameters");
	       case 60: return ("rule_parameters");
	       case 61: return ("rule_parameterlist");
	       case 62: return ("rule_parameterlist");
	       case 63: return ("rule_number");
	       case 64: return ("rule_ruledeclaration");
	       case 65: return ("rule_ruleheader");
	       case 66: return ("rule_ruleheader");
	       case 67: return ("rule_ruletype");
	       case 68: return ("rule_ruletype");
	       case 69: return ("rule_ruletype");
	       case 70: return ("rule_ruletype");
	       case 71: return ("rule_ruletype");
	       case 72: return ("rule_rulename");
	       case 73: return ("rule_rulename");
	       case 74: return ("rule_typedeclaration");
	       case 75: return ("rule_typealternatives");
	       case 76: return ("rule_typealternatives");
	       case 77: return ("rule_typealternative");
	       case 78: return ("rule_typealternative");
	       case 79: return ("rule_typealternative");
	       case 80: return ("rule_typealternative");
	       case 81: return ("rule_text");
	       case 82: return ("rule_typeinstance");
	       default: panic ("strange node %d in cdl3", nodenr);
	     };
	  return (NULL);
	};

/* code goals for non predicate syntax rules */
#define goal_alternatives 0
#define goal_alternative 1
#define goal_cdl3module 2
#define goal_boldnamelist 3
#define goal_boldname 4
#define goal_characters 5
#define goal_declarations 6
#define goal_element 7
#define goal_globaldeclarations 8
#define goal_export 9
#define goal_exportlist 10
#define goal_formalparameters 11
#define goal_formalparameterlist 12
#define goal_formalparameter 13
#define goal_expression 14
#define goal_import 15
#define goal_guardtype 16
#define goal_memberlist 17
#define goal_member 18
#define goal_layout 19
#define goal_moduleheader 20
#define goal_namelist 21
#define goal_name 22
#define goal_parameters 23
#define goal_parameterlist 24
#define goal_number 25
#define goal_ruledeclaration 26
#define goal_ruleheader 27
#define goal_ruletype 28
#define goal_rulename 29
#define goal_typedeclaration 30
#define goal_typealternatives 31
#define goal_typealternative 32
#define goal_text 33
#define goal_typeinstance 34
#define on_spine_to_goal(x) lc_rel[x * 35 + topi()]
static char lc_rel[] =
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
	  0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
	  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 
	  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 
	  0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 
	  0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 
	  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 
	  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
	  0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0, 1, 
	};

static void rule_alternatives ()
	{
	  if (in_lookahead ("/([+abcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_alternatives);
	       pushq (get_alternatives);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_alternatives);
	}

static void get_alternatives ()
	{
	  if (iptr_at ('/'))
	  {
	  pushq (red_alternative);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_alternative);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (2);
	  pushi (7);
	  pushq (make_normal_node);
	  pushs ("/");
	  pushq (parse_terminal);
	  callq ();
	  pop (17);
	  };
	  if (iptr_at ('('))
	  {
	  pushq (red_alternative);
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
	  pushq (rule_alternatives);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (8);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  if (iptr_at ('['))
	  {
	  pushq (red_member);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("]");
	  pushq (parse_terminal);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_guardtype);
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
	  pushi (49);
	  pushq (make_normal_node);
	  pushs ("[");
	  pushq (parse_terminal);
	  callq ();
	  pop (34);
	  };
	  if (iptr_at ('+'))
	  {
	  pushq (red_member);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (52);
	  pushq (make_normal_node);
	  pushs ("+");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  pushq (get_alternatives);
	}

static void red_alternatives ()
	{
	  if (topi () == goal_alternatives)
	     { pop (1);
	       callq ();
	       pushi (goal_alternatives);
	     };
	  pushq (red_alternatives);
	}

static void rule_alternative ()
	{
	  if (in_lookahead ("/([+abcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_alternative);
	       pushq (get_alternative);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_alternative);
	}

static void get_alternative ()
	{
	  if (iptr_at ('/'))
	  {
	  pushq (red_alternative);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_alternative);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (2);
	  pushi (7);
	  pushq (make_normal_node);
	  pushs ("/");
	  pushq (parse_terminal);
	  callq ();
	  pop (17);
	  };
	  if (iptr_at ('('))
	  {
	  pushq (red_alternative);
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
	  pushq (rule_alternatives);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (8);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  if (iptr_at ('['))
	  {
	  pushq (red_member);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("]");
	  pushq (parse_terminal);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_guardtype);
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
	  pushi (49);
	  pushq (make_normal_node);
	  pushs ("[");
	  pushq (parse_terminal);
	  callq ();
	  pop (34);
	  };
	  if (iptr_at ('+'))
	  {
	  pushq (red_member);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (52);
	  pushq (make_normal_node);
	  pushs ("+");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  pushq (get_alternative);
	}

static void red_alternative ()
	{
	  if (topi () == goal_alternative)
	     { pop (1);
	       callq ();
	       pushi (goal_alternative);
	     };
	  if (on_spine_to_goal (goal_alternatives)){
	  if (iptr_at (';'))
	  {
	  pushq (red_alternatives);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_alternatives);
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
	  pushi (2);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_alternatives)){
	  if (in_lookahead (")."))
	  {
	  pushq (red_alternatives);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (3);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_alternative);
	}

static void rule_cdl3module ()
	{
	  if (iptr_at ('M'))
	     { pushi (goal_cdl3module);
	       pushq (get_cdl3module);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_cdl3module);
	}

static void get_cdl3module ()
	{
	  if (iptr_at ('M'))
	  {
	  pushq (red_moduleheader);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (".");
	  pushq (parse_terminal);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_globaldeclarations);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_name);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (4);
	  pushi (54);
	  pushq (make_normal_node);
	  pushs ("MODULE");
	  pushq (parse_terminal);
	  callq ();
	  pop (29);
	  };
	  pushq (get_cdl3module);
	}

static void red_cdl3module ()
	{
	  if (topi () == goal_cdl3module)
	     { pop (1);
	       callq ();
	       pushi (goal_cdl3module);
	     };
	  pushq (red_cdl3module);
	}

static void rule_boldnamelist ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	     { pushi (goal_boldnamelist);
	       pushq (get_boldnamelist);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_boldnamelist);
	}

static void get_boldnamelist ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_boldname);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (12);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  pushq (get_boldnamelist);
	}

static void red_boldnamelist ()
	{
	  if (topi () == goal_boldnamelist)
	     { pop (1);
	       callq ();
	       pushi (goal_boldnamelist);
	     };
	  if (on_spine_to_goal (goal_typedeclaration)){
	  if (iptr_at (':'))
	  {
	  pushq (red_typedeclaration);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (".");
	  pushq (parse_terminal);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_typealternatives);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("::");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (4);
	  pushi (74);
	  pushq (make_normal_node);
	  callq ();
	  pop (29);
	  };
	  };
	  pushq (red_boldnamelist);
	}

static void rule_boldname ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	     { pushi (goal_boldname);
	       pushq (get_boldname);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_boldname);
	}

static void get_boldname ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_boldname);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (12);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  pushq (get_boldname);
	}

static void red_boldname ()
	{
	  if (topi () == goal_boldname)
	     { pop (1);
	       callq ();
	       pushi (goal_boldname);
	     };
	  if (on_spine_to_goal (goal_boldnamelist)){
	  if (iptr_at (','))
	  {
	  pushq (red_boldnamelist);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_boldnamelist);
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
	  pushi (3);
	  pushi (10);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_boldnamelist)){
	  if (iptr_at (':'))
	  {
	  pushq (red_boldnamelist);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (11);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  if (on_spine_to_goal (goal_exportlist)){
	  if (iptr_at (','))
	  {
	  pushq (red_exportlist);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_exportlist);
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
	  pushi (3);
	  pushi (30);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_exportlist)){
	  if (iptr_at ('.'))
	  {
	  pushq (red_exportlist);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (31);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  if (on_spine_to_goal (goal_typealternative)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ"))
	  {
	  pushq (red_typealternative);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_typealternative);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (79);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  if (on_spine_to_goal (goal_typealternative)){
	  if (in_lookahead (";."))
	  {
	  pushq (red_typealternative);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (80);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_boldname);
	}

static void rule_characters ()
	{
	  if (in_lookahead ("\\abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVW\
XYZ0123456789`~!@#$%^&*()_-+=|[{}];:\',.<>/? "))
	     { pushi (goal_characters);
	       pushq (get_characters);
	       callq ();
	       pop(2);
	     };
	  if (iptr_at ('\"'))
	     { pushq (empv_characters);
	       callq ();
	       pop(1);
	     };
	  pushq (rule_characters);
	}

static void empv_characters ()
	{
	  {
	  pushi (0);
	  pushi (0);
	  pushi (16);
	  pushq (make_normal_node);
	  callq ();
	  pop (4);
	  };
	  pushq (empv_characters);
	}

static void emp_characters ()
	{
	  {
	  pushi (0);
	  pushi (0);
	  pushi (16);
	  pushq (make_normal_node);
	  callq ();
	  pop (4);
	  };
	  pushq (emp_characters);
	}

static void get_characters ()
	{
	  if (iptr_at ('\\'))
	  {
	  affix_node a_l = new_affix_node ("a_l");
	  pushq (red_characters);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_characters);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_l);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("\"\\nrb");
	  pushq (parse_set);
	  pushi (0);
	  pushi (2);
	  pushi (13);
	  pushq (make_normal_node);
	  pushs ("\\");
	  pushq (parse_terminal);
	  callq ();
	  pop (21);
	  rfre_value (a_l -> val);
	  free_affix_node (a_l);
	  };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ0123456789"))
	  {
	  affix_node a_c = new_affix_node ("a_c");
	  pushq (red_characters);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_characters);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_c);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ0123456789");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (14);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_c -> val);
	  free_affix_node (a_c);
	  };
	  if (in_lookahead ("`~!@#$%^&*()_-+=|[{}];:\',.<>/? "))
	  {
	  affix_node a_c = new_affix_node ("a_c");
	  pushq (red_characters);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_characters);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_c);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("`~!@#$%^&*()_-+=|[{}];:\',.<>/? ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (15);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_c -> val);
	  free_affix_node (a_c);
	  };
	  pushq (get_characters);
	}

static void red_characters ()
	{
	  if (topi () == goal_characters)
	     { pop (1);
	       callq ();
	       pushi (goal_characters);
	     };
	  pushq (red_characters);
	}

static void rule_declarations ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	     { pushi (goal_declarations);
	       pushq (get_declarations);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_declarations);
	}

static void get_declarations ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_boldname);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (12);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  if (iptr_at ('P'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (67);
	  pushq (make_normal_node);
	  pushs ("PRELUDE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('F'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (68);
	  pushq (make_normal_node);
	  pushs ("FUNCTION");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('A'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (69);
	  pushq (make_normal_node);
	  pushs ("ACTION");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('T'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (70);
	  pushq (make_normal_node);
	  pushs ("TEST");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('P'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (71);
	  pushq (make_normal_node);
	  pushs ("PRED");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
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

static void rule_element ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ123456789\""))
	     { pushi (goal_element);
	       pushq (get_element);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_element);
	}

static void get_element ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  if (in_lookahead ("123456789"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_d = new_affix_node ("a_d");
	  pushq (red_number);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("123456789");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (63);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  };
	  if (iptr_at ('\"'))
	  {
	  pushq (red_text);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("\"");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_characters);
	  pushi (0);
	  pushi (2);
	  pushi (81);
	  pushq (make_normal_node);
	  pushs ("\"");
	  pushq (parse_terminal);
	  callq ();
	  pop (19);
	  };
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_d = new_affix_node ("a_d");
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_typeinstance);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (3);
	  pushi (82);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  pushq (get_element);
	}

static void red_element ()
	{
	  if (topi () == goal_element)
	     { pop (1);
	       callq ();
	       pushi (goal_element);
	     };
	  if (on_spine_to_goal (goal_expression)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ123456789\""))
	  {
	  pushq (red_expression);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (40);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  if (on_spine_to_goal (goal_expression)){
	  if (in_lookahead ("=-],/)"))
	  {
	  pushq (red_expression);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (41);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_element);
	}

static void rule_globaldeclarations ()
	{
	  if (iptr_at ('='))
	     { pushi (goal_globaldeclarations);
	       pushq (get_globaldeclarations);
	       callq ();
	       pop(2);
	     };
	  if (iptr_at ('.'))
	     { pushq (empv_globaldeclarations);
	       callq ();
	       pop(1);
	     };
	  pushq (rule_globaldeclarations);
	}

static void empv_globaldeclarations ()
	{
	  {
	  pushi (0);
	  pushi (0);
	  pushi (26);
	  pushq (make_normal_node);
	  callq ();
	  pop (4);
	  };
	  pushq (empv_globaldeclarations);
	}

static void emp_globaldeclarations ()
	{
	  {
	  pushi (0);
	  pushi (0);
	  pushi (26);
	  pushq (make_normal_node);
	  callq ();
	  pop (4);
	  };
	  pushq (emp_globaldeclarations);
	}

static void get_globaldeclarations ()
	{
	  if (iptr_at ('='))
	  {
	  pushq (red_globaldeclarations);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_typealternative);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (2);
	  pushi (25);
	  pushq (make_normal_node);
	  pushs ("=");
	  pushq (parse_terminal);
	  callq ();
	  pop (17);
	  };
	  pushq (get_globaldeclarations);
	}

static void red_globaldeclarations ()
	{
	  if (topi () == goal_globaldeclarations)
	     { pop (1);
	       callq ();
	       pushi (goal_globaldeclarations);
	     };
	  pushq (red_globaldeclarations);
	}

static void rule_export ()
	{
	  if (iptr_at ('D'))
	     { pushi (goal_export);
	       pushq (get_export);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_export);
	}

static void get_export ()
	{
	  if (iptr_at ('D'))
	  {
	  pushq (red_export);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (".");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_exportlist);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (27);
	  pushq (make_normal_node);
	  pushs ("DEFINES");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  pushq (get_export);
	}

static void red_export ()
	{
	  if (topi () == goal_export)
	     { pop (1);
	       callq ();
	       pushi (goal_export);
	     };
	  pushq (red_export);
	}

static void rule_exportlist ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	     { pushi (goal_exportlist);
	       pushq (get_exportlist);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_exportlist);
	}

static void get_exportlist ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_boldname);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (12);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  if (iptr_at ('P'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (67);
	  pushq (make_normal_node);
	  pushs ("PRELUDE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('F'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (68);
	  pushq (make_normal_node);
	  pushs ("FUNCTION");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('A'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (69);
	  pushq (make_normal_node);
	  pushs ("ACTION");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('T'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (70);
	  pushq (make_normal_node);
	  pushs ("TEST");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('P'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (71);
	  pushq (make_normal_node);
	  pushs ("PRED");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_exportlist);
	}

static void red_exportlist ()
	{
	  if (topi () == goal_exportlist)
	     { pop (1);
	       callq ();
	       pushi (goal_exportlist);
	     };
	  pushq (red_exportlist);
	}

static void rule_formalparameters ()
	{
	  if (in_lookahead ("/>ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	     { pushi (goal_formalparameters);
	       pushq (get_formalparameters);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_formalparameters);
	}

static void get_formalparameters ()
	{
	  if (iptr_at ('/'))
	  {
	  pushq (red_formalparameters);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_formalparameterlist);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (2);
	  pushi (34);
	  pushq (make_normal_node);
	  pushs ("/");
	  pushq (parse_terminal);
	  callq ();
	  pop (17);
	  };
	  if (iptr_at ('>'))
	  {
	  pushq (red_formalparameter);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (">");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_typeinstance);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (37);
	  pushq (make_normal_node);
	  pushs (">");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  if (iptr_at ('>'))
	  {
	  pushq (red_formalparameter);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_typeinstance);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (2);
	  pushi (38);
	  pushq (make_normal_node);
	  pushs (">");
	  pushq (parse_terminal);
	  callq ();
	  pop (17);
	  };
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_d = new_affix_node ("a_d");
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_typeinstance);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (3);
	  pushi (82);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  pushq (get_formalparameters);
	}

static void red_formalparameters ()
	{
	  if (topi () == goal_formalparameters)
	     { pop (1);
	       callq ();
	       pushi (goal_formalparameters);
	     };
	  pushq (red_formalparameters);
	}

static void rule_formalparameterlist ()
	{
	  if (in_lookahead (">ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	     { pushi (goal_formalparameterlist);
	       pushq (get_formalparameterlist);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_formalparameterlist);
	}

static void get_formalparameterlist ()
	{
	  if (iptr_at ('>'))
	  {
	  pushq (red_formalparameter);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (">");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_typeinstance);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (37);
	  pushq (make_normal_node);
	  pushs (">");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  if (iptr_at ('>'))
	  {
	  pushq (red_formalparameter);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_typeinstance);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (2);
	  pushi (38);
	  pushq (make_normal_node);
	  pushs (">");
	  pushq (parse_terminal);
	  callq ();
	  pop (17);
	  };
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_d = new_affix_node ("a_d");
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_typeinstance);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (3);
	  pushi (82);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  pushq (get_formalparameterlist);
	}

static void red_formalparameterlist ()
	{
	  if (topi () == goal_formalparameterlist)
	     { pop (1);
	       callq ();
	       pushi (goal_formalparameterlist);
	     };
	  if (on_spine_to_goal (goal_formalparameters)){
	  if (iptr_at ('/'))
	  {
	  pushq (red_formalparameters);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_formalparameterlist);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("/");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (32);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_formalparameters)){
	  if (iptr_at (')'))
	  {
	  pushq (red_formalparameters);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (33);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_formalparameterlist);
	}

static void rule_formalparameter ()
	{
	  if (in_lookahead (">ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	     { pushi (goal_formalparameter);
	       pushq (get_formalparameter);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_formalparameter);
	}

static void get_formalparameter ()
	{
	  if (iptr_at ('>'))
	  {
	  pushq (red_formalparameter);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (">");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_typeinstance);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (37);
	  pushq (make_normal_node);
	  pushs (">");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  if (iptr_at ('>'))
	  {
	  pushq (red_formalparameter);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_typeinstance);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (2);
	  pushi (38);
	  pushq (make_normal_node);
	  pushs (">");
	  pushq (parse_terminal);
	  callq ();
	  pop (17);
	  };
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_d = new_affix_node ("a_d");
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_typeinstance);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (3);
	  pushi (82);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  pushq (get_formalparameter);
	}

static void red_formalparameter ()
	{
	  if (topi () == goal_formalparameter)
	     { pop (1);
	       callq ();
	       pushi (goal_formalparameter);
	     };
	  if (on_spine_to_goal (goal_formalparameterlist)){
	  if (iptr_at (','))
	  {
	  pushq (red_formalparameterlist);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_formalparameterlist);
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
	  pushi (3);
	  pushi (35);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_formalparameterlist)){
	  if (in_lookahead ("/)"))
	  {
	  pushq (red_formalparameterlist);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (36);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_formalparameter);
	}

static void rule_expression ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ123456789\""))
	     { pushi (goal_expression);
	       pushq (get_expression);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_expression);
	}

static void get_expression ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  if (in_lookahead ("123456789"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_d = new_affix_node ("a_d");
	  pushq (red_number);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("123456789");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (63);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  };
	  if (iptr_at ('\"'))
	  {
	  pushq (red_text);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("\"");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_characters);
	  pushi (0);
	  pushi (2);
	  pushi (81);
	  pushq (make_normal_node);
	  pushs ("\"");
	  pushq (parse_terminal);
	  callq ();
	  pop (19);
	  };
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_d = new_affix_node ("a_d");
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_typeinstance);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (3);
	  pushi (82);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
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
	  if (on_spine_to_goal (goal_parameterlist)){
	  if (iptr_at (','))
	  {
	  pushq (red_parameterlist);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_parameterlist);
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
	  pushi (3);
	  pushi (61);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_parameterlist)){
	  if (in_lookahead ("/)"))
	  {
	  pushq (red_parameterlist);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (62);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_expression);
	}

static void rule_import ()
	{
	  if (iptr_at ('U'))
	     { pushi (goal_import);
	       pushq (get_import);
	       callq ();
	       pop(2);
	     };
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	     { pushq (empv_import);
	       callq ();
	       pop(1);
	     };
	  pushq (rule_import);
	}

static void empv_import ()
	{
	  {
	  pushi (0);
	  pushi (0);
	  pushi (43);
	  pushq (make_normal_node);
	  callq ();
	  pop (4);
	  };
	  pushq (empv_import);
	}

static void emp_import ()
	{
	  {
	  pushi (0);
	  pushi (0);
	  pushi (43);
	  pushq (make_normal_node);
	  callq ();
	  pop (4);
	  };
	  pushq (emp_import);
	}

static void get_import ()
	{
	  if (iptr_at ('U'))
	  {
	  pushq (red_import);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (".");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_namelist);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (42);
	  pushq (make_normal_node);
	  pushs ("USES");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  pushq (get_import);
	}

static void red_import ()
	{
	  if (topi () == goal_import)
	     { pop (1);
	       callq ();
	       pushi (goal_import);
	     };
	  pushq (red_import);
	}

static void rule_guardtype ()
	{
	  if (in_lookahead ("=-"))
	     { pushi (goal_guardtype);
	       pushq (get_guardtype);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_guardtype);
	}

static void get_guardtype ()
	{
	  if (iptr_at ('='))
	  {
	  pushq (red_guardtype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (44);
	  pushq (make_normal_node);
	  pushs ("=");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('-'))
	  {
	  pushq (red_guardtype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (45);
	  pushq (make_normal_node);
	  pushs ("->");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_guardtype);
	}

static void red_guardtype ()
	{
	  if (topi () == goal_guardtype)
	     { pop (1);
	       callq ();
	       pushi (goal_guardtype);
	     };
	  pushq (red_guardtype);
	}

static void rule_memberlist ()
	{
	  if (in_lookahead ("([+abcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_memberlist);
	       pushq (get_memberlist);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_memberlist);
	}

static void get_memberlist ()
	{
	  if (iptr_at ('('))
	  {
	  pushq (red_memberlist);
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
	  pushq (rule_alternatives);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (3);
	  pushi (48);
	  pushq (make_normal_node);
	  pushs ("(");
	  pushq (parse_terminal);
	  callq ();
	  pop (24);
	  };
	  if (iptr_at ('['))
	  {
	  pushq (red_member);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("]");
	  pushq (parse_terminal);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_guardtype);
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
	  pushi (49);
	  pushq (make_normal_node);
	  pushs ("[");
	  pushq (parse_terminal);
	  callq ();
	  pop (34);
	  };
	  if (iptr_at ('+'))
	  {
	  pushq (red_member);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (52);
	  pushq (make_normal_node);
	  pushs ("+");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  pushq (get_memberlist);
	}

static void red_memberlist ()
	{
	  if (topi () == goal_memberlist)
	     { pop (1);
	       callq ();
	       pushi (goal_memberlist);
	     };
	  pushq (red_memberlist);
	}

static void rule_member ()
	{
	  if (in_lookahead ("[+abcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_member);
	       pushq (get_member);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_member);
	}

static void get_member ()
	{
	  if (iptr_at ('['))
	  {
	  pushq (red_member);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("]");
	  pushq (parse_terminal);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_expression);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_guardtype);
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
	  pushi (49);
	  pushq (make_normal_node);
	  pushs ("[");
	  pushq (parse_terminal);
	  callq ();
	  pop (34);
	  };
	  if (iptr_at ('+'))
	  {
	  pushq (red_member);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (52);
	  pushq (make_normal_node);
	  pushs ("+");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  pushq (get_member);
	}

static void red_member ()
	{
	  if (topi () == goal_member)
	     { pop (1);
	       callq ();
	       pushi (goal_member);
	     };
	  if (on_spine_to_goal (goal_alternative)){
	  if (iptr_at (','))
	  {
	  pushq (red_alternative);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_memberlist);
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
	  pushi (3);
	  pushi (4);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_alternative)){
	  if (iptr_at ('/'))
	  {
	  pushq (red_alternative);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_alternative);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("/");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (5);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_alternative)){
	  if (in_lookahead (";)."))
	  {
	  pushq (red_alternative);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (6);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  if (on_spine_to_goal (goal_memberlist)){
	  if (iptr_at (','))
	  {
	  pushq (red_memberlist);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_memberlist);
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
	  pushi (3);
	  pushi (46);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_memberlist)){
	  if (iptr_at ('/'))
	  {
	  pushq (red_memberlist);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_alternative);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("/");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (47);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  pushq (red_member);
	}

static void rule_layout ()
	{
	  if (in_lookahead (" \n\t"))
	     { pushi (goal_layout);
	       pushq (get_layout);
	       callq ();
	       pop(2);
	     };
	  if (in_lookahead ("/([+abcdefghijklmnopqrstuvwxyz;ABCDEFGHIJKLMNOPQRS\
TUVWXYZ,>123456789\"=.\204:)-]"))
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
	  pushs (" \n\t");
	  pushq (fail_if_iptr_at_set);
	  pushi (0);
	  pushi (1);
	  pushi (53);
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
	  pushi (53);
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
	  if (in_lookahead (" \n\t"))
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
	  pushs (" \n\t");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (1);
	  pushi (53);
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
	  pushq (red_layout);
	}

static void rule_moduleheader ()
	{
	  if (iptr_at ('M'))
	     { pushi (goal_moduleheader);
	       pushq (get_moduleheader);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_moduleheader);
	}

static void get_moduleheader ()
	{
	  if (iptr_at ('M'))
	  {
	  pushq (red_moduleheader);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (".");
	  pushq (parse_terminal);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_globaldeclarations);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_name);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (4);
	  pushi (54);
	  pushq (make_normal_node);
	  pushs ("MODULE");
	  pushq (parse_terminal);
	  callq ();
	  pop (29);
	  };
	  pushq (get_moduleheader);
	}

static void red_moduleheader ()
	{
	  if (topi () == goal_moduleheader)
	     { pop (1);
	       callq ();
	       pushi (goal_moduleheader);
	     };
	  if (on_spine_to_goal (goal_cdl3module)){
	  if (iptr_at ('D'))
	  {
	  pushq (red_cdl3module);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_declarations);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_import);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_export);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (4);
	  pushi (9);
	  pushq (make_normal_node);
	  callq ();
	  pop (25);
	  };
	  };
	  pushq (red_moduleheader);
	}

static void rule_namelist ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_namelist);
	       pushq (get_namelist);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_namelist);
	}

static void get_namelist ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  pushq (get_namelist);
	}

static void red_namelist ()
	{
	  if (topi () == goal_namelist)
	     { pop (1);
	       callq ();
	       pushi (goal_namelist);
	     };
	  pushq (red_namelist);
	}

static void rule_name ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_name);
	       pushq (get_name);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_name);
	}

static void get_name ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  pushq (get_name);
	}

static void red_name ()
	{
	  if (topi () == goal_name)
	     { pop (1);
	       callq ();
	       pushi (goal_name);
	     };
	  if (on_spine_to_goal (goal_element)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ123456789\"=-],/)"))
	  {
	  pushq (red_element);
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
	  if (on_spine_to_goal (goal_namelist)){
	  if (iptr_at (','))
	  {
	  pushq (red_namelist);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_namelist);
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
	  pushi (3);
	  pushi (55);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_namelist)){
	  if (iptr_at ('.'))
	  {
	  pushq (red_namelist);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (56);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  if (on_spine_to_goal (goal_rulename)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  pushq (red_rulename);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_rulename);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (72);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  if (on_spine_to_goal (goal_rulename)){
	  if (in_lookahead ("(,/;.:)"))
	  {
	  pushq (red_rulename);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (73);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  if (on_spine_to_goal (goal_typealternative)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ"))
	  {
	  pushq (red_typealternative);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_typealternative);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (77);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  if (on_spine_to_goal (goal_typealternative)){
	  if (in_lookahead (";."))
	  {
	  pushq (red_typealternative);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (78);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_name);
	}

static void rule_parameters ()
	{
	  if (in_lookahead ("/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVW\
XYZ123456789\""))
	     { pushi (goal_parameters);
	       pushq (get_parameters);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_parameters);
	}

static void get_parameters ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  if (iptr_at ('/'))
	  {
	  pushq (red_parameters);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_parameterlist);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (2);
	  pushi (60);
	  pushq (make_normal_node);
	  pushs ("/");
	  pushq (parse_terminal);
	  callq ();
	  pop (17);
	  };
	  if (in_lookahead ("123456789"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_d = new_affix_node ("a_d");
	  pushq (red_number);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("123456789");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (63);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  };
	  if (iptr_at ('\"'))
	  {
	  pushq (red_text);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("\"");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_characters);
	  pushi (0);
	  pushi (2);
	  pushi (81);
	  pushq (make_normal_node);
	  pushs ("\"");
	  pushq (parse_terminal);
	  callq ();
	  pop (19);
	  };
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_d = new_affix_node ("a_d");
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_typeinstance);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (3);
	  pushi (82);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  pushq (get_parameters);
	}

static void red_parameters ()
	{
	  if (topi () == goal_parameters)
	     { pop (1);
	       callq ();
	       pushi (goal_parameters);
	     };
	  pushq (red_parameters);
	}

static void rule_parameterlist ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ123456789\""))
	     { pushi (goal_parameterlist);
	       pushq (get_parameterlist);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_parameterlist);
	}

static void get_parameterlist ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  if (in_lookahead ("123456789"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_d = new_affix_node ("a_d");
	  pushq (red_number);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("123456789");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (63);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  };
	  if (iptr_at ('\"'))
	  {
	  pushq (red_text);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("\"");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_characters);
	  pushi (0);
	  pushi (2);
	  pushi (81);
	  pushq (make_normal_node);
	  pushs ("\"");
	  pushq (parse_terminal);
	  callq ();
	  pop (19);
	  };
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_d = new_affix_node ("a_d");
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_typeinstance);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (3);
	  pushi (82);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  pushq (get_parameterlist);
	}

static void red_parameterlist ()
	{
	  if (topi () == goal_parameterlist)
	     { pop (1);
	       callq ();
	       pushi (goal_parameterlist);
	     };
	  if (on_spine_to_goal (goal_parameters)){
	  if (iptr_at ('/'))
	  {
	  pushq (red_parameters);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_parameterlist);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("/");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (3);
	  pushi (58);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_parameters)){
	  if (iptr_at (')'))
	  {
	  pushq (red_parameters);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (59);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_parameterlist);
	}

static void rule_number ()
	{
	  if (in_lookahead ("123456789"))
	     { pushi (goal_number);
	       pushq (get_number);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_number);
	}

static void get_number ()
	{
	  if (in_lookahead ("123456789"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_d = new_affix_node ("a_d");
	  pushq (red_number);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("123456789");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (63);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
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
	  if (on_spine_to_goal (goal_element)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ123456789\"=-],/)"))
	  {
	  pushq (red_element);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (23);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_number);
	}

static void rule_ruledeclaration ()
	{
	  if (in_lookahead ("PFAT"))
	     { pushi (goal_ruledeclaration);
	       pushq (get_ruledeclaration);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_ruledeclaration);
	}

static void get_ruledeclaration ()
	{
	  if (iptr_at ('P'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (67);
	  pushq (make_normal_node);
	  pushs ("PRELUDE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('F'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (68);
	  pushq (make_normal_node);
	  pushs ("FUNCTION");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('A'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (69);
	  pushq (make_normal_node);
	  pushs ("ACTION");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('T'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (70);
	  pushq (make_normal_node);
	  pushs ("TEST");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('P'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (71);
	  pushq (make_normal_node);
	  pushs ("PRED");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_ruledeclaration);
	}

static void red_ruledeclaration ()
	{
	  if (topi () == goal_ruledeclaration)
	     { pop (1);
	       callq ();
	       pushi (goal_ruledeclaration);
	     };
	  if (on_spine_to_goal (goal_declarations)){
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  pushq (red_declarations);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_declarations);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (17);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  if (on_spine_to_goal (goal_declarations)){
	  if (iptr_at ('\204'))
	  {
	  pushq (red_declarations);
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
	  pushq (red_ruledeclaration);
	}

static void rule_ruleheader ()
	{
	  if (in_lookahead ("PFAT"))
	     { pushi (goal_ruleheader);
	       pushq (get_ruleheader);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_ruleheader);
	}

static void get_ruleheader ()
	{
	  if (iptr_at ('P'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (67);
	  pushq (make_normal_node);
	  pushs ("PRELUDE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('F'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (68);
	  pushq (make_normal_node);
	  pushs ("FUNCTION");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('A'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (69);
	  pushq (make_normal_node);
	  pushs ("ACTION");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('T'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (70);
	  pushq (make_normal_node);
	  pushs ("TEST");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('P'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (71);
	  pushq (make_normal_node);
	  pushs ("PRED");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_ruleheader);
	}

static void red_ruleheader ()
	{
	  if (topi () == goal_ruleheader)
	     { pop (1);
	       callq ();
	       pushi (goal_ruleheader);
	     };
	  if (on_spine_to_goal (goal_exportlist)){
	  if (iptr_at (','))
	  {
	  pushq (red_exportlist);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_exportlist);
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
	  pushi (3);
	  pushi (28);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_exportlist)){
	  if (iptr_at ('.'))
	  {
	  pushq (red_exportlist);
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
	  if (on_spine_to_goal (goal_ruledeclaration)){
	  if (iptr_at (':'))
	  {
	  pushq (red_ruledeclaration);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (".");
	  pushq (parse_terminal);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_alternatives);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (":");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (4);
	  pushi (64);
	  pushq (make_normal_node);
	  callq ();
	  pop (29);
	  };
	  };
	  pushq (red_ruleheader);
	}

static void rule_ruletype ()
	{
	  if (in_lookahead ("PFAT"))
	     { pushi (goal_ruletype);
	       pushq (get_ruletype);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_ruletype);
	}

static void get_ruletype ()
	{
	  if (iptr_at ('P'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (67);
	  pushq (make_normal_node);
	  pushs ("PRELUDE");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('F'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (68);
	  pushq (make_normal_node);
	  pushs ("FUNCTION");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('A'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (69);
	  pushq (make_normal_node);
	  pushs ("ACTION");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('T'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (70);
	  pushq (make_normal_node);
	  pushs ("TEST");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  if (iptr_at ('P'))
	  {
	  pushq (red_ruletype);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (0);
	  pushi (1);
	  pushi (71);
	  pushq (make_normal_node);
	  pushs ("PRED");
	  pushq (parse_terminal);
	  callq ();
	  pop (12);
	  };
	  pushq (get_ruletype);
	}

static void red_ruletype ()
	{
	  if (topi () == goal_ruletype)
	     { pop (1);
	       callq ();
	       pushi (goal_ruletype);
	     };
	  if (on_spine_to_goal (goal_ruleheader)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  pushq (red_ruleheader);
	  pushi (5);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (")");
	  pushq (parse_terminal);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_formalparameters);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("(");
	  pushq (parse_terminal);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_rulename);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (5);
	  pushi (65);
	  pushq (make_normal_node);
	  callq ();
	  pop (34);
	  };
	  };
	  if (on_spine_to_goal (goal_ruleheader)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  pushq (red_ruleheader);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_rulename);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (66);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  pushq (red_ruletype);
	}

static void rule_rulename ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	     { pushi (goal_rulename);
	       pushq (get_rulename);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_rulename);
	}

static void get_rulename ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  pushq (get_rulename);
	}

static void red_rulename ()
	{
	  if (topi () == goal_rulename)
	     { pop (1);
	       callq ();
	       pushi (goal_rulename);
	     };
	  if (on_spine_to_goal (goal_member)){
	  if (iptr_at ('('))
	  {
	  pushq (red_member);
	  pushi (4);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (")");
	  pushq (parse_terminal);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_parameters);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("(");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (4);
	  pushi (50);
	  pushq (make_normal_node);
	  callq ();
	  pop (29);
	  };
	  };
	  if (on_spine_to_goal (goal_member)){
	  if (in_lookahead (",/;)."))
	  {
	  pushq (red_member);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (51);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_rulename);
	}

static void rule_typedeclaration ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	     { pushi (goal_typedeclaration);
	       pushq (get_typedeclaration);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_typedeclaration);
	}

static void get_typedeclaration ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_boldname);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (12);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  pushq (get_typedeclaration);
	}

static void red_typedeclaration ()
	{
	  if (topi () == goal_typedeclaration)
	     { pop (1);
	       callq ();
	       pushi (goal_typedeclaration);
	     };
	  if (on_spine_to_goal (goal_declarations)){
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  pushq (red_declarations);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_declarations);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (19);
	  pushq (make_normal_node);
	  callq ();
	  pop (15);
	  };
	  };
	  if (on_spine_to_goal (goal_declarations)){
	  if (iptr_at ('\204'))
	  {
	  pushq (red_declarations);
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
	  pushq (red_typedeclaration);
	}

static void rule_typealternatives ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ"))
	     { pushi (goal_typealternatives);
	       pushq (get_typealternatives);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_typealternatives);
	}

static void get_typealternatives ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_boldname);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (12);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  pushq (get_typealternatives);
	}

static void red_typealternatives ()
	{
	  if (topi () == goal_typealternatives)
	     { pop (1);
	       callq ();
	       pushi (goal_typealternatives);
	     };
	  pushq (red_typealternatives);
	}

static void rule_typealternative ()
	{
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ"))
	     { pushi (goal_typealternative);
	       pushq (get_typealternative);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_typealternative);
	}

static void get_typealternative ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_boldname);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (2);
	  pushi (12);
	  pushq (make_normal_node);
	  callq ();
	  pop (19);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyz"))
	  {
	  affix_node a_r = new_affix_node ("a_r");
	  affix_node a_h = new_affix_node ("a_h");
	  pushq (red_name);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_r);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_h);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("abcdefghijklmnopqrstuvwxyz");
	  pushq (parse_set);
	  pushi (0);
	  pushi (3);
	  pushi (57);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_r -> val);
	  free_affix_node (a_r);
	  rfre_value (a_h -> val);
	  free_affix_node (a_h);
	  };
	  pushq (get_typealternative);
	}

static void red_typealternative ()
	{
	  if (topi () == goal_typealternative)
	     { pop (1);
	       callq ();
	       pushi (goal_typealternative);
	     };
	  if (on_spine_to_goal (goal_typealternatives)){
	  if (iptr_at (';'))
	  {
	  pushq (red_typealternatives);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_typealternatives);
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
	  pushi (75);
	  pushq (make_normal_node);
	  callq ();
	  pop (22);
	  };
	  };
	  if (on_spine_to_goal (goal_typealternatives)){
	  if (iptr_at ('.'))
	  {
	  pushq (red_typealternatives);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (76);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_typealternative);
	}

static void rule_text ()
	{
	  if (iptr_at ('\"'))
	     { pushi (goal_text);
	       pushq (get_text);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_text);
	}

static void get_text ()
	{
	  if (iptr_at ('\"'))
	  {
	  pushq (red_text);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs ("\"");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_characters);
	  pushi (0);
	  pushi (2);
	  pushi (81);
	  pushq (make_normal_node);
	  pushs ("\"");
	  pushq (parse_terminal);
	  callq ();
	  pop (19);
	  };
	  pushq (get_text);
	}

static void red_text ()
	{
	  if (topi () == goal_text)
	     { pop (1);
	       callq ();
	       pushi (goal_text);
	     };
	  if (on_spine_to_goal (goal_element)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ123456789\"=-],/)"))
	  {
	  pushq (red_element);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (24);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  pushq (red_text);
	}

static void rule_typeinstance ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	     { pushi (goal_typeinstance);
	       pushq (get_typeinstance);
	       callq ();
	       pop(2);
	     };
	  pushq (rule_typeinstance);
	}

static void get_typeinstance ()
	{
	  if (in_lookahead ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	  {
	  affix_node a_d = new_affix_node ("a_d");
	  affix_node a_n = new_affix_node ("a_n");
	  pushq (red_typeinstance);
	  pushi (3);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushi (2);
	  pushq (link_son);
	  pusha (a_d);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("0123456789");
	  pushq (parse_set_star_strict);
	  pushi (1);
	  pushq (link_son);
	  pusha (a_n);
	  pushi (tag_single);
	  pushi (1);
	  pushq (make_affix_link);
	  pushq (make_leaf_node);
	  pushs ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	  pushq (parse_set_plus_strict);
	  pushi (0);
	  pushi (3);
	  pushi (82);
	  pushq (make_normal_node);
	  callq ();
	  pop (28);
	  rfre_value (a_d -> val);
	  free_affix_node (a_d);
	  rfre_value (a_n -> val);
	  free_affix_node (a_n);
	  };
	  pushq (get_typeinstance);
	}

static void red_typeinstance ()
	{
	  if (topi () == goal_typeinstance)
	     { pop (1);
	       callq ();
	       pushi (goal_typeinstance);
	     };
	  if (on_spine_to_goal (goal_element)){
	  if (in_lookahead ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX\
YZ123456789\"=-],/)"))
	  {
	  pushq (red_element);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (1);
	  pushi (22);
	  pushq (make_normal_node);
	  callq ();
	  pop (10);
	  };
	  };
	  if (on_spine_to_goal (goal_formalparameter)){
	  if (iptr_at ('>'))
	  {
	  pushq (red_formalparameter);
	  pushi (2);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (rule_layout);
	  pushs (">");
	  pushq (parse_terminal);
	  pushi (1);
	  pushq (link_son);
	  pushi (0);
	  pushq (make_affix_link);
	  pushq (exchange_top);
	  pushi (0);
	  pushi (2);
	  pushi (39);
	  pushq (make_normal_node);
	  callq ();
	  pop (17);
	  };
	  };
	  pushq (red_typeinstance);
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
	  pushq (rule_cdl3module);
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
	     { case 0: return ("cdl3");
	       case 1: return ("eag_stddefs");
	       default: panic ("strange module nr %d", mnr);
	     };
	  return (NULL);
	};

char *name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return (cdl3_name_from_nodenr (nodenr));
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

