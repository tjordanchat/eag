/*
   File: ebs_error.h
   Defines routines for diagnostic messages

   CVS ID: "$Id: ebs_error.h,v 1.3 2004/12/25 15:17:32 marcs Exp $"
*/
#ifndef IncEbsError
#define IncEbsError

/* Exported vars for debugging and logging options */
extern int debug;
extern int verbose;
extern int full_verbose;

/* Exported error logging code */
void init_error ();
void panic (char *format, ...);
void error (char *format, ...);
void warning (char *format, ...);
void hint (char *format, ...);
void wlog (char *format, ...);
void eprint_log (char *format, ...);

/* Exported exception code */
void internal_error (char *routine);
void bad_tag (int tag, char *routine);

#endif /* IncEbsError */
