#ifndef LL_H
#define LL_H

typedef struct linked_list linked_list;

typedef enum {

  ll_Ok ,
  ll_NoMem ,
  ll_Found ,
  ll_NotFound ,
  ll_PrevInserted ,
  ll_ErrParm

} ll_ret;

ll_ret ll_create  (linked_list ** l);
ll_ret ll_destroy (linked_list * l);
ll_ret ll_insert  (linked_list * l, char * key);
ll_ret ll_remove  (linked_list * l, char * key);
ll_ret ll_search  (linked_list * l, char * key);

#endif