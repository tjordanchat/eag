/*
   File: XtArgs.h
   Defines macros for easy argument list handling

   CVS ID: "$Id: XtArgs.h,v 1.3 2004/12/25 22:28:11 marcs Exp $"
*/
#ifndef IncXtArgs
#define IncXtArgs
#include <X11/Intrinsic.h>
#ifndef MaxArgs
#define MaxArgs 20
#endif
#ifndef MaxCallbacks
#define MaxCallbacks 20
#endif

extern XtCallbackRec WidgetCallbacks[MaxCallbacks];
extern Arg WidgetArgs[MaxArgs];
extern int NrOfCallbacks, FirstCallback;
extern int NrOfArgs;

/*
   Now define some access macros on the widget arguments
*/
#define StartArgs NrOfArgs = NrOfCallbacks = FirstCallback = 0
#define SetArg(r,v) XtSetArg (WidgetArgs [NrOfArgs], r, v); NrOfArgs++

#define SetCallback(func, value) \
	WidgetCallbacks [NrOfCallbacks].callback = (XtCallbackProc) func;\
	WidgetCallbacks [NrOfCallbacks++].closure = (caddr_t) value

#define UseCallback(resource) \
	SetCallback (NULL, NULL); \
	SetArg (resource, &WidgetCallbacks [FirstCallback]);\
	FirstCallback = NrOfCallbacks

#define Callback(proc) \
	SetCallback(proc,NULL); \
	UseCallback(XtNcallback)

#define UseArgs WidgetArgs, NrOfArgs
#define NoArgs NULL,0
#endif /* IncXtArgs */
