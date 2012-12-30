/*
   File: eag_init.h
   Initializes the transducer or recognizer routines

   CVS ID: "$Id: eag_init.h,v 1.3 2004/12/25 22:19:08 marcs Exp $"
*/
#ifndef IncEagInit
#define IncEagInit

/* Just one export */
int init_transducer (int argc, char **argv, int edit, int *usize, char **fname);

#endif /* IncEagInit */
