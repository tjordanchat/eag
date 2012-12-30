/*
   File: XSupp.c
   Defines the global variables to support building simple X11 applications

   CVS ID: "$Id: XSupp.c,v 1.3 2004/12/25 22:28:11 marcs Exp $"
*/

/* standard includes */
#include <stdio.h>
#include <stdlib.h>

/* local includes */
#include "XSupp.h"

/* Exported global X vars */
Display *MyDisplay;
Window MyRootWindow;
Screen *MyRootScreen;
Visual *MyVisual;
Colormap MyColormap;
int MyDepth;
int MonoChrome;

long White, Black;
GC MyWhiteGC, MyBlackGC;

/*
   Open the display and fill in the necessary variables
*/
void SetupSimpleXApplication ()
	{ if (!(MyDisplay = XOpenDisplay (DefaultDisplayName)))
	     { fprintf (stderr, "could not open X display\n");
	       exit (4);
	     };
	  SetupRSimpleXApplication ();
	};

/*
   Fill in the necessary variables provided MyDisplay is set
*/
void SetupRSimpleXApplication ()
	{ XGCValues Gcv;
	  MyRootWindow = XDefaultRootWindow (MyDisplay);
	  MyRootScreen = XDefaultScreenOfDisplay (MyDisplay);
	  MyVisual = XDefaultVisualOfScreen (MyRootScreen);
	  MyColormap = XDefaultColormapOfScreen (MyRootScreen);
	  MyDepth = XDefaultDepthOfScreen (MyRootScreen);
	  MonoChrome = XCellsOfScreen (MyRootScreen) == 2;

	  White = XWhitePixelOfScreen (MyRootScreen);
	  Black = XBlackPixelOfScreen (MyRootScreen);
	  Gcv.foreground = White;
	  Gcv.background = Black;
	  Gcv.function = GXcopy;
	  MyWhiteGC = XCreateGC (MyDisplay, MyRootWindow,
			GCFunction | GCForeground | GCBackground, &Gcv);

	  Gcv.foreground = Black;
	  Gcv.background = White;
	  Gcv.function = GXcopy;
	  MyBlackGC = XCreateGC (MyDisplay, MyRootWindow,
			GCFunction | GCForeground | GCBackground, &Gcv);
	};
