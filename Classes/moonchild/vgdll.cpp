/*
FILE    : vg_dll.c
DATE    : 16-11-1993
AUTHOR  : Reinier v Vliet
CONTENTS: Routines to work conveniently with 
    doublelinkedlists.
*/


/* {START DOCUMENTATION}
   
   vgdll.c - A doublelinkedlist handler.
       Reinier v Vliet

   The double linked list handler is a tool to let you work conveniently
   with double linked lists (dll's). It allows you to initialise any number
   of linked lists and to manipulate them. You can attach any number of
   items to the list as long as memory allows it. The items you want to add
   to the list, should be supplied with a pointer to them and call the
   appropriate routine to add them to the list. Items, referenced by
   void *, can be used to store anything from casual data to pointers.

   This library enables you to work with double linked lists in a way that
   you don't have to worry about nodes, chain pointers etc, but can start
   immediately with your creative implementation.

Typical usage:

   #include <vgdll.hpp>

   vg_dll_list    *mylist;
   char           *myitem
   int            result;


   mylist = vg_dll_create();
   result = vg_dll_add(mylist,myitem,VG_DLL_FIRST);
   etc...


   Last note: None of the functions will mess with your list-currentpointer
        unless the function explicitly declares it will.

   {END DOCUMENTATION} */


/* include files */

#include <stdio.h>
#include <stdlib.h>


#include "vgdll.hpp"

/* functionprototypes for library use only */

static VG_DLL_NODE   *vg_dll_makenode (VG_DLL_ITEM *,int);
static void           vg_dll_addsortitem (VG_DLL_LIST *,VG_DLL_NODE *);


/* special macro if this proggie isn't used on cdi */

#define vg_allocate(x,y) malloc(x)
#define vg_free(x) free(x)

/* 
 * SYNOPSIS       Create and init a dl list.
 *
 * SYNTAX         int vg_dll_init(VG_DLL_LIST *list, int memplane);
 *
 * DESCRIPTION    This function initialises a dl list variable of type
 *                VG_DLL_LIST pointed to by <list>.
 *                If CDI isn't defined the memplane will be ignored and malloc will be used
 *                otherwise memplane will be forwarded to the vg_allocate function.
 *                               
 * RETURNS        TRUE if succesfully initialised, FALSE otherwise.
*/

int vg_dll_init(VG_DLL_LIST *list,int memplane)
{
   if (list==NULL) return FALSE;

   list->is_allocated = FALSE;
   list->memplane=memplane;
   list->vg_dll_lfirst=NULL;
   list->vg_dll_llast=NULL;
   list->vg_dll_lcurrent=NULL;

   return TRUE;
}

/* 
 * SYNOPSIS       Create and init a dl list.
 *
 * SYNTAX         VG_DLL_LIST *vg_dll_create(int memplane);
 *
 * DESCRIPTION    This function allocates and initialises a dl list
 *                definition in memory and returns a pointer to the user.
 *                This pointer is used to identify the list in all the functions.
 *                Always use this function to create a list.
 *                If CDI isn't defined the memplane will be ignored and malloc will be used
 *                otherwise memplane will be forwarded to the vg_allocate function.
 *                               
 * RETURNS        A pointer to a linked list definition structure.
 *                Returns NULL if not succeeded.                  
*/


VG_DLL_LIST *vg_dll_create(int memplane)
{
   VG_DLL_LIST *list=NULL;    /* tijdelijke pointer om te retourneren */
   
   list= (VG_DLL_LIST *) vg_allocate(sizeof(VG_DLL_LIST),memplane);    /* alloc struktuur */

   if (list==NULL) return list;             /* is er geheugen gereserveerd? */

/* alloceren is schijnbaar gelukt en we gaan nu de list initialiseren naar */
/* onze dummy struktuur */

   list->is_allocated = TRUE;
   list->memplane=memplane;
   list->vg_dll_lfirst=NULL;
   list->vg_dll_llast=NULL;
   list->vg_dll_lcurrent=NULL;

/* de pointer naar de list retourneren aan de user */

   return list;
}


