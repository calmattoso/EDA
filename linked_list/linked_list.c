/* singly-linked list, with unique elements */
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

typedef struct ll_node {
  char * key;
  struct ll_node * next;
} ll_node;


struct linked_list {
  ll_node * head;
};


ll_ret ll_create(linked_list ** l){
  if(l == NULL)
    return ll_ErrParm;

  *l = (linked_list *) malloc(sizeof(linked_list));
  if(*l == NULL)
    return ll_NoMem;

  (*l)->head = NULL;
  
  return ll_Ok;
}

ll_ret ll_destroy (linked_list * l){
  if(l == NULL)
    return ll_ErrParm;

  while(l->head != NULL)
    ll_remove(l, l->head->key);

  free(l);

  return ll_Ok;
}

ll_ret ll_insert(linked_list * l, char * key){
  ll_node * new_node = NULL;

  if(l == NULL || key == NULL)
    return ll_ErrParm;

  if( ll_search(l, key) == ll_Found )
    return ll_PrevInserted ;

  new_node = (ll_node *) malloc(sizeof(ll_node));
  if(new_node == NULL)
    return ll_NoMem;

  new_node->key  = key;
  new_node->next = NULL;

  /* Empty list */
  if(l->head == NULL)
    l->head = new_node;
  else {
    new_node->next = l->head;
    l->head = new_node;
  }

  return ll_Ok;
}

ll_ret ll_remove(linked_list * l, char * key){
  ll_node * cur, * prev;

  if(l == NULL || key == NULL)
    return ll_ErrParm;

  prev = NULL;
  cur = l->head;

  while(cur != NULL && (strcmp(key, cur->key) != 0) ){
    prev = cur;
    cur  = cur->next;
  }

  /* We're removing the head */
  if(prev == NULL && cur != NULL)
    l->head = l->head->next;
  else if(cur != NULL)
    prev->next = cur->next;
  else
    return ll_NotFound;
  
  free(cur);

  return ll_Ok;
}

ll_ret ll_search(linked_list * l, char * key){
  ll_node * itr = NULL;

  if(l == NULL || key == NULL)
    return ll_ErrParm;

  itr = l->head;
  while( itr != NULL && (strcmp(key, itr->key) != 0) )
    itr = itr->next;

  if(itr == NULL)
    return ll_NotFound;

  return ll_Found;
}