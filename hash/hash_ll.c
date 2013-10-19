#include <stdlib.h>
#include "..\\linked_list\\linked_list.h"
#include "hash.h"
#include "fnv.h"

struct hash
{
  linked_list ** elems;
  unsigned int size;
  unsigned int capacity;
};

/**** Private data  ****/

#define PRIME_LIST_SIZE 32

/* Extracted from: http://attractivechaos.awardspace.com/khash.h.html */
static const unsigned int prime_list[PRIME_LIST_SIZE] =
{
  0ul,          3ul,          11ul,         23ul,         53ul,
  97ul,         193ul,        389ul,        769ul,        1543ul,
  3079ul,       6151ul,       12289ul,      24593ul,      49157ul,
  98317ul,      196613ul,     393241ul,     786433ul,     1572869ul,
  3145739ul,    6291469ul,    12582917ul,   25165843ul,   50331653ul,
  100663319ul,  201326611ul,  402653189ul,  805306457ul,  1610612741ul,
  3221225473ul, 4294967291ul
};

/**** Private functions prototypes ****/

static unsigned int closest_prime(unsigned int n);

/**** Exported functions code ****/

hash_ret hash_create(hash ** h, unsigned int capacity){
  hash * h_aux = NULL;
  unsigned int i = 0;

  if(h == NULL || capacity < 0)
    return hash_ErrParm;

  h_aux = (hash *) malloc(sizeof(hash));
  *h = h_aux;
  if(h_aux == NULL)
    return hash_NoMem;

  capacity = closest_prime(capacity);

  h_aux->elems = (linked_list **) malloc(capacity * sizeof(linked_list *));
  if(h_aux->elems == NULL)
    return hash_NoMem;

  for(i = 0; i < capacity; i++){
    if( ll_create( &(h_aux->elems[i]) ) == ll_NoMem )
      return hash_NoMem;
  }

  h_aux->size = 0;
  h_aux->capacity = capacity;

  return hash_Ok;
}

hash_ret hash_destroy(hash * h){
  unsigned int i;

  for(i = 0; i < h->capacity; i++)
    if(h->elems[i] != NULL)
      ll_destroy(h->elems[i]);

  free(h->elems);
  free(h);

  return hash_Ok;
}

hash_ret hash_insert(hash * h,  char * str){
  Fnv32_t hash = 0;
  ll_ret ret = 0;

  if(h == NULL || str == NULL)
    return hash_ErrParm;

  hash = ( fnv_32a_str(str, FNV1_32A_INIT) % (h->capacity) );

  ret = ll_insert(h->elems[hash], str);
  if( ret == ll_PrevInserted )
    return hash_PrevInserted;
  else if( ret == ll_NoMem )
    return hash_NoMem;

  (h->size)++;
  
  return hash_Ok;
}

hash_ret hash_search(hash * h,  char * str){
  Fnv32_t hash = 0;

  if(h == NULL || str == NULL)
    return hash_ErrParm;

  hash = ( fnv_32a_str(str, FNV1_32A_INIT) % (h->capacity) );

  if( ll_search(h->elems[hash], str) == ll_NotFound )
    return hash_NotFound;
  
  return hash_Found;
}

hash_ret hash_remove(hash * h,  char * str){
  Fnv32_t hash = 0;

  if(h == NULL || str == NULL)
    return hash_ErrParm;

  hash = ( fnv_32a_str(str, FNV1_32A_INIT) % (h->capacity) );

  if( ll_remove(h->elems[hash], str) == ll_NotFound )
    return hash_NotFound;

  (h->size)--;
  
  return hash_Ok;
}

/**** Private functions code ****/
static unsigned int closest_prime(unsigned int n){
  int i;

  for(i = 0; i < PRIME_LIST_SIZE; i++)
    if(prime_list[i] >= n)
      return prime_list[i];

  return n;
}