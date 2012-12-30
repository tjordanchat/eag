/*
   File: edt_templates.h
   Administers templates and template substitution

   CVS ID: "$Id: edt_templates.h,v 1.3 2004/12/25 22:40:52 marcs Exp $"
*/
#ifndef IncEdtTemplates
#define IncEdtTemplates

extern char *template_buffer;
void update_template_buffer (int nodenr);
/* export void enter_template_in_list_struct (int nodenr, char *text, int nrsons, int *sonnrs); */
void enter_template_in_list_text (int nodenr, char *text);
void replace_focus_by_template (int nodenr, int nr);
void init_templates ();
#endif /* IncEdtTemplates */
