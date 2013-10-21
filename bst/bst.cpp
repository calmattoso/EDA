#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#include "bst.h"

typedef struct node {
  char * key;
  node * parent;
  node * lchild;
  node * rchild;
} node;

struct _bst
{
  node * head;          /* array to the sets' strings */
  unsigned int len;       /* current number of elements */
  unsigned int capacity;  /* maximum number of elements */
};

/*****************************************************************************
 **** PRIVATE functions prototypes                                       *****
 *****************************************************************************/

static node * find_min(node * cur);

static node * find_max(node * cur);

static bst_ret swap(node * a, node * b);

static bst_ret remove(node * cur);

/*****************************************************************************
 **** EXPORTED functions code                                             ****
 *****************************************************************************/

bst_ret bst_create(bst ** arv, int num_elems){
  bst * aux_bst = NULL;
  int i;

  if(arv == NULL || num_elems < 1)
    return bst_ErrParm;

  /* create the tree */
  aux_bst = (bst *) malloc(sizeof(bst));
  (*arv) = aux_bst;
  if(aux_bst == NULL)
    return bst_NoMem;

  /* "Mark" the head node as empty */
  aux_bst->head = NULL;

  /* the tree currently has no elements */
  aux_bst->len = 0;
  aux_bst->capacity = num_elems;

  return bst_Ok;
}

bst_ret bst_destroy(bst * arv){
  unsigned int i = 0;
  stack<node *> nodes;
  
  if(arv == NULL || arv->head == NULL)
    return bst_ErrParm;  

  free(arv->head);
  free(arv);

  return bst_Ok;
}

bst_ret bst_insert(bst * arv, char * key){
  node * cur = NULL, * new_node = NULL;
  unsigned int idx = 0;
  int cmp = 0;

  if( arv == NULL )
    return bst_ErrParm;

  if(arv->len == arv->capacity)
    return bst_Full;
	
  /* Create new node */
  new_node = (node *) malloc(sizeof(node));
  if(new_node == NULL)
    return bst_NoMem;
	
  new_node->key    = key;
  new_node->lchild = NULL;
  new_node->rchild = NULL;
  new_node->parent = NULL;

  cur = arv->head;
  while( cur != NULL )
  {
	cmp = strcmp(key, cur->key);
  
    /* left child */
    if( cmp < 0 ){
	  if(cur->lchild == NULL){
		cur->lchild = new_node;
		new_node->parent = cur;
		break;
	  }
	  
	  cur = cur->lchild;
	}
    /* right child */
    else if( cmp > 0 ){
      if(cur->rchild == NULL){
		cur->rchild = new_node;
		new_node->parent = cur;
		break;
	  }
	  
	  cur = cur->rchild;
	}
    /* previously used key */
    else {
	  free(new_node);
      return bst_PrevInserted;
	}
  }
	
  /* The tree was empty */
  if(arv->head == NULL)
	arv->head = new_node;  

  /* update the number os elements */
  (arv->len)++;

  return bst_Ok;
}

bst_ret bst_remove(bst * arv, char * key){
  node * cur = NULL;
  int cmp = 0;

  if( arv == NULL )
    return bst_ErrParm;

  cur = arv->head;	
  while( cur != NULL )
  {
	cmp = strcmp(key, cur->key);
  
    /* left child */
    if( cmp < 0 )
      cur = cur->lchild;
    /* right child */
    else if( cmp > 0 )
      cur = cur->rchild;
    /* we've found the key */
    else
      return remove(cur);
  }

  return bst_NotFound;
}

bst_ret bst_search(bst * arv, char * key){
  node * cur = NULL;
  int cmp = 0;

  if( arv == NULL )
    return bst_ErrParm;

  cur = arv->head;
  while( cur != NULL )
  {
	cmp = strcmp(key, cur->key);
  
    /* left child */
    if( cmp < 0 )
	  cur = cur->lchild;
    /* right child */
    else if( cmp > 0 )
      cur = cur->rchild;
    /* we've found the key */
    else
      return bst_Found;
  }

  return bst_NotFound; 
}

/*****************************************************************************
 **** PRIVATE functions code                                              ****
 *****************************************************************************/

node * find_min(node * cur){
  if( cur == NULL )
    return NULL;
	
  while(cur->lchild != NULL)
    cur = cur->lchild;  

  return cur;
}

node * find_max(node * cur){
  if( cur == NULL )
    return NULL;

  while(cur->rchild != NULL)
    cur = cur->rchild;  

  return cur;
}

bst_ret swap(node * a, node * b){
  char * temp = NULL;

  if( a == NULL || b == NULL )
    return bst_ErrParm;

  temp   = a->key;
  a->key = b->key;
  b->key = temp;

  return bst_Ok;
}

bst_ret remove(node * cur){
  node * aux = NULL;

  if(cur == NULL)
    return bst_ErrParm;

  /* while the current node is not a leaf node */
  while( cur->lchild != NULL || cur->rchild != NULL )
  {
    /* The node has no left subtree */
    if(cur->lchild == NULL){
      aux = find_min(cur->rchild);
      swap(cur, aux);
    }
    /* The node has or doesn't have a right subtree */
    else {
      aux = find_max(cur->lchild);
      swap(cur, aux);
    }
        
    cur = aux;
  }
  
  /* Fix the the pointers of cur's parent */
  if(cur->parent->lchild == cur)
	cur->parent->lchild = NULL;
  else if(cur->parent->rchild == cur)
	cur->parent->rchild = NULL;

  free(cur->key);
  free(cur);

  return bst_Ok;
}
