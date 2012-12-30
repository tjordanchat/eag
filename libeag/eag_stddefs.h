/*
   File: eag_stddefs.h
   Implements the standard eag library

   CVS ID: "$Id: eag_stddefs.h,v 1.5 2008/06/21 14:13:17 marcs Exp $"
*/
#ifndef IncEagStddefs
#define IncEagStddefs

/* libebs includes */
#include "ebs_bst.h"

/* local includes */
#include "eag_ds.h"

/* Exported metarule code */
affix_node make_nil_affix ();
affix_node make_string_affix ();
affix_node make_int_affix ();
affix_node make_real_affix ();
affix_node make_tuple_affix ();
affix_node make_nonlat_affix ();

void rec_nil_value ();
void rec_string_value ();
void rec_int_value ();
void rec_real_value ();
void rec_nonlat_value ();
void rec_tuple_value ();

void recbup_string_value ();
void recbup_int_value ();
void recbup_real_value ();

/* Exported standard predicates */
void pred_equal ();
void pred_notequal ();
void pred_inttostring ();
void pred_realtostring ();
void pred_minus ();
void pred_mul ();
void pred_div ();
void pred_power ();
void pred_uppercase ();
void pred_notinreservedwordlist ();
void pred_stringtoint ();
void pred_stringtoreal ();
void pred_dumpaffix ();
void semipred_endofsentence ();
void semipred_column ();
void semipred_row ();

/* Exported module linkage */
string eag_stddefs_name_from_nodenr ();
void init_eag_stddefs ();

#endif /* IncEagStddefs */
