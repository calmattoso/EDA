#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "..//bst//bst.h"
#include "..//timer//CPUTimer.h"
 
#define OP_OK         0
#define PREV_INSERTED 1
#define NOT_FOUND     2
#define BST_FULL      3

char set[200];
CPUTimer insTime;
CPUTimer searchTime;
CPUTimer remTime;
 
int insert(bst * h, char * str){
  bst_ret ret;
  char * new_str = (char *) malloc(strlen(str) + 1);
 
  if(new_str == NULL){
    printf("ERROR: No memory for new string!\n");
    exit(1);
  }
 
  strcpy(new_str, str);
 
  /* Insert into Hash Table */
  insTime.start();
    ret = bst_insert(h, new_str);
  insTime.stop();
 
  if(ret == bst_NoMem){
    printf("ERROR: No memory for new node!\n");
    exit(1);
  }
  /* We could insert the set */
  if(ret == bst_Ok)
    return OP_OK;
  /* We had already inserted the set */
  if(ret == bst_PrevInserted){
    free(new_str);
    return PREV_INSERTED;
  }
 
  /* The hash table is full */
  return BST_FULL; 
}

int search(bst * h, char * str){
  bst_ret ret;
 
  searchTime.start();
    ret = bst_search(h, str);
  searchTime.stop();

  if(ret == bst_Found)
    return OP_OK;
 
  return NOT_FOUND;
}

int _delete(bst * h, char * str){
  bst_ret ret;
 
  remTime.start();
    ret = bst_remove(h, str);
  remTime.stop();

  if(ret == bst_NotFound)
    return NOT_FOUND;
 
  return OP_OK;  
} 

int main(){
  unsigned int n_lines, max_val, i, ret, a = 0, b = 0, c = 0; 
  bst_ret b_ret;
  char op;
  bst * h = NULL;
   
  scanf(" %u %u", &n_lines, &max_val); 
  
  /* Create the binary tree */
  b_ret = bst_create(&h, n_lines);
  if(b_ret == bst_NoMem){
    printf("ERROR: No memory to create bst tree!\n");
    printf("\t\tSize: %u\n", n_lines);
    exit(1);
  }

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
        ret = insert(h, set);

        if( ret  == OP_OK )
          printf("i   OK  %s\n", set);
        else if( ret == PREV_INSERTED )
          printf("i PREV  %s\n", set);
        else if( ret == BST_FULL) {
          printf("ERROR: bst if full!\n\tcould not insert %s\n", set);
          exit( 1 );
        }
        a++;

      break;
      /* Search */
      case 'b':
        ret = search(h, set);

        if( ret == OP_OK )
          printf("b  FND  %s\n", set);
        else
          printf("b ~FND  %s\n", set);
        b++;

      break;
      /* Removal */
      case 'r':
        ret = _delete(h, set); 

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
 
  bst_destroy(h);
 
  return 0;
}

