#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include "..//avl//avl.h"
#include "..//timer//CPUTimer.h"
 
#define OP_OK         0
#define PREV_INSERTED 1
#define NOT_FOUND     2

char set[200];
CPUTimer insTime;
CPUTimer searchTime;
CPUTimer remTime;
 
int _delete(struct avl_table * avl_tree, char * str){
  void * ret = NULL;
 
  remTime.start();
    ret = avl_delete(avl_tree, str);
  remTime.stop();

  if(ret == NULL)
    return NOT_FOUND;
 
  free( ret );
  return OP_OK;  
} 
 
int insert(struct avl_table * avl_tree, char * str){
  void ** ret = NULL;
  char * new_str = (char *) malloc(strlen(str) + 1);
 
  if(new_str == NULL){
    printf("ERROR: No memory for new string!\n");
    exit(1);
  }
 
  strcpy(new_str, str);
 
  /* Insert into AVL */
  insTime.start();
    ret = avl_probe(avl_tree, new_str);
  insTime.stop();
 
  if(ret == NULL){
    printf("ERROR: No memory for new node!\n");
    exit(1);
  }
  /* We could insert the set */
  if(*ret == new_str)
    return OP_OK;
  /* We had already inserted the set */
  if(*ret != NULL){
    free(new_str);
    return PREV_INSERTED;
  }
 
  /* Something weird happened */
  return -1; 
}

int search(struct avl_table * avl_tree, char * str){
  void * ret = NULL;
 
  searchTime.start();
    ret = avl_find(avl_tree, str);
  searchTime.stop();

  if(ret != NULL)
    return OP_OK;
 
  return NOT_FOUND;
}

int compare_str (const void * key1, const void * key2, void * param){
  const char * a = (const char *) key1;
  const char * b = (const char *) key2;
 
  return strcmp(a, b);
}

void destroy_str (void * key, void * param){
  free( (char *) key);
}

int main(){
  unsigned int n_lines, max_val, i, ret, a = 0, b = 0, c = 0; 
  char op;
  struct avl_table * avl_tree = NULL;
  struct libavl_allocator aloc;
 
  scanf(" %u %u", &n_lines, &max_val);
 
  aloc.libavl_malloc = avl_malloc;
  aloc.libavl_free   = avl_free;  /* USA USA USA */
 
  /* Create the AVL tree */
  avl_tree = avl_create(compare_str, NULL, &aloc);
  if(avl_tree == NULL)
    exit(1);

  /* Intiatializing time variables */
  insTime.reset();
  searchTime.reset();
  remTime.reset();
 
  /* Read the input file */
  for(i = 0; i < n_lines; i++){
    scanf(" %c %*d %[^\n]", &op, set);
 
    switch( op ){
      /* Insertion */
      case 'i':
        ret = insert(avl_tree, set);

        if( ret  == OP_OK )
          printf("i   OK  %s\n", set);
        else if( ret == PREV_INSERTED )
          printf("i PREV  %s\n", set);
        else {
          printf("ERROR: could not insert %s\n", set);
          exit( 1 );
        }
        a++;

      break;
      /* Search */
      case 'b':
        ret = search(avl_tree, set);

        if( ret == OP_OK )
          printf("b  FND  %s\n", set);
        else
          printf("b ~FND  %s\n", set);
        b++;

      break;
      /* Removal */
      case 'r':
        ret = _delete(avl_tree, set); 

        if( ret == OP_OK )
          printf("r   OK  %s\n", set);
        else
          printf("r ~FND  %s\n", set);
        c++;

      break;
      /* Wrong code */
      default:
        printf("ERROR: Wrong input at line %u!\n", i+1);
        exit(1);
    } 
  }

  if(a == 0)
    a++;
  if(b == 0)
    b++;
  if(c == 0)
    c++;

  printf("\n\nSTATISTICS\n==========\n\n");
  /* Insertion */
  printf(" Insertion Total Time: %lfs\n\tInsertion Average Time: %.12lfms\n",
         insTime.getCPUTotalSecs(), (insTime.getCPUTotalSecs() / a) * 1000 );
  /* Search */
  printf(" Search Total Time:    %lfs\n\tSearch Average Time:    %.12lfms\n",
         searchTime.getCPUTotalSecs(), (searchTime.getCPUTotalSecs() / b) * 1000 );
  /* Removal */
  printf(" Remove Total Time:    %lfs\n\tRemove Average Time:    %.12lfms\n", 
         remTime.getCPUTotalSecs(), (remTime.getCPUTotalSecs() / c) * 1000 );
  /* Total running time */
  printf("Total Running time:    %lfs\n", ( remTime.getCPUTotalSecs() +  
         searchTime.getCPUTotalSecs() + insTime.getCPUTotalSecs() ) );
 
  avl_destroy(avl_tree, destroy_str);
 
  return 0;
}