/* 
 * SYNOPSIS       Free a dl list.
 *
 * SYNTAX         void vg_dll_free(VG_DLL_LIST *list);
 *
 * DESCRIPTION    This function frees the dl <list> definition structure from
 *                memory. It will free all nodes in the list, but not the associated
 *                items. The function vg_dll_process() with the function free() can
 *                used if the items are allocated structures.
 *
 * RETURNS      Nothing
*/
void vg_dll_free(VG_DLL_LIST *list)
{
   VG_DLL_NODE *node,*node2;   /* dummy naampjes */

/* eerst alle nodes free'en */

   node=list->vg_dll_lfirst;
   while (node!=NULL) 
   {
   node2=node->vg_dll_nnext;
   vg_free((void *)node);
   node=node2;
   }

/* en de list free'en (if allocated!) */
   if (list->is_allocated) {
       vg_free((void *)list);
   }
}


/* 
 * SYNOPSIS       Resets and emptys the list.
 *
 * SYNTAX         void vg_dll_clear(VG_DLL_LIST *list);
 *
 * DESCRIPTION.   This function clears the contents of the entire dl <list>.
 *
 * RETURNS        Nothing
*/
void vg_dll_clear(VG_DLL_LIST *list)
{
   list->vg_dll_lcurrent=list->vg_dll_lfirst;
   while (list->vg_dll_lcurrent!=NULL)
   {
      vg_dll_remcur(list);       /* delete current item */
   }
}


/* 
 * SYNOPSIS       Sort the list.
 *
 * SYNTAX         void vg_dll_qsort(VG_DLL_LIST *list,
 *                                  int (*cmp)(VG_DLL_ITEM *item,
 *                                             VG_DLL_ITEM *item));
 *
 * DESCRIPTION.   This function sorts the entire dll <list> using the function
 *                provided by the user. This user supplied function will
 *                be called from the sort routine, and must return a 0 if the
 *                items are of equal ordening, a minus int if item1 is lower
 *                ordened than item2 and a positive int if item1 is higher
 *                ordened than item2.
 *                Note that the current pointer is undefined after sorting, 
 *                the best advised thing to do is to use the function:
 *                vg_dll_first() to reset the currentpointer to the first
 *                position.
 *
 * RETURNS        Nothing.
*/
void vg_dll_qsort (VG_DLL_LIST *list, int (*compare) (VG_DLL_ITEM *,VG_DLL_ITEM *))
{
  VG_DLL_LIST lowerequallist = {NULL,NULL,NULL};
  VG_DLL_LIST higherlist = {NULL,NULL,NULL};
  VG_DLL_NODE *testitem,*pointitem,*onthouitem;

  /* eerste stap qsort.... is list leeg? zo ja meteen terug */

  if (list->vg_dll_lfirst==NULL) return;

  /* stap 2: bestaat list uit 1 item? zo ja meteen teurg */

  if (list->vg_dll_lfirst->vg_dll_nnext==NULL) return;

  /* stap 3: eerste node afsplitsen naar testitem */

  testitem = list->vg_dll_lfirst;
  pointitem = list->vg_dll_lfirst->vg_dll_nnext; /* point node zetten naar 2e node in list */
  pointitem->vg_dll_nprev = NULL;
  testitem->vg_dll_nnext = NULL;
  testitem->vg_dll_nprev = NULL;

  /* stap 4: de list (pointitem) gaan verdelen in de lists: lowerequal & higher */

  while (pointitem!=NULL){
    onthouitem=pointitem->vg_dll_nnext;
    if (compare(testitem->vg_dll_nitem,pointitem->vg_dll_nitem) < 0){
      vg_dll_addsortitem(&higherlist,pointitem);
    }
    else{
      vg_dll_addsortitem(&lowerequallist,pointitem);
    }
    pointitem=onthouitem;
  }

  /* stap 5: deze routine recursief gaan aanroepen met de lowerequal list */

  vg_dll_qsort(&lowerequallist,compare);

  /* stap 6: deze routine recursief gaan aanroepen met de higher list */

  vg_dll_qsort(&higherlist,compare);

  /* stap 7: de twee lijsten aan elkaar binden samen met de losse item */

  vg_dll_addsortitem(&lowerequallist,testitem); /* eerst losse testitem achter binden */

  /* nu de lists aan elkaar: */

   if ((lowerequallist.vg_dll_lfirst==NULL)&&(higherlist.vg_dll_lfirst!=NULL))
   {
      /* first list is empty but second is not */
      /* copy the pointers of list2 to list1   */

      lowerequallist.vg_dll_lfirst  =higherlist.vg_dll_lfirst;
      lowerequallist.vg_dll_llast   =higherlist.vg_dll_llast;
      lowerequallist.vg_dll_lcurrent=higherlist.vg_dll_lcurrent;
      goto skipsort;
   }
   if ((lowerequallist.vg_dll_lfirst==NULL)&&(higherlist.vg_dll_lfirst==NULL))
   {
      /* both lists are empty so we do nothing */
     goto skipsort;
   }

  if ((lowerequallist.vg_dll_lfirst!=NULL)&&(higherlist.vg_dll_lfirst==NULL)){
    /* list 2 is empty so we do nothing */

    goto skipsort;
  }
  /* both lists contain elements so we concatenate them */

  lowerequallist.vg_dll_llast->vg_dll_nnext=higherlist.vg_dll_lfirst;
  higherlist.vg_dll_lfirst->vg_dll_nprev=lowerequallist.vg_dll_llast;
  lowerequallist.vg_dll_llast=higherlist.vg_dll_llast;

 skipsort:
  /* stap 8: de aanelkaar gebonden gesorteerde lijst terug retourneren */
  /*         m.a.w. de pointer naar de list invullen */

  list->vg_dll_lfirst  =lowerequallist.vg_dll_lfirst;
  list->vg_dll_llast   =lowerequallist.vg_dll_llast;
  list->vg_dll_lcurrent=lowerequallist.vg_dll_lcurrent;
}

