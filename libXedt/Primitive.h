#ifndef _Primitive_h
#define _Primitive_h

/****************************************************************
 * Primitive widgets
 * CVS ID: "$Id: Primitive.h,v 1.3 2004/12/25 22:28:11 marcs Exp $"
 ****************************************************************/

#include <X11/Xaw/Simple.h>

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		XtDefaultBackground
 backgroundPixmap    Pixmap		Pixmap		NULL
 border		     BorderColor	Pixel		XtDefaultForeground
 borderPixmap	     Pixmap		Pixmap		NULL
 borderWidth	     BorderWidth	Dimension	1
 cursor		     Cursor		Cursor		None
 cursorName	     Cursor		String		NULL
 destroyCallback     Callback		Pointer		NULL
 font		     Font		XFontStruct*	XtDefaultFont
 foreground	     Foreground         Pixel           XtDefaultForeground
 height		     Height		Dimension	0
 insensitiveBorder   Insensitive	Pixmap		Gray
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 pointerColor	     Foreground		Pixel		XtDefaultForeground
 pointerColorBackground Background	Pixel		XtDefaultBackground
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	0
 x		     Position		Position	0
 y		     Position		Position	0

*/

typedef struct _PrimitiveClassRec	*PrimitiveWidgetClass;
typedef struct _PrimitiveRec		*PrimitiveWidget;

extern WidgetClass primitiveWidgetClass;

#endif /* _Primitive_h */
