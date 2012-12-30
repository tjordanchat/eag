/*
   File: XtSupp.h
   Defines the global variables to support building applications using the X toolkit

   CVS ID: "$Id: XtSupp.h,v 1.3 2004/12/25 22:28:11 marcs Exp $"
*/
#ifndef IncXtSupp
#define IncXtSupp
#include <X11/Intrinsic.h>

extern Widget MyRootWidget;
extern XtAppContext MyContext;
extern long BackgroundColor;
extern long ForegroundColor;
extern GC MyDefaultGC;

void SetupToolkitApplication (char *name, char *class,
		XrmOptionDescRec options [], int nr_options,
		caddr_t myresources, XtResource resources [], int nr_resources,
		int *argc, char **argv);

#endif /* IncXtSupp */