static void vg_dll_addsortitem(VG_DLL_LIST *list, VG_DLL_NODE *myitem)
{
  if (list->vg_dll_lfirst==NULL){
    list->vg_dll_lfirst=myitem;
    list->vg_dll_llast=myitem;
    list->vg_dll_lcurrent=myitem;
    myitem->vg_dll_nnext=NULL;
    myitem->vg_dll_nprev=NULL;
    return;
  }

  list->vg_dll_llast->vg_dll_nnext=myitem;
  myitem->vg_dll_nprev=list->vg_dll_llast;
  myitem->vg_dll_nnext=NULL;
  list->vg_dll_llast=myitem;
}


/* 
 * SYNOPSIS       Process all the items in a list.
 *
 * SYNTAX         void vg_dll_process(VG_DLL_LIST *list,
 *                                    void (*process)(VG_DLL_ITEM *item));
 *
 * DESCRIPTION.   This function processes the entire dl <list> using the
 *                <process> provided by the user. This user supplied
 *                <process> will be called from this function.
 *
 * RETURNS        Nothing
*/
void vg_dll_process(VG_DLL_LIST *list, void (*process) (VG_DLL_ITEM *))
{
   VG_DLL_NODE *node;
   node=list->vg_dll_lfirst;
   while (node!=NULL)
   {
      process(node->vg_dll_nitem);
      node=node->vg_dll_nnext;
   }
}


/* 
 * SYNOPSIS       Concatenate two lists.
 *
 * SYNTAX         void vg_dll_concat(VG_DLL_LIST *list,VG_DLL_LIST *list2);
 *
 * DESCRIPTION.   This funtion concatenates (appends) <list2> behind <list1>.
 *                The <list2> descriptor is freed from memory afterwards.
 *
 * RETURNS        Nothing
*/
void vg_dll_concat(VG_DLL_LIST *list, VG_DLL_LIST *list2)
{
   if ((list->vg_dll_lfirst==NULL) && (list2->vg_dll_lfirst!=NULL))
   {
      /* first list is empty but second is not */
      /* copy the pointers of list2 to list1   */
      /* and free list2 */

      list->vg_dll_lfirst  =list2->vg_dll_lfirst;
      list->vg_dll_llast   =list2->vg_dll_llast;
      list->vg_dll_lcurrent=list2->vg_dll_lcurrent;

      /* en de 2e list free'en (if allocated!) */
      if (list2->is_allocated) {
    vg_free((void *)list2);
      }
   }
   if ((list->vg_dll_lfirst==NULL) && (list2->vg_dll_lfirst==NULL))
   {
      /* both lists are empty so we just free list2 */
      /* de 2e list free'en (if allocated!) */
      if (list2->is_allocated) {
    vg_free((void *)list2);
      }
      return;
   }

   if ((list->vg_dll_lfirst!=NULL) && (list2->vg_dll_lfirst==NULL))
   {
      /* list 2 is empty so we just free list2 */
      /* de 2e list free'en (if allocated!) */
      if (list2->is_allocated) {
    vg_free((void *)list2);
      }
      return;
   }
    /* both lists contain elements so we concatenate them */

   list->vg_dll_llast->vg_dll_nnext=list2->vg_dll_lfirst;
   list2->vg_dll_lfirst->vg_dll_nprev=list->vg_dll_llast;
   list->vg_dll_llast=list2->vg_dll_llast;
   /* en de 2e list free'en (if allocated!) */
   if (list2->is_allocated) {
       vg_free((void *)list2);
   }
   return;
}


