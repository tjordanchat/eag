/*
   File: ebs_value.h
   Define operations on values

   CVS ID: "$Id: ebs_value.h,v 1.7 2008/05/28 14:55:34 marcs Exp $"
*/
#ifndef IncEbsValue
#define IncEbsValue

/* local includes */
#include "ebs_bst.h"
#include "ebs_cst.h"

/* Introduce record type definitions */
typedef struct value_rec *value;		/* values of all kind */

/* Introduce list definitions */
typedef struct value_list_rec *value_list;	/* value lists */

/* Introduce nil pointers for records */
#define value_nil ((value) NULL)

/* Introduce nil pointers for lists */
#define value_list_nil ((value_list) NULL)

/* Introduce structure of affix values */
#define undefined_value 0
#define string_value 1
#define integer_value 2
#define real_value 3
#define small_lattice_value 4
#define large_lattice_value 5
#define tuple_value 6
struct value_rec
{ int admin_nr;				/* meta rule cons nr */
  int ref_count;			/* reference count */
  void *dptr;				/* debug ptr */
  int tag;
  union
     { string str;
       int inum;
       real rnum;
       value_list tuple;
       int slat;
       int_list elat;
       value free;
     } u;
};

/* Introduce structure of value lists */
struct value_list_rec
{ int size;
  int room;
  value *array;
};

/* value manipulation */
value new_undefined_value ();
value new_string_value (string s);
value new_integer_value (int inum);
value new_real_value (double rnum);
value new_tuple_value (value_list vl);
value new_tuple_value_from_array (int size, value *array);
value new_small_lattice_value (int slat, string *lnames);
value new_large_lattice_value (int size, int *elat, string *lnames);
value new_singleton_value (int eltnr, int size, string *lnames);
value rdup_value (value val);
void rfre_value (value old);
int less_value (value v1, value v2);
int equal_value (value v1, value v2);
void save_value (FILE *f, value v);
int load_value (FILE *f, value *v, string *lnames);

/* value list manipulation */
value_list new_value_list (int room);
void room_value_list (value_list vl, int room);
void free_value_list (value_list vl);
void app_value_list (value_list sl, value s);
value_list rdup_value_list (value_list sl);
void rfre_value_list (value_list old);
int less_value_list (value_list vl1, value_list vl2);
int equal_value_list (value_list vl1, value_list vl2);
void save_value_list (FILE *f, value_list vl);
int load_value_list (FILE *f, value_list *vl, string *lnames);

/* value calculation */
value concatenate_values (value_list vl);
value join_lattice_values (value_list vl);
int meet_lattice_values (value v1, value v2, value *vret);

/* value tracing and outputting */
void dump_value (value val);
void output_value (FILE *out, value val);

/* initialization */
void init_value ();

#endif /* IncEbsValue */
