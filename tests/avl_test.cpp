#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..//avl//avl.h"
#include "..//timer//CPUTimer.h"

#define OP_OK         0
#define PREV_INSERTED 1
#define NOT_FOUND     2


int insert(struct avl_table * avl_tree, char * str);
int search(struct avl_table * avl_tree, char * str);
int del(struct avl_table * avl_tree, char * str);

int compare_str (const void * key1, const void * key2, void * param);
void destroy_str (void * key, void * param);

char set[200];
static CPUTimer insTime;
static CPUTimer searchTime;
static CPUTimer remTime;
int main(){
  unsigned int n_lines, max_val, i, ret, a=0, b=0, c=0; 
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
  //Intiatializing time variables
  insTime.reset();
  searchTime.reset();
  remTime.reset();

  /* Read the input file */
  for(i = 0; i < n_lines; i++){
    scanf(" %c %*d %[^\n]", &op, set);

    switch( op ){
      /* Insertion */
      case 'i':
        insTime.start();
        ret = insert(avl_tree, set);
        insTime.stop();
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
        searchTime.start();
        ret = search(avl_tree, set);
        searchTime.stop();
        if( ret == OP_OK )
          printf("b  FND  %s\n", set);
        else
          printf("b ~FND  %s\n", set);
        b++;
        break;
      /* Removal */
      case 'r':
        remTime.start();
        ret = del(avl_tree, set); 
        remTime.stop();
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
    printf("\n Insertion Total Time:%lf   Insertion Averarge Time: %lf\n", insTime.getCPUTotalSecs(), (insTime.getCPUTotalSecs() / a) );
    printf(" Search Total Time:%lf   Search Averarge Time: %lf\n", searchTime.getCPUTotalSecs(), (searchTime.getCPUTotalSecs() / b) );
    printf(" Remove Total Time:%lf   Remove Averarge Time: %lf\n", remTime.getCPUTotalSecs(), (remTime.getCPUTotalSecs() / c) );
    printf("Total Running time: %lf \n",( remTime.getCPUTotalSecs() +  searchTime.getCPUTotalSecs() + insTime.getCPUTotalSecs() ) );

  }

  avl_destroy(avl_tree, destroy_str);
  
  return 0;
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
  ret = avl_probe(avl_tree, new_str);

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

  ret = avl_find(avl_tree, str);
  if(ret != NULL)
    return OP_OK;
  
  return NOT_FOUND;
}

int del(struct avl_table * avl_tree, char * str){
  void * ret = NULL;

  ret = avl_delete(avl_tree, str);
  if(ret == NULL)
    return NOT_FOUND;
  
  free( ret );
  return OP_OK;
}


int compare_str (const void * key1, const void * key2, void * param){
  const char * a = (const char *) key1;
  const char * b = (const char *) key2;

  return strcmp(a, b);
}

void destroy_str (void * key, void * param){
  free( (char *) key);
}

