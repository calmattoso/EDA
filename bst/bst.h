typedef struct _bst bst;

#define BST_NO_KEY -1

typedef enum {

  bst_Ok ,
  bst_Found ,
  bst_NotFound ,
  bst_PrevInserted ,
  bst_Full ,
  bst_NoMem ,
  bst_ErrParm 

} bst_condRet;

bst_condRet bst_create(bst ** arv, int num_elems);
bst_condRet bst_destroy(bst * arv);
bst_condRet bst_insert(bst * arv, char * key);
bst_condRet bst_remove(bst * arv, char * key);
bst_condRet bst_search(bst * arv, char * key);
