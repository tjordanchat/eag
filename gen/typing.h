/*
   File: typing.h
   Type checks all rules and meta rules
  
   CVS ID: "$Id: typing.h,v 1.3 2004/12/26 00:48:47 marcs Exp $"
*/
#ifndef IncTyping
#define IncTyping

/* libebs includes */
#include <ebs_bst.h>

#define integer_type 1
#define real_type 2
#define numeric_type 3
#define string_type 4
#define concat_type 5
#define tuple_type 8
#define non_lattice_type 15
#define lattice_type 16
#define any_type 31
#define error_type 0

string string_from_type (int t);
void infer_typing ();
void dump_grammar_typing ();

#endif /* IncTyping */
