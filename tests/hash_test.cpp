#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "..//hash//hash.h"
#include "..//timer//CPUTimer.h"
 
#define OP_OK         0
#define PREV_INSERTED 1
#define NOT_FOUND     2
#define HASH_FULL     3

char set[200];
CPUTimer insTime;
CPUTimer searchTime;
CPUTimer remTime;
 
int insert(hash * h, char * str){
  hash_ret ret;
  char * new_str = (char *) malloc(strlen(str) + 1);
 
  if(new_str == NULL){
    printf("ERROR: No memory for new string!\n");
    exit(1);
  }
 
  strcpy(new_str, str);
 
  /* Insert into Hash Table */
  insTime.start();
    ret = hash_insert(h, new_str);
  insTime.stop();
 
  if(ret == hash_NoMem){
    printf("ERROR: No memory for new node!\n");
    exit(1);
  }
  /* We could insert the set */
  if(ret == hash_Ok)
    return OP_OK;
  /* We had already inserted the set */
  if(ret == hash_PrevInserted){
    free(new_str);
    return PREV_INSERTED;
  }
 
  /* The hash table is full */
  return HASH_FULL; 
}

int search(hash * h, char * str){
  hash_ret ret;
 
  searchTime.start();
    ret = hash_search(h, str);
  searchTime.stop();

  if(ret == hash_Found)
    return OP_OK;
 
  return NOT_FOUND;
}

int _delete(hash * h, char * str){
  hash_ret ret;
 
  remTime.start();
    ret = hash_remove(h, str);
  remTime.stop();

  if(ret == hash_NotFound)
    return NOT_FOUND;
 
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

int main(){
  unsigned int n_lines, max_val, i, ret, a = 0, b = 0, c = 0; 
  hash_ret h_ret;
  char op;
  hash * h = NULL;
   
  scanf(" %u %u", &n_lines, &max_val); 
  
  /* Create the Hash Table */
  h_ret = hash_create(&h, (unsigned int)( n_lines * 1.5 ));
  if(h_ret == hash_NoMem){
    printf("ERROR: No memory to create hash table!\n");
    printf("\t\tSize: %u\n", n_lines * 1.5);
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
 
  hash_destroy(h);
 
  return 0;
}

