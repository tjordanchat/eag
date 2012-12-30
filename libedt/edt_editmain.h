/*
   File: edt_editmain.h
   Registers the widgets of the editor

   CVS ID: "$Id: edt_editmain.h,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/
#ifndef IncEdtEditmain
#define IncEdtEditmain

/* Exported main code */
void register_main_widgets ();
void register_editor_widget (int ubuffer_size);
void register_remaining_widgets ();
void start_application_loop ();

#endif /* IncEdtEditmain */