/* 
 * SYNOPSIS       Copy a list to a second one.
 *
 * SYNTAX         VG_DLL_LIST *vg_dll_copy(VG_DLL_LIST *list);
 *
 * DESCRIPTION.   This funtion copys the given <list> and creates
 *                and returns a second list. The old list remains intact
 *                ofcourse. The second list is an exact copy so that it
 *                contains the same elements. It does however not make
 *                copies of the elements itself but references to the same
 *                as in the original <list>.
 *
 * RETURNS        A pointer to the new list.
 *                NULL If the new list couldn't be made due to low memory
*/
VG_DLL_LIST    *vg_dll_copy(VG_DLL_LIST *list)
{
   VG_DLL_NODE *oldcurrent=list->vg_dll_lcurrent;
   VG_DLL_LIST *newlist;
   VG_DLL_ITEM *dummyitem;
   newlist=vg_dll_create(list->memplane);
   if (newlist==NULL)
   {
      return newlist; /* kon newlist niet alloceren */
   }

   if (vg_dll_first(list)==FALSE)
   {
      /* list is empty so we copy nothing */
      return newlist;
   }

   dummyitem=vg_dll_collect(list);
   while (dummyitem!=NULL)
   {
      if (vg_dll_add(newlist,dummyitem,VG_DLL_LAST)==FALSE)
      {
   vg_dll_free(newlist);
   return NULL;
      }
      if (vg_dll_next(list)==FALSE)
      {
   dummyitem=NULL;      /* einde van list bereikt */
      }
      else
      {
   dummyitem=vg_dll_collect(list);
      }
   }
   list->vg_dll_lcurrent=oldcurrent;    /* current terugzetten */
   return newlist;
}


/* 
 * SYNOPSIS       Split a list into two lists..
 *
 * SYNTAX         VG_DLL_LIST *vg_dll_split(VG_DLL_LIST *list);
 *
 * DESCRIPTION.   This funtion splits a dl <list> into two lists. 
 *                The given <list> will contain the elements from
 *                the first node upto and excluding the current one.
 *                The second list (which is returned by this function)
 *                will contain the current node upto and including
 *                the last one. This function might prove usefull if you
 *                want to process a partiall part of the list for instance.
 *                This way you could split the lists and process one of
 *                the partial lists and concatenate them aftwerwards.
 *                Note that if the <list> no elements, this function
 *                will return a second list (also empty ofcourse).
 *                The currentpointers of both lists will be resetted
 *                to the first node in their list.
 *
 * RETURNS        A pointer to the new list.
 *                NULL If the new list couldn't be made due to low memory
*/
VG_DLL_LIST *vg_dll_split(VG_DLL_LIST *list)
{
  VG_DLL_LIST *newlist;
  if ((newlist=vg_dll_create(list->memplane)) == NULL) return newlist;
  if ((list->vg_dll_lfirst) == NULL) return newlist; /* viel nix te splitten */

  if ((list->vg_dll_lcurrent) == (list->vg_dll_lfirst)){
    /* list1 wordt NULL en list2 wordt list 1 */
    newlist->vg_dll_lfirst   = list->vg_dll_lfirst;
    newlist->vg_dll_llast    = list->vg_dll_llast;
    newlist->vg_dll_lcurrent = list->vg_dll_lcurrent;
    list->vg_dll_lfirst   = NULL;
    list->vg_dll_llast    = NULL;
    list->vg_dll_lcurrent = NULL;
    return newlist;
  }

  /* list identifiers opnieuw initialiseren */

  newlist->vg_dll_lfirst   = list->vg_dll_lcurrent;
  newlist->vg_dll_lcurrent = list->vg_dll_lcurrent;
  newlist->vg_dll_llast    = list->vg_dll_llast;

  list->vg_dll_llast    = list->vg_dll_lcurrent->vg_dll_nprev;
  list->vg_dll_lcurrent = list->vg_dll_lfirst;

  /* nodes van de lists van elkaar afkoppelen */

  list->vg_dll_llast->vg_dll_nnext = NULL;

  newlist->vg_dll_lfirst->vg_dll_nprev = NULL;

  return newlist;
}


