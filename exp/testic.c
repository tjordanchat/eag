/*
   File: testic.c
*/

/* global includes */
#include <stdio.h>

/* libebs includes */
#include <export.h>
#include <bst.h>
#include <cst.h>
#include <value.h>

/* libeag includes */
#include <ds.h>
#include <c_interface.h>

public int main ()
	{ int nrparses, ix;
	  value *in_vals;
	  value **out_vals;
	  init_error ();
	  init_C_interface ();
	  in_vals = (value *) ckcalloc (2, sizeof (value));
	  in_vals[0] = new_string_value ("Dit is een test: ");
	  nrparses = parse_this_buffer ("aargh", in_vals, &out_vals);
	  for (ix = 0; ix < nrparses; ix++)
	     dump_value (out_vals[ix][1]);
	  return (0);
	};
