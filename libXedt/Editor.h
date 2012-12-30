#ifndef _Editor_h
#define _Editor_h

/****************************************************************
 * Editor widgets
 * CVS ID: "$Id: Editor.h,v 1.3 2004/12/25 22:28:10 marcs Exp $"
 ****************************************************************/

#include <X11/Xaw/Simple.h>
#include "Primitive.h"

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		XtDefaultBackground
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 bufferSize	     BufferSize		Cardinal	0
 cursor		     Cursor		Cursor		None
 cursorName	     Cursor		String		NULL
 destroyCallback     Callback		Pointer		NULL
 editable	     Editable		Boolean		True
 editChangeProc	     Callback		Pointer		NULL
 escapeProc	     Callback		Pointer		NULL
 focusBeginX	     Focus		Cardinal	0
 focusBeginY	     Focus		Cardinal	0
 focusEndX	     Focus		Cardinal	0
 focusEndY	     Focus		Cardinal	0
 font		     Font		XFontStruct*	XtDefaultFont
 foreground	     Foreground         Pixel           XtDefaultForeground
 height		     Height		Dimension	0
 insensitiveBorder   Insensitive	Pixmap		Gray
 internalHeight	     Height		Dimension	2
 internalWidth	     Width		Dimension	2
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 pointerColor	     Foreground		Pixel		XtDefaultForeground
 pointerColorBackground Background	Pixel		XtDefaultBackground
 pressedProc	     Callback		Pointer		NULL
 screenWidth	     ScreenWidth	Cardinal	80
 screenHeight	     ScreenHeight	Cardinal	24
 sensitive	     Sensitive		Boolean		True
 sink		     Buffer		String		NULL
 source		     Buffer		String		NULL
 width		     Width		Dimension	0
 wrapAround	     WrapAround		Boolean		False
 x		     Position		Position	0
 y		     Position		Position	0

*/

#ifndef XtNeditable
#define XtNeditable	"editable"
#endif

#ifndef XtNeditChangeProc
#define XtNeditChangeProc "editChangeProc"
#endif

#ifndef XtNescapeProc
#define XtNescapeProc	"escapeProc"
#endif

#ifndef XtNpressedProc
#define XtNpressedProc	"pressedProc"
#endif

#ifndef XtNbufferSize
#define XtNbufferSize	"bufferSize"
#endif

#ifndef XtNsource
#define XtNsource	"source"
#endif

#ifndef XtNsink
#define XtNsink		"sink"
#endif

#ifndef XtNfocusBeginX
#define XtNfocusBeginX	"focusBeginX"
#endif

#ifndef XtNfocusBeginY
#define XtNfocusBeginY	"focusBeginY"
#endif

#ifndef XtNfocusEndX
#define XtNfocusEndX	"focusEndX"
#endif

#ifndef XtNfocusEndY
#define XtNfocusEndY	"focusEndY"
#endif

#ifndef XtNscreenWidth
#define XtNscreenWidth	"screenWidth"
#endif

#ifndef XtNscreenHeight
#define XtNscreenHeight "screenHeight"
#endif

#ifndef XtNwrapAround
#define XtNwrapAround	"wrapAround"
#endif

#ifndef XtCBufferSize
#define XtCBufferSize	"BufferSize"
#endif

#ifndef XtCBuffer
#define XtCBuffer	"Buffer"
#endif

#ifndef XtCEditable
#define XtCEditable	"Editable"
#endif

#ifndef XtCFocus
#define XtCFocus	"Focus"
#endif

#ifndef XtCScreenWidth
#define XtCScreenWidth	"ScreenWidth"
#endif

#ifndef XtCScreenHeight
#define XtCScreenHeight "ScreenHeight"
#endif

#ifndef XtCWrapAround
#define XtCWrapAround	"WrapAround"
#endif

typedef struct _EditorClassRec	*EditorWidgetClass;
typedef struct _EditorRec	*EditorWidget;

extern WidgetClass editorWidgetClass;

/* call data for ButtonPress */
typedef struct pressed_data_rec
	{ int x,y;
	  int button;
	} *PressedData;

/* Convenience routines */
void XmsForcedEscapeEditor (Widget w);

#endif /* _Editor_h */
