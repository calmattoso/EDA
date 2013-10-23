#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "..//btree//btree.h"
#include "..//timer//CPUTimer.h"

#define OP_OK         0
#define PREV_INSERTED 1
#define NOT_FOUND     2
#define HASH_FULL     3

KBTREE_INIT(str, char *, kb_str_cmp);

char set[200];
CPUTimer insTime;
CPUTimer searchTime;
CPUTimer remTime;
 
int insert(kbtree_t(str) * h, char * s){
  char * new_s = (char *) malloc(strlen(s) + 1);
 
  if(new_s == NULL){
    printf("ERROR: No memory for new string!\n");
    exit(1);
  }
 
  strcpy(new_s, s);
 
  /* Insert into Hash Table */
  insTime.start();
    if(kb_get(str, h, new_s) != 0){
      insTime.stop();
      return PREV_INSERTED;
    }

    kb_put(str, h, new_s);
  insTime.stop();
 
  return OP_OK;
}

int search(kbtree_t(str) * h, char * s){
  searchTime.start();
    if( kb_get(str, h, s) != 0){
      searchTime.stop();
      return OP_OK;
    }
  searchTime.stop();
 
  return NOT_FOUND;
}

int _delete(kbtree_t(str) * h, char * s){
  remTime.start();
    if(kb_get(str, h, s) == 0){
      remTime.stop();
      return NOT_FOUND;
    }

    kb_del(str, h, s);
  remTime.stop();
 
  return OP_OK;  
} 

int main(){
  unsigned int n_lines, max_val, i, ret, a = 0, b = 0, c = 0; 
  char op;
  kbtree_t(str) *h;
   
  scanf(" %u %u", &n_lines, &max_val); 
  
  /* Create the Btree */
  h = kb_init(str, n_lines);
  if(h == NULL){
    printf("ERROR: Could not create BTree!\n");
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

        #ifdef _LOG
          if( ret  == OP_OK )
            printf("i   OK  %s\n", set);
          else if( ret == PREV_INSERTED )
            printf("i PREV  %s\n", set);
          else if( ret == HASH_FULL) {
            printf("ERROR: Hash if full!\n\tcould not insert %s\n", set);
            exit( 1 );
          }
        #endif

        a++;

      break;
      /* Search */
      case 'b':
        ret = search(h, set);

        #ifdef _LOG
          if( ret == OP_OK )
            printf("b  FND  %s\n", set);
          else
            printf("b ~FND  %s\n", set);
        #endif

        b++;

      break;
      /* Removal */
      case 'r':
        ret = _delete(h, set); 

        #ifdef _LOG
          if( ret == OP_OK )
            printf("r   OK  %s\n", set);
          else
            printf("r ~FND  %s\n", set);
        #endif
        
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
 
  __kb_destroy(h);
 
  return 0;
}

