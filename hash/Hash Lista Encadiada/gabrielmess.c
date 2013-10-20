typedef struct hash hash;


typedef struct hash_node {
	char * str;
	CNode * next;  		
  	Status status;
} hash_node;

typedef struct{
	char * str;
	CNode *next;

}CNode;

struct hash {
  hash_node * elems;
  unsigned int capacity;
  unsigned int size;
};

typedef enum{
	removido,
	colidiu
}Status;

typedef enum {

  hash_Ok ,
  hash_NoMem ,
  hash_Found ,
  hash_NotFound ,
  hash_ErrParm

} hash_ret;

hash_ret hash_create(hash ** h, unsigned int capacity);
hash_ret hash_insert(hash * h,  char * str);
hash_ret hash_search(hash * h,  char * str);
hash_ret hash_remove(hash * h,  char * str);