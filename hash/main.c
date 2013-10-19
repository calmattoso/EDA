#include <stdio.h>
#include "fnv.h"

int main(){
	char set[200];
	Fnv32_t hash ;
	unsigned long long int t = 4;

	printf("prime: %lu\n", FNV1_32A_INIT);

	scanf("%*[^\n]");
	while( scanf(" %*c %*d %[^\n]", set) == 1){
		printf("set: %s\n", set);

		hash = fnv_32a_str(set, FNV1_32A_INIT);
		printf("hash: %lu\n", hash);
	}

	return 0;
}