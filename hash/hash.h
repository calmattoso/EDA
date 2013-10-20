#ifndef HASH_H
#define HASH_H

typedef struct hash hash;

typedef enum {

  hash_Ok ,
  hash_NoMem ,
  hash_Found ,
  hash_NotFound ,
  hash_Full ,
  hash_PrevInserted ,
  hash_ErrParm

} hash_ret;

hash_ret hash_create(hash ** h, unsigned int capacity);
hash_ret hash_destroy(hash * h);
hash_ret hash_insert(hash * h,  char * str);
hash_ret hash_search(hash * h,  char * str);
hash_ret hash_remove(hash * h,  char * str);

#endif