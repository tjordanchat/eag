/*
   File: recursion.h
   Determines the leftcorner relation and
   checks left recursion for topdown parsers
  
   CVS ID: "$Id: recursion.h,v 1.3 2004/12/26 00:48:47 marcs Exp $"
*/
#ifndef IncRecursion
#define IncRecursion

/*
   leftc_relation [j * nr_of_hyper_rules + i] = 1 <=>
   j is called by i in one or more steps as a (possibly) leftmost member <=>
   rule j <LC+ rule i

   The leftc_relation is only non zero between non predicate hyperrules.
*/
extern char *leftc_relation;
void check_recursion ();

#endif /* IncRecursion */
