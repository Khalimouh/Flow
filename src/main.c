#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct {
	char L[3][16];
	char F[8];
}LFSR;

void initialiserLFSR(const char* path,LFSR* gen){
	FILE* file = fopen(path,"r");
	assert(file);
	char* buffer;
	ssize_t read;
	size_t len = 0;
	int i = 0;
    while ((read = getline(&buffer, &len, file)) != -1) {
    	read = getline(&buffer, &len, file);
       // printf("%s", buffer);
       	strcpy(gen->L[i],buffer);
       	//gerer derniere ligne
       //		if(i > 3) strncpy(gen->F,buffer,8);
       	i++;
    }
    printf("\n");

	
	fclose(file);
}

/**
 *	Usage  : ./Flow fichier n
 */

int main(int argc, char const *argv[])
{
	if(argc != 3){ fprintf(stderr, "Usage: Usage  : ./Flow fichier n\n"); exit(0);}
	//Ouvir le fichier et initialiser le LFSR
	LFSR* Geffe = (LFSR*) malloc(sizeof(LFSR));
	initialiserLFSR(argv[1],Geffe);
	for (int i = 0; i < 3; ++i)
	{
		printf("%s\n", Geffe->L[i]);
	}
  	
  	printf("%s\n", Geffe->F);
	return 0;
}
