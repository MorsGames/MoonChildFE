/*
FILE    : vg_dll.h
DATE    : 16-11-1993
AUTHOR  : Reinier v Vliet
CONTENTS: Prototypes and constants for the doublelist handler
COMMENTS: Don't use empty items in the list (pointers to NULL)


Typical use:

//   #include <vg_dll.h>

   VG_DLL_LIST    *mylist;
   char           *myitem
   int            result;


   mylist = vg_dll_create(VG_PLANE_A);
   result = vg_dll_add(mylist,myitem,VG_DLL_FIRST);
   etc...

*/

#ifndef _VG_DLL_H
#define _VG_DLL_H

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

typedef void VG_DLL_ITEM;
typedef struct defVG_DLL_NODE VG_DLL_NODE;

struct defVG_DLL_NODE {
      VG_DLL_NODE *vg_dll_nnext;
      VG_DLL_NODE *vg_dll_nprev;
      VG_DLL_ITEM *vg_dll_nitem;
};

typedef struct {
      VG_DLL_NODE *vg_dll_lfirst;
      VG_DLL_NODE *vg_dll_llast;
      VG_DLL_NODE *vg_dll_lcurrent;
      short        memplane;
      short        is_allocated; /* TRUE if struct is allocated and must be freed once */
} VG_DLL_LIST;

/* options for method */
typedef enum {
    VG_DLL_BEFORECURRENT = 0,
    VG_DLL_BEHINDCURRENT,
    VG_DLL_FIRST,
    VG_DLL_LAST
} VG_DLL_METHOD;

/* options for smethod (search method) */
typedef enum {
    VG_DLL_BACKWARDS = 0,
    VG_DLL_FORWARDS
} VG_DLL_SMETHOD;

#define vg_dll_set_memplane(list, plane) (list)->memplane = (plane)
#define vg_dll_get_memplane(list) (list)->memplane

#ifdef __cplusplus
extern "C" {
#endif

 
int            vg_dll_init(VG_DLL_LIST *, int);
VG_DLL_LIST   *vg_dll_create(int);
void           vg_dll_free(VG_DLL_LIST *);
void           vg_dll_clear(VG_DLL_LIST *);
void           vg_dll_qsort(VG_DLL_LIST *, int (*cmp)(VG_DLL_ITEM *,VG_DLL_ITEM *));
void           vg_dll_process(VG_DLL_LIST *, void (*process)(VG_DLL_ITEM *));
void           vg_dll_concat(VG_DLL_LIST *,VG_DLL_LIST *);
VG_DLL_LIST   *vg_dll_copy(VG_DLL_LIST *);
VG_DLL_LIST   *vg_dll_split(VG_DLL_LIST *);

int            vg_dll_add(VG_DLL_LIST *,VG_DLL_ITEM *,VG_DLL_METHOD);
int            vg_dll_search(VG_DLL_LIST *,VG_DLL_ITEM *,VG_DLL_SMETHOD);
void           vg_dll_remcur(VG_DLL_LIST *);
int            vg_dll_count(VG_DLL_LIST *);
int            vg_dll_next(VG_DLL_LIST *);
int            vg_dll_prev(VG_DLL_LIST *);
int            vg_dll_first(VG_DLL_LIST *);
int            vg_dll_last(VG_DLL_LIST *);
VG_DLL_ITEM   *vg_dll_collect(VG_DLL_LIST *);

#ifdef __cplusplus
}
#endif

#endif /* _VG_DLL_H */

