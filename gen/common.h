/*
   File: common.h
   Defines all routines that are common to all parser generators.
  
   CVS ID: "$Id: common.h,v 1.3 2004/12/26 00:48:44 marcs Exp $"
*/
#ifndef IncCommon
#define IncCommon

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* local includes */
#include "tree.h"

/* coding support */
extern FILE *out;
void code_string (char *format, ...);
void code_line (char *format, ...);
void open_output_file (string basename, string infix, string suffix);
void close_output_file ();

void generate_std_includes (string_list predefs);
string rule_qualifier (rule srule);
void code_nodenrs (string fname);
void code_lattices ();
void code_lexicon ();

/* coding of values */
void reset_temporaries ();
int generate_temp_value (value val, int indent);

/* coding of non call members */
void may_generate_lookahead_check (rule srule, string s);
void generate_terminal (string s, int *nr_pushes);
void generate_semiterminal (member m, int *sonnr, int *nrpushes, int code_build);
void generate_cut (int *nrpushes);

void generate_affix_output (pos_list dpy, int *nrpushes);
void generate_affix_decls (alt a);
void generate_affix_undecls (alt a);
void generate_display (pos_list dpy, int *nr_pushes);

void generate_alt_header (rule srule, int altnr, alt a, int code_build, int cut);
void generate_alt_trailer (rule srule, int altnr, alt a, int nrpushes, int code_build, int cut);
void generate_placeholder_alt_header (rule srule, int untyped, char look);
void generate_placeholder_alt_trailer (rule srule, int untyped, int nrpushes);
void generate_buildplaceholdernode (rule srule, int untyped, int *nrpushes);

void generate_start_rule (string_list predefs);
void generate_module_interface (string fname, string_list predefs);
void generate_main (string fname);

#endif /* IncCommon */
