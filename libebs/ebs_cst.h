/*
   File: ebs_cst.h
   Define composite standard types and operations

   CVS ID: "$Id: ebs_cst.h,v 1.4 2008/05/28 12:47:03 marcs Exp $"
*/
#ifndef IncEbsCst
#define IncEbsCst

/* local includes */
#include "ebs_bst.h"

/* Introduce list definitions */
typedef struct int_list_rec *int_list;		/* int lists */
typedef struct string_list_rec *string_list;	/* string lists */

/* Introduce nil pointers for lists */
#define int_list_nil ((int_list) NULL)
#define string_list_nil ((string_list) NULL)

/* Introduce structure of int_list */
struct int_list_rec
{ int size;
  int room;
  int *array;
};

/* Introduce structure of string_list */
struct string_list_rec
{ int size;
  int room;
  string *array;
};

/* int list manipulation */
void room_int_list (int_list il, int room);
int_list new_int_list ();
void app_int_list (int_list il, int i);
int_list rdup_int_list (int_list il);
void rfre_int_list (int_list old);
int less_int_list (int_list il1, int_list il2);
int equal_int_list (int_list il1, int_list il2);
void save_int_list (FILE *f, int_list il);
int load_int_list (FILE *f, int_list *il);

/* string list manipulation */
void room_string_list (string_list sl, int room);
string_list new_string_list ();
void app_string_list (string_list sl, string s);
string_list rdup_string_list (string_list sl);
void rfre_string_list (string_list sl);
int less_string_list (string_list sl1, string_list sl2);
int equal_string_list (string_list sl1, string_list sl2);
void save_string_list (FILE *f, string_list sl);
int load_string_list (FILE *f, string_list *sl);

/* specific extra operations */
string concatenate_strings (string_list sl);

#endif /* IncEbsCst */
