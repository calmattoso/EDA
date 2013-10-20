
#include <stdio.h>

#include "CPUTimer.h"
#define MaxDim  100000
#define Rand_Max  100000

// Variáveis Globais

int Conjunto[MaxDim];
int Pi[MaxDim];

int * Qi;

int Dim;
int Fib_number[MaxDim];




static CPUTimer totaltime;
static CPUTimer roottime;

using namespace std;

int main (void){

	int k,i;
    double t;

    //Main Program
    k=0;
    totaltime.reset();
    totaltime.start();
    t = totaltime.getCPUTotalSecs();
    	//Execution 
    while(k<100000000){

    	k++;
    }
    printf("Agora\n");
    scanf("%d",&i);
    	//End Time
    totaltime.stop();
 	printf("\n Tempo Insertion Sort: %f  k=%d ttotal: %lf\n", (totaltime.getCPUTotalSecs()) , k, (totaltime.getCPUTotalSecs() - t) );




	return 0;
}