/* 
 * SYNOPSIS       Add an item to the dl list.
 *
 * SYNTAX         int vg_dll_add(VG_DLL_LIST *list,VG_DLL_ITEM *item,
 *                               vg_dll_method method);
 *
 * DESCRIPTION    This function adds a node to the dl <list>. In the node will
 *                be a pointer to your <item>. You can specify several
 *                insertion <method>s:
 *    -VG_DLL_BEFORECURRENT will insert the <item> before the current item.
 *    -VG_DLL_BEHINDCURRENT will insert the <item> behind the current item.
 *    _VG_DLL_FIRST         will insert the <item> as the first in the list.
 *    _VG_DLL_LAST          will insert the <item> as the last in the list.
 *                               
 * RETURNS        FALSE if it wasn't able to insert the <item> in the <list>.
 *                TRUE if it succeeded.
*/
int vg_dll_add(VG_DLL_LIST *list, VG_DLL_ITEM *item, VG_DLL_METHOD method)
{
   VG_DLL_NODE *current;         /* current node */
   VG_DLL_NODE *node;            /* nieuwe node */
   VG_DLL_NODE *oldnode;         /* de node die ook verandert moet worden */

   node=vg_dll_makenode(item,list->memplane);
   if (node==0) return FALSE;
   if (list->vg_dll_lfirst==NULL)  /* list nog leeg?? */
   {
      list->vg_dll_lfirst=node;
      list->vg_dll_llast=node;
      list->vg_dll_lcurrent=node;
      return TRUE;
   }
   switch (method)
   {
      case VG_DLL_BEFORECURRENT:

/* wordt het de eerste in de list? */

   if (list->vg_dll_lcurrent==list->vg_dll_lfirst) goto do_first;

/* paar hulpnodes klaarmaken */

   current=list->vg_dll_lcurrent;
   oldnode=current->vg_dll_nprev;

/* node inbinden */

   oldnode->vg_dll_nnext        =node;
   node->vg_dll_nnext    =current;
   node->vg_dll_nprev    =oldnode;
   current->vg_dll_nprev =node;
   return TRUE;
      break;
      case VG_DLL_BEHINDCURRENT:

/* wordt de laatste in de list? */

   if (list->vg_dll_lcurrent==list->vg_dll_llast) goto do_last;

/* paar hulpnodes klaarmaken */

   current=list->vg_dll_lcurrent;
   oldnode=current->vg_dll_nnext;

/* node inbinden */

   oldnode->vg_dll_nprev        =node;
   node->vg_dll_nprev    =current;
   node->vg_dll_nnext    =oldnode;
   current->vg_dll_nnext =node;
   return TRUE;
      break;
      case VG_DLL_FIRST:
do_first:
   oldnode=list->vg_dll_lfirst;
   list->vg_dll_lfirst=node;
   oldnode->vg_dll_nprev =node;
   node->vg_dll_nnext =oldnode;
   return TRUE;
      break;
      case VG_DLL_LAST:
do_last:
   oldnode=list->vg_dll_llast;
   list->vg_dll_llast =node;
   oldnode->vg_dll_nnext     =node;
   node->vg_dll_nprev =oldnode;
   return TRUE;
      break;
   }
   return FALSE;
}


