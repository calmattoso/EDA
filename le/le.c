#include <stdio.h>
#include <string.h>

//A função tanto imprime em tela quanto gera um novo aquivo  .cqlv2

int le (char NomeArquivo[] ){
  char NomeSaida[100];
  char buffer[200];
  FILE * Original;
  FILE * Alterado;
 // char NomeArquivo[]= "C125.9.clq";
  printf("%s\n",NomeArquivo );
  strcpy (NomeSaida, NomeArquivo);
  strcat(NomeSaida, "v2");
   Original = fopen ( NomeArquivo,"r");
    if (Original==NULL){
    	return 1;
    }
   Alterado = fopen ( NomeSaida, "w+");
    if(Alterado==NULL){
    	return 1;
    } 

   while(buffer[0]!='e'){
      fgets(buffer,199,Original);
   }
   puts((buffer +1) );
   fputs((buffer+1),Alterado);

   while(fgets(buffer,199,Original)!=NULL){

   	    puts((buffer +1) );
   	    fputs((buffer+1),Alterado);
   }
   return 0;
}