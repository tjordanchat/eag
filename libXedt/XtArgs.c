/*
   File: XtArgs.c
   Defines macros for easy argument list handling

   CVS ID: "$Id: XtArgs.c,v 1.3 2004/12/25 22:28:11 marcs Exp $"
*/

/* X11 includes */
#include <X11/Intrinsic.h>

/* local includes */
#include <XtArgs.h>

XtCallbackRec WidgetCallbacks[MaxCallbacks];
Arg WidgetArgs[MaxArgs];
int NrOfCallbacks, FirstCallback;
int NrOfArgs;
