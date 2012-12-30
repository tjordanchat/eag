/*
   File: eag_trace.h
   Provides some debugging support

   CVS ID: "$Id: eag_trace.h,v 1.3 2004/12/25 22:19:09 marcs Exp $"
*/
#ifndef IncEagTrace
#define IncEagTrace

/* local includes */
#include "eag_ds.h"

/* Exported tracing flag */
extern int tracing;

/* Exported tracing code */
void init_trace ();
void trace_enter (char *s);
void trace_placeholder_alternative (char *s, int untyped);
void trace_alternative (char *s, int i);
void trace_leave (char *s);

void trace_affix (affix_node a);
void trace_posside (pos_node p, int i);
void trace_pos (pos_node p);
void trace_restrict_pos (pos_node p, value mval);

#endif /* IncEagTrace */
