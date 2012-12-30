/*
   File: ebs_primio.h
   Define some primitive IO functions
   Also defines the internal End Of File Character

   CVS ID: "$Id: ebs_primio.h,v 1.3 2004/12/25 15:17:33 marcs Exp $"
*/
#ifndef IncEbsPrimIO
#define IncEbsPrimIO

/* global includes */
#include <stdio.h>

/* local includes */
#include "ebs_bst.h"

#define EOFCHAR '\204'
void output_char (FILE *file, char c);
void output_unquoted_string (FILE *file, string str);
void output_string (FILE *file, string str);

#endif /* IncEbsPrimIO */
