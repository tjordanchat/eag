/*
   File: tree.c
   Defines the internal representation of an EAG
  
   CVS ID: "$Id: tree.c,v 1.3 2004/12/26 00:48:47 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_memalloc.h>

/* local includes */
#include "tree.h"

rule_tree rule_root;
meta_tree meta_root;
element_tree element_root;
rule layout_rule;
member start_rule;
alt start_alt;

int enter_meta_rule (meta_rule node)
	{ meta_tree *ptr = &meta_root; 
	  while (1)
	     { int cond;
	       if (*ptr == meta_tree_nil)
		  { *ptr = (meta_tree) ckmalloc (sizeof (struct meta_tree_rec));
		    (*ptr) -> node = node;
		    (*ptr) -> left = meta_tree_nil;
		    (*ptr) -> right = meta_tree_nil;
		    return (1);
		  };
	       cond = strcmp (node -> nonterminal,
			      (*ptr) -> node -> nonterminal);
	       if (cond < 0) ptr = &((*ptr) -> left);
	       else if (cond > 0) ptr = &((*ptr) -> right);
	       else return (0);		/* Multiple occurence */
	     }
	};

meta_rule lookup_meta_rule (string nont)
	{ meta_tree ptr = meta_root;
	  while (1)
	     { int cond;
	       if (ptr == meta_tree_nil) return (meta_rule_nil);
	       cond = strcmp (nont, ptr -> node -> nonterminal);
	       if (cond < 0) ptr = ptr -> left;
	       else if (cond > 0) ptr = ptr -> right;
	       else return (ptr -> node);
	     };
	};

int enter_syntax_rule (rule node)
	{ rule_tree *ptr = &rule_root; 
	  while (1)
	     { int cond;
	       if (*ptr == rule_tree_nil)
		  { *ptr = (rule_tree) ckmalloc (sizeof (struct rule_tree_rec));
		    (*ptr) -> node = node;
		    (*ptr) -> left = rule_tree_nil;
		    (*ptr) -> right = rule_tree_nil;
		    return (1);
		  };
	       cond = strcmp (node -> nonterminal,
			      (*ptr) -> node -> nonterminal);
	       if (cond < 0) ptr = &((*ptr) -> left);
	       else if (cond > 0) ptr = &((*ptr) -> right);
	       else return (0);
	     }
	};

rule lookup_syntax_rule (string nont)
	{ rule_tree ptr = rule_root;
	  while (1)
	     { int cond;
	       if (ptr == rule_tree_nil) return (rule_nil);
	       cond = strcmp (nont, ptr -> node -> nonterminal);
	       if (cond < 0) ptr = ptr -> left;
	       else if (cond > 0) ptr = ptr -> right;
	       else return (ptr -> node);
	     };
	};

int enter_element (element_def node)
	{ element_tree *ptr = &element_root; 
	  while (1)
	     { int cond;
	       if (*ptr == element_tree_nil)
		  { *ptr = (element_tree) ckmalloc
					(sizeof (struct element_tree_rec));
		    (*ptr) -> node = node;
		    (*ptr) -> left = element_tree_nil;
		    (*ptr) -> right = element_tree_nil;
		    return (1);
		  };
	       cond = strcmp (node -> eltnm, (*ptr) -> node -> eltnm);
	       if (cond < 0) ptr = &((*ptr) -> left);
	       else if (cond > 0) ptr = &((*ptr) -> right);
	       else return (0);
	     }
	};

element_def lookup_element (string eltnm)
	{ element_tree ptr = element_root;
	  while (1)
	     { int cond;
	       if (ptr == element_tree_nil) return (element_def_nil);
	       cond = strcmp (eltnm, ptr -> node -> eltnm);
	       if (cond < 0) ptr = ptr -> left;
	       else if (cond > 0) ptr = ptr -> right;
	       else return (ptr -> node);
	     };
	};

void init_tree ()
	{ rule_root = rule_tree_nil;
	  meta_root = meta_tree_nil;
	  element_root = element_tree_nil;
	};
