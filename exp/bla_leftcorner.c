/*
   File: bla_leftcorner.c
   Generated on Tue Aug  5 14:53:19 2025
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

 void semipred_endofsentence ();
static void semipred_start ();

string bla_name_from_nodenr (int nodenr)
	{ switch (nodenr)
	     { case 0: return ("root_node");
	       case 1: return ("leaf_node");
	       case 2: return ("semipred_start");
	       default: panic ("strange node %d in bla", nodenr);
	     };
	  return (NULL);
	};

/* code goals for non predicate syntax rules */
#define on_spine_to_goal(x) lc_rel[x * 0 + topi()]
static char lc_rel[] =
	{ };

static void semipred_start ()
	{
	  {
	  value v0 = new_string_value ("Wow, it works!!\n");
	  affix_node a_gen_0 = value_to_affix ("a_gen_0", v0);
	  pushi (1);
	  pushq (link_son);
	  pushq (semipred_endofsentence);
	  pusha (a_gen_0);
	  pushi (tag_single);
	  pushi (1);
	  pushi (1);
	  pushi (2);
	  pushq (make_semipredicate_node);
	  callq ();
	  pop (9);
	  rfre_value (a_gen_0 -> val);
	  free_affix_node (a_gen_0);
	  };
	  pushq (semipred_start);
	};

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
	  pushq (semipred_start);
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
	     { case 0: return ("bla");
	       case 1: return ("eag_stddefs");
	       default: panic ("strange module nr %d", mnr);
	     };
	  return (NULL);
	};

char *name_from_nodenr (int nodenr)
	{ int mnr = modnr_from_nodenr (nodenr);
	  switch (mnr)
	     { case 0: return (bla_name_from_nodenr (nodenr));
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

