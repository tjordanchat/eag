#ifndef _EditorP_h
#define _EditorP_h

/****************************************************************
 * Editor widgets
 * CVS ID: "$Id: EditorP.h,v 1.3 2004/12/25 22:28:10 marcs Exp $"
 ****************************************************************/

#include <X11/CoreP.h>
#include <X11/Xaw/SimpleP.h>
#include "PrimitiveP.h"
#include "Editor.h"

typedef struct {
    int dummy;
} EditorClassPart;

typedef struct _EditorClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    PrimitiveClassPart	primitive_class;
    EditorClassPart	editor_class;
} EditorClassRec;

extern EditorClassRec editorClassRec;

typedef struct {
    /* resources */
    Cardinal	buffersize;
    Cardinal	screenwidth;
    Cardinal	screenheight;

    String	sink;
    String	source;

    Cardinal	focusbeginx;
    Cardinal	focusbeginy;
    Cardinal	focusendx;
    Cardinal	focusendy;

    Dimension	internalheight;
    Dimension	internalwidth;

    Boolean	editable;
    Boolean	wraparound;

    XtCallbackList	editChangeProc;
    XtCallbackList	escapeProc;
    XtCallbackList	pressedProc;

    /* private state */
    String buffer;
    Cardinal textsize;
    Cardinal curx;
    Cardinal cury;
    String curlin;

} EditorPart;

typedef struct _EditorRec {
    CorePart		core;
    SimplePart 		simple;
    PrimitivePart	primitive;
    EditorPart		editor;
} EditorRec;

#endif /* _EditorP_h */
