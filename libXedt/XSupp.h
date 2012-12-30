/*
   File: XSupp.h
   Defines the global variables to support building simple X11 applications

   CVS ID: "$Id: XSupp.h,v 1.3 2004/12/25 22:28:11 marcs Exp $"
*/
#ifndef IncXSupp
#define IncXSupp

#include <X11/Xlib.h>
#define DefaultDisplayName (char *)0

extern Display *MyDisplay;
extern Window MyRootWindow;
extern Screen *MyRootScreen;
extern Visual *MyVisual;
extern Colormap MyColormap;
extern int MyDepth;
extern int MonoChrome;

extern long White, Black;
extern GC MyBlackGC;
extern GC MyWhiteGC;

void SetupSimpleXApplication ();
void SetupRSimpleXApplication ();

#endif /* IncXSupp */
