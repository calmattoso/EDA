/* Open addressed hash table with double hashing */
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "fnv.h"

#define true  1
#define false 0

typedef struct hash_node {
  char * str;
  int  removed;
} hash_node;

struct hash {
  hash_node * elems;
  unsigned int capacity;
  unsigned int size;
};

/*****************************************************************************
 **** PRIVATE data                                                       *****
 *****************************************************************************/

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

/*****************************************************************************
 **** PRIVATE functions prototypes                                       *****
 *****************************************************************************/

static unsigned int closest_prime(unsigned int n);

/* djb2 implementation from: http://www.cse.yorku.ca/~oz/hash.html */
/* credits also go to: http://programmers.stackexchange.com/questions/49550 */
static unsigned long djb2(unsigned char *str);

/*****************************************************************************
 **** EXPORTED functions code                                            *****
 *****************************************************************************/

hash_ret hash_create(hash ** h, unsigned int capacity){
  hash * h_aux = (hash *) malloc(sizeof(hash));
  int i = 0;

  if(h == NULL || capacity < 0)
    return hash_ErrParm;

  *h = h_aux;
  if(h_aux == NULL)
    return hash_NoMem;

  capacity = closest_prime(capacity);

  h_aux->elems = (hash_node *) malloc(capacity * sizeof(hash_node));
  if(h_aux->elems == NULL)
    return hash_NoMem;

  for(i = 0; i < capacity; i++){
    h_aux->elems[i].str = NULL;
    h_aux->elems[i].removed = false;
  }

  h_aux->size = 0;
  h_aux->capacity = capacity;

  return hash_Ok;
}

hash_ret hash_destroy(hash * h){
  if(h == NULL)
    return hash_ErrParm;

  free(h->elems);
  free(h);

  return hash_Ok;
}

hash_ret hash_insert(hash * h, char * str){
  Fnv32_t hash = fnv_32a_str(str, FNV1_32A_INIT);
  unsigned int counter = 0, first_empty = -1;
  unsigned long hash2 = djb2(str);

  if(h == NULL || str == NULL)
    return hash_ErrParm;

  hash = (hash % h->capacity);

  /* Find an empty bucket */
  while( h->elems[hash].str != NULL )
  {
    /* If the current bucket already has the key being inserted */
    if( strcmp( str, h->elems[hash].str ) == 0){
      h->elems[hash].removed = false;
      return hash_PrevInserted;
    }

    /* Find the first empty bucket */
    if(first_empty == -1 &&
       h->elems[hash].removed == true)
      first_empty = hash;

    /* Move to the next bucket */
    hash = (hash + hash2) % h->capacity;

    /* if we have seen all buckets, there are no empty ones */
    if(counter == h->size) 
      break;
    counter++;
  }
  
  if ( first_empty != -1 ) 
    hash = first_empty;
  else if( counter == h->size)
    return hash_Full;

  /* free(h->elems[hash].str); */

  h->elems[hash].str = str;
  h->elems[hash].removed = false;
  (h->size)++;

  return hash_Ok;
}

hash_ret hash_search(hash * h, char * str){
  Fnv32_t hash = fnv_32a_str(str, FNV1_32A_INIT);
  unsigned int counter = 0;
  unsigned long hash2 = djb2(str);

  if(h == NULL || str == NULL)
    return hash_ErrParm;
 
  hash = (hash % h->capacity);

  while( h->elems[hash].str != NULL &&
         strcmp( str, h->elems[hash].str ) != 0)
  {
    hash = (hash + hash2) % h->capacity;

    if(h->size == counter)
      return hash_NotFound;
    counter++;
  }

  if(h->elems[hash].str == NULL    ||
     h->elems[hash].removed == true  )
    return hash_NotFound;

  return hash_Found;
}

hash_ret hash_remove(hash * h, char * str){
  Fnv32_t hash = fnv_32a_str(str, FNV1_32A_INIT);
  unsigned int counter = 0;
  unsigned long hash2 = djb2(str);
  char * aux;

  if(h == NULL || str == NULL)
    return hash_ErrParm;

  hash = (hash % h->capacity);

  /* Find the bucket where the key is (if it exists) */
  while( h->elems[hash].str != NULL &&
         strcmp( str, h->elems[hash].str ) != 0){
    hash = (hash + hash2) % h->capacity;

    if(h->size == counter)
      return hash_NotFound;
    counter++;
  }

  if(h->elems[hash].str == NULL)
    return hash_NotFound;

  /* Mark the key as removed */
  h->elems[hash].removed = true;
  (h->size)--;

  return hash_Ok;
}

/*****************************************************************************
 **** PRIVATE functions code                                             *****
 *****************************************************************************/

unsigned int closest_prime(unsigned int n){
  int i;

  for(i = 0; i < PRIME_LIST_SIZE; i++)
    if(prime_list[i] >= n)
      return prime_list[i];

  return n;
}

unsigned long djb2(unsigned char *str){
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}
