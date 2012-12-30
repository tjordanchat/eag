#ifndef _ImmedLabelP_h
#define _ImmedLabelP_h

/****************************************************************
 * ImmedLabel widgets
 * CVS ID: "$Id: ImmedLabelP.h,v 1.3 2004/12/25 22:28:11 marcs Exp $"
 ****************************************************************/

#include <X11/CoreP.h>
#include <X11/Xaw/SimpleP.h>
#include <X11/Xmu/Converters.h>
#include "PrimitiveP.h"
#include "ImmedLabel.h"

typedef struct {
    int dummy;
} ImmedLabelClassPart;

typedef struct _ImmedLabelClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    PrimitiveClassPart	primitive_class;
    ImmedLabelClassPart immedlabel_class;
} ImmedLabelClassRec;

extern ImmedLabelClassRec immedLabelClassRec;

typedef struct {
    /* resources */
    String 	label;
    XtJustify	justify;
    Dimension	internalheight;
    Dimension	internalwidth;

    /* private state */
    Position	label_x;
    Position	label_y;
} ImmedLabelPart;

typedef struct _ImmedLabelRec {
    CorePart		core;
    SimplePart 		simple;
    PrimitivePart	primitive;
    ImmedLabelPart	immedlabel;
} ImmedLabelRec;

#endif /* _ImmedLabelP_h */
