#include <stdlib.h>
#include <string.h>
#include "bst.h"

struct _bst
{
  char ** elems;          /* array to the sets' strings */
  unsigned int len;       /* current number of elements */
  unsigned int capacity;  /* maximum number of elements */
};

/**** Private functions prototypes ****/

static unsigned int find_min(bst * arv, unsigned int idx);

static unsigned int find_max(bst * arv, unsigned int idx);

static bst_condRet swap(bst * arv, unsigned int idx1, unsigned int idx2);

static bst_condRet remove(bst * arv, unsigned int idx);

/**** Exported functions code ****/

bst_condRet bst_create(bst ** arv, int num_elems){
  bst * aux_bst = NULL;
  int i;

  if(arv == NULL || num_elems < 1)
    return bst_ErrParm;

  /* create the tree */
  aux_bst = (bst *) malloc(sizeof(bst));
  (*arv) = aux_bst;
  if(aux_bst == NULL)
    return bst_NoMem;

  /* create the array of elements */
  aux_bst->elems = (char **) malloc((num_elems + 2) * sizeof(char *));
  if(aux_bst->elems == NULL)
    return bst_NoMem;

  /* set all current placeholder elements to empty */
  for(i = 0; i < num_elems; i++)
    aux_bst->elems[i] = NULL;

  aux_bst->len = 0;
  aux_bst->capacity = num_elems;

  return bst_Ok;
}

bst_condRet bst_destroy(bst * arv){
  if(arv == NULL || arv->elems == NULL)
    return bst_ErrParm;

  free(arv->elems);
  free(arv);
}

bst_condRet bst_insert(bst * arv, char * key){
  unsigned int idx = 0;

  if( arv == NULL || arv->elems == NULL )
    return bst_ErrParm;

  if(arv->len == arv->capacity)
    return bst_Full;

  while(arv->elems[idx] != NULL){
    /* left child */
    if( strcmp(key, arv->elems[idx]) < 0 )
      idx = 2 * idx + 1;
    /* right child */
    else if( strcmp(key, arv->elems[idx]) > 0 )
      idx = 2 * idx + 2;
    /* previously used key */
    else
      return bst_PrevInserted;
  }

  /* add the new element */
  arv->elems[idx] = key;

  /* update the number os elements */
  (arv->len)++;

  return bst_Ok;
}

bst_condRet bst_remove(bst * arv, char * key){
  unsigned int idx = 0;

  if( arv == NULL || arv->elems == NULL )
    return bst_ErrParm;

  while( arv->elems[idx] != NULL ){
    /* left child */
    if( strcmp(key, arv->elems[idx]) < 0 )
      idx = 2 * idx + 1;
    /* right child */
    else if( strcmp(key, arv->elems[idx]) > 0 )
      idx = 2 * idx + 2;
    /* we've found the key */
    else
      return remove(arv, idx);
  }

  return bst_NotFound;
}

bst_condRet bst_search(bst * arv, char * key){
  unsigned int idx = 0;

  if( arv == NULL || arv->elems == NULL )
    return bst_ErrParm;

  while( arv->elems[idx] != NULL ){
    /* left child */
    if( strcmp(key, arv->elems[idx]) < 0 )
      idx = 2 * idx + 1;
    /* right child */
    else if( strcmp(key, arv->elems[idx]) > 0 )
      idx = 2 * idx + 2;
    /* we've found the key */
    else
      return bst_Found;
  }

  return bst_NotFound; 
}

/**** Private functions code ****/

unsigned int find_min(bst * arv, unsigned int idx){
  unsigned int lchild = idx * 2 + 1;

  if(arv == NULL || idx < 0 || idx > arv->capacity)
    return -1;

  while(arv->elems[lchild] != NULL){
    idx    = lchild;
    lchild = idx * 2 + 1;
  }

  return idx;
}

unsigned int find_max(bst * arv, unsigned int idx){
  unsigned int rchild = idx * 2 + 2;

  if(arv == NULL || idx < 0 || idx > arv->capacity)
    return -1;

  while(arv->elems[rchild] != NULL){
    idx    = rchild;
    rchild = idx * 2 + 2;
  }

  return idx;
}

bst_condRet swap(bst * arv, unsigned int idx1, unsigned int idx2){
  char * temp = NULL;

  if(arv == NULL || idx1 < 0|| idx1 > arv->capacity ||
     idx2 < 0 ||  idx2 > arv->capacity)
    return bst_ErrParm;

  temp = arv->elems[idx1];
  arv->elems[idx1] = arv->elems[idx2];
  arv->elems[idx2] = temp;

  return bst_Ok;
}

bst_condRet remove(bst * arv, unsigned int idx){
  unsigned int lchild, rchild, aux;

  if(arv == NULL || idx < 0 || idx > arv->capacity)
    return bst_ErrParm;

  lchild = idx * 2 + 1;
  rchild = idx * 2 + 2;

  /* while the current node is not a leaf node */
  while( arv->elems[lchild] != NULL || 
         arv->elems[rchild] != NULL   )
  {
    /* The node has no left subtree */
    if(arv->elems[lchild] == NULL){
      aux = find_min(arv, rchild);
      swap(arv, idx, aux);
    }
    /* The node has or doesn't have a right subtree */
    else {
      aux = find_max(arv, lchild);
      swap(arv, idx, aux);
    }
        
    idx = aux;

    lchild = idx * 2 + 1;
    rchild = idx * 2 + 2;
  }

  arv->elems[idx] = NULL;

  return bst_Ok;
}
