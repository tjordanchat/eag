#ifndef _ImmedLabel_h
#define _ImmedLabel_h

/****************************************************************
 * ImmedLabel widgets
 * CVS ID: "$Id: ImmedLabel.h,v 1.3 2004/12/25 22:28:11 marcs Exp $"
 ****************************************************************/

#include <X11/Xaw/Simple.h>
#include "Primitive.h"

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		XtDefaultBackground
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 cursor		     Cursor		Cursor		None
 cursorName	     Cursor		String		NULL
 destroyCallback     Callback		Pointer		NULL
 font		     Font		XFontStruct*	XtDefaultFont
 foreground	     Foreground         Pixel           XtDefaultForeground
 height		     Height		Dimension	0
 insensitiveBorder   Insensitive	Pixmap		Gray
 internalHeight      Height		Dimension	2
 internalWidth	     Width		Dimension	2
 justify	     Justify		XtJustify	XtJustifyCenter
 label		     Label		String		NULL
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 pointerColor	     Foreground		Pixel		XtDefaultForeground
 pointerColorBackground Background	Pixel		XtDefaultBackground
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	0
 x		     Position		Position	0
 y		     Position		Position	0

*/

typedef struct _ImmedLabelClassRec	*ImmedLabelWidgetClass;
typedef struct _ImmedLabelRec		*ImmedLabelWidget;

extern WidgetClass immedLabelWidgetClass;

#endif /* _ImmedLabel_h */