static VG_DLL_NODE *vg_dll_makenode(VG_DLL_ITEM *item,int memplane)
{
   VG_DLL_NODE *node;
   node=(VG_DLL_NODE *) vg_allocate(sizeof(VG_DLL_NODE),memplane);
   if (node==NULL) return node;
   node->vg_dll_nnext=NULL;
   node->vg_dll_nprev=NULL;
   node->vg_dll_nitem=item;
   return node;
}


/* 
 * SYNOPSIS       Search an item in the dl list.
 *
 * SYNTAX         int vg_dll_search(VG_DLL_LIST *list,VG_DLL_ITEM *item,
 *                                   VG_DLL_SMETHOD *smethod);
 *
 * DESCRIPTION.   Points the currentpointer to the node containing your <item>.
 *                if the currentpointer is already pointing to the node with
 *                your specified <item>. the function will return TRUE.
 *                If the <smethod> = VG_DLL_BACKWARDS, the search will go from
 *                the current node in the direction of the first item.
 *                If the <smethod> = VG_DLL_FORWARDS, the search will go from
 *                the current node in the direction of the last item.
 *                Note that if the search didn't find a match, the
 *                currentpointer isn't altered.
 *
 * RETURNS        TRUE if we found a matching item.
 *                FALSE if we found no matching item.
*/
int vg_dll_search(VG_DLL_LIST *list, VG_DLL_ITEM *item, VG_DLL_SMETHOD smethod)
{
   VG_DLL_NODE *node;
   node = list->vg_dll_lcurrent;
   if (node->vg_dll_nitem == item) return TRUE;

   switch (smethod){
   case VG_DLL_FORWARDS:

     while (node != NULL)
       {
   if (node->vg_dll_nitem == item)
     {
       list->vg_dll_lcurrent = node;
       return TRUE;
       break;
     }
   node = node->vg_dll_nnext;
       }
     return FALSE;
     break;
   case VG_DLL_BACKWARDS:
     while (node!=NULL)
       {
   if (node->vg_dll_nitem==item)
     {
       list->vg_dll_lcurrent=node;
       return TRUE;
       break;
     }
   node=node->vg_dll_nprev;
       }
     return FALSE;
     break;
   }
   return FALSE;
}



/* 
 * SYNOPSIS       Remove the current item from the dl list.
 *
 * SYNTAX         void vg_dll_remcur(VG_DLL_LIST *list);
 *
 * DESCRIPTION.   This function removes the current item from the dl <list>.
 *                After the operation, the current pointer will point to the
 *                item following the deleted current item.
 *                If the current points to the last item in the list then,
 *                the currentpointer will point to the new last item (after
 *                deletion) in the list. 
 *                If the current item was the only one  in the list, the current
 *                will point to NULL
 *                               
 * RETURNS        Nothing
*/
void vg_dll_remcur(VG_DLL_LIST *list)
{
VG_DLL_NODE *current;
VG_DLL_NODE *nextnode;
VG_DLL_NODE *prevnode;

   current=list->vg_dll_lcurrent;
   if (current==NULL) return;       /* list is al leeg */

/* mogelijkheid 2: de current item is de enige in de lijst */

   if (current->vg_dll_nnext==NULL&&current->vg_dll_nprev==NULL)
   {
      list->vg_dll_lfirst  =NULL;
      list->vg_dll_llast   =NULL;
      list->vg_dll_lcurrent=NULL;
      vg_free(current);
      return;
   }

/* mogelijkheid 3: de current item is de eerste in de lijst */

   if (current->vg_dll_nprev==NULL)
   {
      nextnode=current->vg_dll_nnext;
      nextnode->vg_dll_nprev=NULL;
      list->vg_dll_lfirst=nextnode;
      list->vg_dll_lcurrent=nextnode;
      vg_free(current);
      return;
   }

/* mogelijkheid 4: de current item is de laatste in de lijst */

   if (current->vg_dll_nnext==NULL)
   {
      prevnode=current->vg_dll_nprev;
      prevnode->vg_dll_nnext=NULL;
      list->vg_dll_llast=prevnode;
      list->vg_dll_lcurrent=prevnode;
      vg_free(current);
      return;
   }

/* mogelijkheid 5: de current item staat middenin de lijst */

      prevnode=current->vg_dll_nprev;
      nextnode=current->vg_dll_nnext;
      prevnode->vg_dll_nnext=nextnode;
      nextnode->vg_dll_nprev=prevnode;
      list->vg_dll_lcurrent=nextnode;
      vg_free(current);
      return;
}


/* 
 * SYNOPSIS       Count all items in the dl list.
 *
 * SYNTAX         int vg_dll_count(VG_DLL_LIST *list);
 *
 * DESCRIPTION.   This function will count the number of items the dl <list>
 *                holds.
 *
 * RETURNS        number of items in the dl <list>.
*/
int vg_dll_count (VG_DLL_LIST *list)
{
   int count=0;              /* teller */
   VG_DLL_NODE *node;      /* hulp node om te tellen */

   node=list->vg_dll_lfirst;
   while (node!=NULL)
   {
      count++;
      node=node->vg_dll_nnext;
   }
   return count;
}


/* 
 * SYNOPSIS       Advance the currentpointer in the list.
 *
 * SYNTAX         int vg_dll_next(bg_dll_list *list);
 *
 * DESCRIPTION.   This function advances the current pointer in the
 *                dl <list> so that it points to the next item.
 *                               
 * RETURNS        TRUE if the operation succeeded.
 *                FALSE if the current item was the last in the dl <list>.
 *                FALSE if the <list> is empty
*/
int vg_dll_next(VG_DLL_LIST *list)
{
   if (list->vg_dll_lfirst == NULL) return FALSE;
   if (list->vg_dll_lcurrent == list->vg_dll_llast) return FALSE;
   list->vg_dll_lcurrent = list->vg_dll_lcurrent->vg_dll_nnext;
   return TRUE;
}


/* 
 * SYNOPSIS       Retreat the currentpointer in the list.
 *
 * SYNTAX         int vg_dll_prev(bg_dll_list *list);
 *
 * DESCRIPTION.   This function retreats the current pointer in the
 *                dl <list> so that it points to the previous item.
 *                               
 * RETURNS        TRUE if the operation succeeded.
 *                FALSE if the current item was the first in the dl <list>.
 *                FALSE if the <list> is empty.
*/
int vg_dll_prev(VG_DLL_LIST *list)
{
   if (list->vg_dll_lfirst==NULL) return FALSE;
   if (list->vg_dll_lcurrent==list->vg_dll_lfirst) return FALSE;
   list->vg_dll_lcurrent=list->vg_dll_lcurrent->vg_dll_nprev;
   return TRUE;
}


/* 
 * SYNOPSIS       Reset the currentpointer in the list.
 *
 * SYNTAX         int vg_dll_first(bg_dll_list *list);
 *
 * DESCRIPTION.   This function resets the current pointer in the
 *                dl <list> so that it points to the first item.
 *                               
 * RETURNS        TRUE if the operation succeeded.
 *                FALSE if the <list> is empty
*/
int vg_dll_first(VG_DLL_LIST *list)
{
   if (list->vg_dll_lfirst==NULL) return FALSE;
   list->vg_dll_lcurrent=list->vg_dll_lfirst;
   return TRUE;
}


/* 
 * SYNOPSIS       Point the currentpointer to last item in the list.
 *
 * SYNTAX         int vg_dll_last(bg_dll_list *list);
 *
 * DESCRIPTION.   This function sets the current pointer in the
 *                dl <list> so that it points to the last item.
 *                               
 * RETURNS        TRUE if the operation succeeded.
 *                FALSE if the <list> is empty
*/
int vg_dll_last(VG_DLL_LIST *list)
{
   if (list->vg_dll_lfirst==NULL) return FALSE;
   list->vg_dll_lcurrent=list->vg_dll_llast;
   return TRUE;
}


/* 
 * SYNOPSIS       Returns pointer to the current item.
 *
 * SYNTAX         VG_DLL_ITEM *vg_dll_collect(VG_DLL_LIST *list);
 *
 * DESCRIPTION.   This function returns the pointer to the current <item> in
 *                the specified dl <list>.
 *                               
 * RETURNS        Pointer to <item> if it succeeded.
 *                NULL if the dl <list> is empty.
*/
VG_DLL_ITEM *vg_dll_collect(VG_DLL_LIST *list)
{
   if(list->vg_dll_lfirst == NULL) return NULL;
   return   list->vg_dll_lcurrent->vg_dll_nitem;
}
